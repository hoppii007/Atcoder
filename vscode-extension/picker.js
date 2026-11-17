// 問題の選び方（ランダムピック / URL 入力）をまとめたモジュール。
// 未挑戦の判定は pick_problem.py と同じ（problems/abcXXX/abcXXX_y.md の有無）。
const vscode = require("vscode");
const fs = require("fs");
const path = require("path");

/** problems/ を持つワークスペースフォルダのパスを返す（無ければ null）。 */
function find_root() {
  for (const folder of vscode.workspace.workspaceFolders || []) {
    if (fs.existsSync(path.join(folder.uri.fsPath, "problems"))) return folder.uri.fsPath;
  }
  return null;
}

/** 設定を読む。 */
function get_config() {
  const cfg = vscode.workspace.getConfiguration("atcoderPicker");
  return {
    from: cfg.get("from", 212),
    to: cfg.get("to", 318),
    levels: String(cfg.get("levels", "cde")).toLowerCase(),
  };
}

/**
 * @brief 問題を表すオブジェクトを作る
 * @param contest_id コンテスト ID（例 "abc318"）
 * @param task_id 問題 ID（例 "abc318_f"）
 */
function make_problem(contest_id, task_id) {
  const m = task_id.match(/^([a-z]+)(\d+)_([a-z0-9]+)$/);
  const label = m ? `${m[1].toUpperCase()}${m[2]} ${m[3].toUpperCase()}` : task_id;
  return {
    contest_id,
    task_id,
    label,
    url: `https://atcoder.jp/contests/${contest_id}/tasks/${task_id}`,
  };
}

/** そのノート md のパス（存在するとは限らない）。 */
function note_path(root, problem) {
  return path.join(root, "problems", problem.contest_id, `${problem.task_id}.md`);
}

/** 未記録の問題を列挙する。 */
function candidates(root) {
  const { from, to, levels } = get_config();
  const result = [];
  for (let c = from; c <= to; c++) {
    for (const level of levels) {
      const p = make_problem(`abc${c}`, `abc${c}_${level}`);
      if (!fs.existsSync(note_path(root, p))) result.push(p);
    }
  }
  return result;
}

/** 未記録の問題からランダムに1問（無ければ null）。 */
function pick_random(root) {
  const list = candidates(root);
  if (list.length === 0) return null;
  return list[Math.floor(Math.random() * list.length)];
}

/**
 * @brief URL や問題 ID の文字列から問題を作る
 * @note 受け付ける形: 問題 URL / "abc318_f" / "ABC318F" / "abc318 f"
 * @return 解釈できなければ null
 */
function parse_problem(text) {
  const s = String(text || "").trim();
  const url = s.match(/contests\/([A-Za-z0-9_-]+)\/tasks\/([A-Za-z0-9_-]+)/);
  if (url) return make_problem(url[1].toLowerCase(), url[2].toLowerCase());
  const id = s.match(/^([A-Za-z]+)(\d+)[\s_-]*([A-Za-z])$/);
  if (id) {
    const contest_id = `${id[1].toLowerCase()}${id[2]}`;
    return make_problem(contest_id, `${contest_id}_${id[3].toLowerCase()}`);
  }
  return null;
}

module.exports = { find_root, candidates, pick_random, parse_problem, note_path };
