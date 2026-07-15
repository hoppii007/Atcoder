// AtCoder Problem Picker — problems/ に未記録の問題からランダムに1問開く。
// pick_problem.py と同じ判定ロジック（md の有無で未挑戦を判定）を JS で持つ。
const vscode = require("vscode");
const fs = require("fs");
const path = require("path");

/** problems/ ディレクトリを持つワークスペースフォルダを探す。 */
function find_problems_dir() {
  const folders = vscode.workspace.workspaceFolders || [];
  for (const folder of folders) {
    const candidate = path.join(folder.uri.fsPath, "problems");
    if (fs.existsSync(candidate)) return candidate;
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

function url_of(contest, level) {
  return `https://atcoder.jp/contests/abc${contest}/tasks/abc${contest}_${level}`;
}

/** 未記録の (contest, level) を列挙する。 */
function build_candidates(problems_dir, from, to, levels) {
  const candidates = [];
  for (let contest = from; contest <= to; contest++) {
    for (const level of levels) {
      const note = path.join(problems_dir, `abc${contest}`, `abc${contest}_${level}.md`);
      if (!fs.existsSync(note)) candidates.push({ contest, level });
    }
  }
  return candidates;
}

function collect() {
  const problems_dir = find_problems_dir();
  if (!problems_dir) {
    vscode.window.showErrorMessage(
      "AtCoder Picker: problems/ フォルダを含むワークスペースが開かれていません。"
    );
    return null;
  }
  const { from, to, levels } = get_config();
  const candidates = build_candidates(problems_dir, from, to, levels);
  if (candidates.length === 0) {
    vscode.window.showInformationMessage("AtCoder Picker: 未挑戦の問題はありません。全て記録済みです。");
    return null;
  }
  return { candidates, from, to, levels };
}

/** ランダムに1問開く。 */
async function pick() {
  const result = collect();
  if (!result) return;
  const { candidates } = result;
  const chosen = candidates[Math.floor(Math.random() * candidates.length)];
  const label = `ABC${chosen.contest} ${chosen.level.toUpperCase()}`;
  await vscode.env.openExternal(vscode.Uri.parse(url_of(chosen.contest, chosen.level)));
  vscode.window.showInformationMessage(`AtCoder Picker: ${label} を開きました（未挑戦 ${candidates.length} 問中）`);
}

/** 一覧から選んで開く（QuickPick）。 */
async function list() {
  const result = collect();
  if (!result) return;
  const { candidates } = result;
  const items = candidates.map((c) => ({
    label: `ABC${c.contest} ${c.level.toUpperCase()}`,
    detail: url_of(c.contest, c.level),
    c,
  }));
  const picked = await vscode.window.showQuickPick(items, {
    placeHolder: `未挑戦 ${candidates.length} 問 — 開く問題を選択`,
    matchOnDetail: true,
  });
  if (picked) {
    await vscode.env.openExternal(vscode.Uri.parse(url_of(picked.c.contest, picked.c.level)));
  }
}

function activate(context) {
  context.subscriptions.push(
    vscode.commands.registerCommand("atcoderPicker.pick", pick),
    vscode.commands.registerCommand("atcoderPicker.list", list)
  );

  // ステータスバーのボタン（クリックでランダムピック）
  const button = vscode.window.createStatusBarItem(vscode.StatusBarAlignment.Left, 100);
  button.text = "$(rocket) ATC ランダム";
  button.tooltip = "未挑戦の AtCoder 問題をランダムに開く（右クリックで一覧）";
  button.command = "atcoderPicker.pick";
  button.show();
  context.subscriptions.push(button);
}

function deactivate() {}

module.exports = { activate, deactivate };
