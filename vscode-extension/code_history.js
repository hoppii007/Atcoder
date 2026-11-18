// コードの推移の記録 — 解いている間の honban/main.cpp を、区切りの操作ごとに経過時間つきで保存する。
//
// 保存先: honban/.history/<task_id>/NNN.cpp（内容が直前と同じなら新しいファイルは作らず、直前のファイルを指す）
// 一覧:   進行中の問題 cur.code_timeline = [{ms, event, file, exit?}]
//         記録時に .session.json の code_timeline（elapsed は "mm:ss"）として書き出す。
//
// event:
//   start  … 解き始めた時点（テンプレートの状態。以降の差分の基準）
//   save   … main.cpp を保存した
//   log    … 思考ログを1件書いた（そのときエディタに見えているコード）
//   build / input / run / expand / submit … 同名のタスクが終わった（exit は終了コード。build=1 ならコンパイルエラー）
//   finish … 「解き終わり」を押した
//
// スナップショットは honban-review skill がレビュー後に .session.json と一緒に消す。
const fs = require("fs");
const path = require("path");
const crypto = require("crypto");

/** 終了を記録するタスク（.vscode/tasks.json の label）。合成タスク build_input などはプロセスを持たないので来ない。 */
const TRACKED_TASKS = new Set(["build", "input", "run", "expand", "submit"]);

function main_path(root) {
  return path.join(root, "honban", "main.cpp");
}

function history_dir(root, task_id) {
  return path.join(root, "honban", ".history", task_id);
}

/** ワークスペース相対のスラッシュ区切りパス（session に書く用）。 */
function rel_history_dir(task_id) {
  return `honban/.history/${task_id}`;
}

/**
 * 同じファイルを指すか。シンボリックリンクは実体に解決し、Windows / mac（既定の APFS は大文字小文字を区別しない）
 * では大文字小文字を無視して比べる。
 */
function same_path(a, b) {
  const norm = (p) => {
    let r = path.resolve(p);
    try {
      r = fs.realpathSync.native(r);
    } catch {
      // 存在しないパスはそのまま比べる
    }
    return process.platform === "win32" || process.platform === "darwin" ? r.toLowerCase() : r;
  };
  return norm(a) === norm(b);
}

function hash(text) {
  return crypto.createHash("sha1").update(text).digest("hex");
}

/** 問題の履歴を消す（解き始め・破棄・記録の上書き時）。 */
function clear(root, task_id) {
  if (!root || !task_id) return;
  fs.rmSync(history_dir(root, task_id), { recursive: true, force: true });
}

/**
 * main.cpp の現在の内容を保存し、cur.code_timeline に1件追加する（cur を書き換える。保存は呼び出し側）。
 * @param root   ワークスペースのルート
 * @param cur    進行中の問題（problem.task_id, code_timeline を持つ）
 * @param ms     タイマーの経過時間
 * @param event  上記の event 名
 * @param text   保存する内容。省略時はディスク上の main.cpp を読む
 * @param exit   タスクの終了コード（タスクのときだけ）
 * @return 追加したかどうか（main.cpp が読めなければ false）
 */
function snapshot(root, cur, ms, event, text, exit) {
  if (!root || !cur) return false;
  if (text === undefined) {
    try {
      text = fs.readFileSync(main_path(root), "utf8");
    } catch {
      return false;
    }
  }
  if (!Array.isArray(cur.code_timeline)) cur.code_timeline = [];
  const tl = cur.code_timeline;
  const h = hash(text);
  let file;
  const last = [...tl].reverse().find((e) => e.file);
  if (last && last.hash === h) {
    file = last.file; // 内容が変わっていない
  } else {
    const n = new Set(tl.map((e) => e.file)).size + 1;
    file = `${String(n).padStart(3, "0")}.cpp`;
    const dir = history_dir(root, cur.problem.task_id);
    fs.mkdirSync(dir, { recursive: true });
    fs.writeFileSync(path.join(dir, file), text, "utf8");
  }
  // 内容が同じ save が続くだけなら記録しない（保存キーの連打）
  if (event === "save" && last && last.hash === h && tl[tl.length - 1].event === "save") return false;
  const entry = { ms, event, file, hash: h };
  if (exit !== undefined && exit !== null) entry.exit = exit;
  tl.push(entry);
  return true;
}

/** session に書き出す形（hash は内部用なので落とす）。 */
function to_session(cur, fmt_elapsed) {
  return (cur.code_timeline || []).map((e) => {
    const o = { elapsed: fmt_elapsed(e.ms), event: e.event, file: e.file };
    if (e.exit !== undefined) o.exit = e.exit;
    return o;
  });
}

module.exports = {
  TRACKED_TASKS,
  main_path,
  history_dir,
  rel_history_dir,
  same_path,
  clear,
  snapshot,
  to_session,
};
