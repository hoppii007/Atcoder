// #include "library/..." の行にカーソルを合わせると、そのライブラリの使い方を表示する。
// 表示内容: @brief / 先頭の Doxygen ブロック（@code の使用例を含む）/ 定義一覧 / 依存ライブラリ / ファイルを開くリンク
const vscode = require("vscode");
const fs = require("fs");
const path = require("path");

const INCLUDE_LINE_RE = /^\s*#\s*include\s*"(library\/[^"]+)"/;

/** workspace 直下に library/ を持つフォルダを返す。 */
function find_root() {
  for (const folder of vscode.workspace.workspaceFolders || []) {
    if (fs.existsSync(path.join(folder.uri.fsPath, "library"))) return folder.uri.fsPath;
  }
  return null;
}

/**
 * @brief コメント・文字列を空白に置き換えたテキストを返す（行構造は保つ）
 * @note 波括弧の深さを数えるための前処理
 */
function mask_comments_and_strings(text) {
  let out = "";
  let i = 0;
  while (i < text.length) {
    const two = text.slice(i, i + 2);
    if (two === "/*") {
      const end = text.indexOf("*/", i + 2);
      const stop = end === -1 ? text.length : end + 2;
      out += text.slice(i, stop).replace(/[^\n]/g, " ");
      i = stop;
    } else if (two === "//") {
      let end = text.indexOf("\n", i);
      if (end === -1) end = text.length;
      out += " ".repeat(end - i);
      i = end;
    } else if (text[i] === '"' || text[i] === "'") {
      const quote = text[i];
      let j = i + 1;
      while (j < text.length && text[j] !== quote && text[j] !== "\n") j += text[j] === "\\" ? 2 : 1;
      out += " ".repeat(Math.min(j + 1, text.length) - i);
      i = j + 1;
    } else {
      out += text[i];
      i++;
    }
  }
  return out;
}

/** @brief Doxygen ブロックや // コメント行から説明文（@brief 優先）を取り出す */
function brief_of(comment_lines) {
  const joined = comment_lines.join("\n");
  const m = joined.match(/@brief\s+(.+)/);
  if (m) return m[1].trim();
  for (const line of comment_lines) {
    const t = line.replace(/^\s*(\/\*\*?|\*\/|\*|\/\/\/?)\s?/, "").trim();
    if (t && !t.startsWith("@") && !t.startsWith("*/")) return t;
  }
  return "";
}

/**
 * @brief ライブラリファイルを解析する
 * @return { brief, head_doc, defs: [{signature, brief}], deps }
 */
function parse_library(file_path) {
  const text = fs.readFileSync(file_path, "utf8").replace(/\r\n/g, "\n");
  const lines = text.split("\n");
  const masked = mask_comments_and_strings(text).split("\n");

  const deps = [];
  const defs = [];
  let depth = 0;
  let pending_template = "";
  for (let i = 0; i < lines.length; i++) {
    const code = masked[i].trim();
    const at_top = depth === 0;
    if (at_top && code) {
      const inc = lines[i].match(/^\s*#\s*include\s*"([^"]+)"/);
      if (inc) {
        deps.push(inc[1]);
      } else if (!code.startsWith("#") && !code.startsWith("}")) {
        if (/^template\s*</.test(code) && !/[{;]/.test(code.replace(/^template\s*<.*?>/, ""))) {
          pending_template = code + " ";
        } else {
          // 直前のコメント（/** */ ブロックか連続する // 行）を説明として拾う
          const comment = [];
          let j = i - 1;
          if (pending_template) j--;
          while (j >= 0 && lines[j].trim() === "") j--;
          if (j >= 0 && lines[j].trim().endsWith("*/")) {
            while (j >= 0) {
              comment.unshift(lines[j]);
              if (lines[j].includes("/*")) break;
              j--;
            }
          } else {
            while (j >= 0 && /^\s*\/\//.test(lines[j]) && !/@no-include/.test(lines[j])) {
              comment.unshift(lines[j]);
              j--;
            }
          }
          const signature = (pending_template + code).replace(/\s*\{.*$/, "").replace(/\s+/g, " ").trim();
          defs.push({ signature, brief: brief_of(comment) });
          pending_template = "";
        }
      }
    }
    for (const ch of masked[i]) {
      if (ch === "{") depth++;
      else if (ch === "}") depth = Math.max(0, depth - 1);
    }
  }

  const block = text.match(/\/\*\*[\s\S]*?\*\//);
  let head_doc = block ? block[0] : "";
  if (!head_doc) {
    // Doxygen が無いファイルは、先頭の // コメント群を使う
    const head = [];
    for (const line of lines) {
      if (/^\s*#/.test(line) || line.trim() === "") {
        if (head.length) break;
        continue;
      }
      if (!/^\s*\/\//.test(line)) break;
      head.push(line);
    }
    head_doc = head.join("\n");
  }
  const brief = brief_of(head_doc.split("\n")) || (defs[0] && defs[0].brief) || "";
  return { brief, head_doc, defs, deps };
}

function provide_hover(document, position) {
  const line = document.lineAt(position).text;
  const m = line.match(INCLUDE_LINE_RE);
  if (!m) return undefined;
  const root = find_root();
  if (!root) return undefined;
  const file_path = path.join(root, m[1]);
  if (!fs.existsSync(file_path)) return undefined;

  let info;
  try {
    info = parse_library(file_path);
  } catch {
    return undefined;
  }

  const md = new vscode.MarkdownString();
  md.appendMarkdown(`**${m[1]}**`);
  if (info.brief) md.appendMarkdown(` — ${info.brief}`);
  md.appendMarkdown("\n\n");
  if (info.head_doc) md.appendCodeblock(info.head_doc, "cpp");
  if (info.defs.length) {
    md.appendMarkdown("\n**定義**\n\n");
    for (const d of info.defs) {
      md.appendMarkdown(`- \`${d.signature.replace(/`/g, "'")}\`${d.brief ? " — " + d.brief : ""}\n`);
    }
  }
  if (info.deps.length) {
    md.appendMarkdown(`\n**依存**: ${info.deps.map((d) => "`" + d + "`").join(", ")}\n`);
  }
  md.appendMarkdown(`\n[ファイルを開く](${vscode.Uri.file(file_path).toString()})`);

  const start = line.indexOf(m[1]);
  return new vscode.Hover(md, new vscode.Range(position.line, start, position.line, start + m[1].length));
}

function activate(context) {
  context.subscriptions.push(
    vscode.languages.registerHoverProvider([{ language: "cpp" }, { language: "c" }], { provideHover: provide_hover })
  );
}

module.exports = { activate, parse_library };
