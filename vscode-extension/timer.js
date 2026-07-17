// AtCoder Solve Timer — honban/main.cpp を解いている時間を計測し、
// 解き終わりに solved / felt / つまずき分類 を QuickPick で入力して
// 挑戦履歴（第N回）ブロックを生成する（CLAUDE.md §2 のフォーマット）。
//
// 目的: time_spent_min を自動で埋め、solved/felt/つまずき分類 の入力漏れを防ぐことで
// 弱点分析（CLAUDE.md 目的#1）に食わせる記録の質を、摩擦ゼロで底上げする。
const vscode = require("vscode");
const fs = require("fs");
const path = require("path");

// つまずき分類の統制語彙（tags.md の専用表と一致させること。勝手に増やさない）。
const STUMBLE_VOCAB = [
  { label: "方針決定", description: "どの手法・方向で攻めるかに気づけない／迷う" },
  { label: "状態設計", description: "DP等の状態変数の定義が決められない・誤る" },
  { label: "遷移・立式", description: "漸化式・遷移・数式への落とし込みで詰まる" },
  { label: "計算量見積もり", description: "想定計算量・間に合うかの判断ミス" },
  { label: "定数倍高速化", description: "漸近計算量は正しいが定数倍で落ちる" },
  { label: "数学", description: "余事象・組合せ・数論・確率などの立式で詰まる" },
  { label: "実装バグ", description: "方針は正しいが実装を誤る（添字・境界・初期値）" },
  { label: "エッジケース", description: "特殊入力・境界条件の見落とし" },
  { label: "デバッグ", description: "バグの原因特定・修正に時間を溶かす" },
];

const SOLVED_OPTIONS = [
  { label: "自力AC", description: "指摘や解説なしに方針も実装も自分で到達" },
  { label: "解説AC", description: "鍵となる考察や実装を解説・指摘に依存" },
  { label: "未AC", description: "AC に至らなかった" },
];

const FELT_OPTIONS = [
  { label: "簡単" },
  { label: "中" },
  { label: "難しい" },
];

// 永続化キー（workspaceState）。リロードしてもタイマーが消えないようにする。
const KEY_STARTED_AT = "atcoderTimer.startedAt"; // 現在計測中セグメントの開始 epoch ms（停止中は null）
const KEY_ACCUMULATED = "atcoderTimer.accumulatedMs"; // 過去セグメントの累積 ms

let status_item = null;
let tick = null;
let ctx = null;

/** honban/ を持つワークスペースフォルダのパスを返す（無ければ最初のフォルダ）。 */
function find_workspace_root() {
  const folders = vscode.workspace.workspaceFolders || [];
  for (const folder of folders) {
    if (fs.existsSync(path.join(folder.uri.fsPath, "honban"))) return folder.uri.fsPath;
  }
  return folders.length ? folders[0].uri.fsPath : null;
}

function get_started_at() {
  return ctx.workspaceState.get(KEY_STARTED_AT, null);
}
function get_accumulated() {
  return ctx.workspaceState.get(KEY_ACCUMULATED, 0);
}
function is_running() {
  return get_started_at() !== null;
}

/** これまでの経過 ms（計測中セグメント + 累積）。 */
function elapsed_ms() {
  const acc = get_accumulated();
  const started = get_started_at();
  return started === null ? acc : acc + (Date.now() - started);
}

function fmt_clock(ms) {
  const total_sec = Math.floor(ms / 1000);
  const h = Math.floor(total_sec / 3600);
  const m = Math.floor((total_sec % 3600) / 60);
  const s = total_sec % 60;
  const mm = String(m).padStart(2, "0");
  const ss = String(s).padStart(2, "0");
  return h > 0 ? `${h}:${mm}:${ss}` : `${mm}:${ss}`;
}

function render_status() {
  if (!status_item) return;
  const icon = is_running() ? "$(debug-pause)" : "$(play)";
  status_item.text = `$(watch) ${fmt_clock(elapsed_ms())}`;
  status_item.tooltip =
    (is_running() ? "計測中 — クリックで一時停止" : "停止中 — クリックで開始") +
    "\n右のコマンドパレットから「AtCoder: 記録する」で solved/felt/つまずきを記録";
  // アイコンは経過表示に統一（トグルは text クリックで toggle コマンドが走る）
  void icon;
}

function start_ticking() {
  if (tick) return;
  tick = setInterval(render_status, 1000);
}
function stop_ticking() {
  if (tick) {
    clearInterval(tick);
    tick = null;
  }
}

async function do_start() {
  if (is_running()) return;
  await ctx.workspaceState.update(KEY_STARTED_AT, Date.now());
  start_ticking();
  render_status();
}

async function do_pause() {
  if (!is_running()) return;
  const acc = get_accumulated() + (Date.now() - get_started_at());
  await ctx.workspaceState.update(KEY_ACCUMULATED, acc);
  await ctx.workspaceState.update(KEY_STARTED_AT, null);
  stop_ticking();
  render_status();
}

async function do_toggle() {
  if (is_running()) await do_pause();
  else await do_start();
}

async function do_reset() {
  await ctx.workspaceState.update(KEY_STARTED_AT, null);
  await ctx.workspaceState.update(KEY_ACCUMULATED, 0);
  stop_ticking();
  render_status();
  vscode.window.showInformationMessage("AtCoder Timer: タイマーをリセットしました。");
}

/** タイマーをリセットして即スタートする（ATC ランダムで問題を開いたとき用）。 */
async function reset_and_start() {
  await ctx.workspaceState.update(KEY_ACCUMULATED, 0);
  await ctx.workspaceState.update(KEY_STARTED_AT, Date.now());
  start_ticking();
  render_status();
}

function today_iso() {
  // 拡張ホスト（Node）では Date が使える。ローカル日付で YYYY-MM-DD。
  const d = new Date();
  const y = d.getFullYear();
  const m = String(d.getMonth() + 1).padStart(2, "0");
  const day = String(d.getDate()).padStart(2, "0");
  return `${y}-${m}-${day}`;
}

/** 挑戦履歴（第N回）ブロックを組み立てる。N は不明なので placeholder。 */
function build_block({ date, minutes, solved, felt, stumbles }) {
  const stumble_str = stumbles.length ? `[${stumbles.join(", ")}]` : "[]";
  return [
    `### 第N回 — ${date}`,
    ``,
    `- solved: ${solved}`,
    `- felt: ${felt}`,
    `- time_spent_min: ${minutes}`,
    `- つまずき分類: ${stumble_str}`,
    ``,
    `**考察ノート**`,
    ``,
    ``,
    `**難所**`,
    ``,
    ``,
  ].join("\n");
}

/** 解き終わりの記録フロー。QuickPick で solved/felt/つまずき、time は自動。 */
async function do_record() {
  const auto_min = Math.max(1, Math.round(elapsed_ms() / 60000));

  const solved_pick = await vscode.window.showQuickPick(SOLVED_OPTIONS, {
    placeHolder: "solved — この回の結果は？（弱点分析の要）",
    ignoreFocusOut: true,
  });
  if (!solved_pick) return;

  const felt_pick = await vscode.window.showQuickPick(FELT_OPTIONS, {
    placeHolder: "felt — この回の体感難易度は？",
    ignoreFocusOut: true,
  });
  if (!felt_pick) return;

  const stumble_picks = await vscode.window.showQuickPick(STUMBLE_VOCAB, {
    placeHolder: "つまずき分類 — 詰まった工程を選択（無ければ何も選ばず Enter → []）",
    canPickMany: true,
    ignoreFocusOut: true,
  });
  if (stumble_picks === undefined) return; // Esc キャンセル。空配列（詰まり無し）は許容。

  const min_input = await vscode.window.showInputBox({
    prompt: "time_spent_min — 所要時間（分）。タイマーから自動算出。必要なら修正",
    value: String(auto_min),
    ignoreFocusOut: true,
    validateInput: (v) => (/^\d+$/.test(v.trim()) ? null : "正の整数で入力してください"),
  });
  if (min_input === undefined) return;

  const record = {
    date: today_iso(),
    minutes: parseInt(min_input.trim(), 10),
    solved: solved_pick.label,
    felt: felt_pick.label,
    stumbles: stumble_picks.map((p) => p.label),
  };

  const block = build_block(record);

  // 1) クリップボードへ（md に貼る／honban-review に渡す用）
  await vscode.env.clipboard.writeText(block);

  // 2) honban/.session.json に構造化データを残す（honban-review が拾える形）
  const root = find_workspace_root();
  if (root) {
    const honban_dir = path.join(root, "honban");
    try {
      if (!fs.existsSync(honban_dir)) fs.mkdirSync(honban_dir, { recursive: true });
      fs.writeFileSync(
        path.join(honban_dir, ".session.json"),
        JSON.stringify(record, null, 2) + "\n",
        "utf8"
      );
    } catch (e) {
      // 書き込み失敗はクリップボードがあるので致命的でない
    }
  }

  const choice = await vscode.window.showInformationMessage(
    `記録をクリップボードにコピーしました（${record.solved} / ${record.felt} / ${record.minutes}分）。タイマーをリセットしますか？`,
    "リセット",
    "そのまま"
  );
  if (choice === "リセット") await do_reset();
}

function activate(context) {
  ctx = context;

  status_item = vscode.window.createStatusBarItem(vscode.StatusBarAlignment.Left, 99);
  status_item.command = "atcoderTimer.toggle";
  status_item.show();
  render_status();
  context.subscriptions.push(status_item);

  // 計測中だった状態でリロードされた場合は tick を復帰
  if (is_running()) start_ticking();

  context.subscriptions.push(
    vscode.commands.registerCommand("atcoderTimer.toggle", do_toggle),
    vscode.commands.registerCommand("atcoderTimer.start", do_start),
    vscode.commands.registerCommand("atcoderTimer.pause", do_pause),
    vscode.commands.registerCommand("atcoderTimer.reset", do_reset),
    vscode.commands.registerCommand("atcoderTimer.record", do_record)
  );
}

function deactivate() {
  stop_ticking();
}

module.exports = { activate, deactivate, reset_and_start };
