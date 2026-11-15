// #include "library/..." のパス補完。
// `#include "` を打つと library/ を、`#include "library/` 以降はフォルダ・ファイルを候補に出す。
// ファイル候補には Doxygen の @brief を説明として表示する。
const vscode = require("vscode");
const fs = require("fs");
const path = require("path");

const INCLUDE_RE = /^\s*#\s*include\s*"([^"]*)$/;

/** workspace 直下に library/ を持つフォルダを返す。 */
function find_root() {
  for (const folder of vscode.workspace.workspaceFolders || []) {
    if (fs.existsSync(path.join(folder.uri.fsPath, "library"))) return folder.uri.fsPath;
  }
  return null;
}

/**
 * @brief ライブラリファイルから説明（@brief、なければ先頭コメント）とコメントブロックを取り出す
 * @param file_path ライブラリファイルの絶対パス
 * @return { brief, doc }
 */
function read_doc(file_path) {
  let text = "";
  try {
    text = fs.readFileSync(file_path, "utf8");
  } catch {
    return { brief: "", doc: "" };
  }
  const brief_match = text.match(/@brief\s+(.+)/);
  let brief = brief_match ? brief_match[1].trim() : "";
  if (!brief) {
    const comment = text.split(/\r?\n/).find((line) => /^\s*\/\/\s*\S/.test(line));
    brief = comment ? comment.replace(/^\s*\/\/\s*/, "").trim() : "";
  }
  const block_match = text.match(/\/\*\*[\s\S]*?\*\//);
  const doc = block_match ? block_match[0] : text.split(/\r?\n/).slice(0, 15).join("\n");
  return { brief, doc };
}

/** @brief 先頭行に `// @no-include` の目印があるか */
function is_no_include(file_path) {
  try {
    const head = fs.readFileSync(file_path, "utf8").slice(0, 200);
    return /^\s*\/\/\s*@no-include/.test(head);
  } catch {
    return false;
  }
}

function provide_items(document, position) {
  const before = document.lineAt(position).text.slice(0, position.character);
  const m = before.match(INCLUDE_RE);
  if (!m) return undefined;
  const root = find_root();
  if (!root) return undefined;

  const typed = m[1]; // 例: "library/Ma"
  const slash = typed.lastIndexOf("/");
  const dir_part = typed.slice(0, slash + 1); // 例: "library/"
  const partial = typed.slice(slash + 1); // 例: "Ma"
  const range = new vscode.Range(position.line, position.character - partial.length, position.line, position.character);

  // `#include "` 直後や "lib" 入力中は library/ を候補にする
  if (dir_part === "") {
    const item = new vscode.CompletionItem("library/", vscode.CompletionItemKind.Folder);
    item.range = range;
    item.sortText = "0";
    item.command = { command: "editor.action.triggerSuggest", title: "" };
    return [item];
  }
  if (!dir_part.startsWith("library/")) return undefined;

  const abs_dir = path.join(root, dir_part);
  let entries;
  try {
    entries = fs.readdirSync(abs_dir, { withFileTypes: true });
  } catch {
    return undefined;
  }

  const items = [];
  for (const entry of entries) {
    if (entry.isDirectory()) {
      const item = new vscode.CompletionItem(entry.name + "/", vscode.CompletionItemKind.Folder);
      item.range = range;
      item.sortText = "0" + entry.name.toLowerCase();
      item.command = { command: "editor.action.triggerSuggest", title: "" }; // フォルダ確定後も続けて候補を出す
      items.push(item);
    } else if (entry.isFile() && entry.name.endsWith(".cpp")) {
      const file_path = path.join(abs_dir, entry.name);
      if (is_no_include(file_path)) continue; // main に貼るスニペット専用のファイルは候補に出さない
      const { brief, doc } = read_doc(file_path);
      const item = new vscode.CompletionItem(
        { label: entry.name, description: brief },
        vscode.CompletionItemKind.File
      );
      item.range = range;
      item.sortText = "1" + entry.name.toLowerCase();
      item.filterText = entry.name;
      item.insertText = entry.name;
      item.documentation = new vscode.MarkdownString().appendCodeblock(doc, "cpp");
      items.push(item);
    }
  }
  return items;
}

function activate(context) {
  const provider = vscode.languages.registerCompletionItemProvider(
    [{ language: "cpp" }, { language: "c" }],
    { provideCompletionItems: provide_items },
    '"',
    "/"
  );
  context.subscriptions.push(provider);
}

module.exports = { activate };
