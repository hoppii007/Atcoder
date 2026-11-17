// AtCoder Solve Timer — 精進パネルで解いている時間を計測する。
// 状態は workspaceState に永続化するので、リロードしても計測は続く。
// 記録の入力は精進パネル（shojin.js）が担い、このモジュールは時計だけを持つ。
const vscode = require("vscode");

// 永続化キー（workspaceState）
const KEY_STARTED_AT = "atcoderTimer.startedAt"; // 現在計測中セグメントの開始 epoch ms（停止中は null）
const KEY_ACCUMULATED = "atcoderTimer.accumulatedMs"; // 過去セグメントの累積 ms

let status_item = null;
let tick = null;
let ctx = null;
const listeners = new Set();

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

/** パネルに渡す状態（パネル側で毎秒の表示を計算する）。 */
function snapshot() {
  return { running: is_running(), started_at: get_started_at(), accumulated: get_accumulated() };
}

/**
 * @brief 状態が変わったときに呼ばれる関数を登録する
 * @return dispose() を持つオブジェクト
 */
function on_change(fn) {
  listeners.add(fn);
  return { dispose: () => listeners.delete(fn) };
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
  const icon = is_running() ? "$(watch)" : "$(debug-pause)";
  status_item.text = `${icon} ${fmt_clock(elapsed_ms())}`;
  status_item.tooltip = (is_running() ? "計測中" : "停止中") + " — クリックで精進パネルを開く";
}

function notify() {
  render_status();
  for (const fn of listeners) fn();
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

async function start() {
  if (is_running()) return;
  await ctx.workspaceState.update(KEY_STARTED_AT, Date.now());
  start_ticking();
  notify();
}

async function pause() {
  if (!is_running()) return;
  const acc = get_accumulated() + (Date.now() - get_started_at());
  await ctx.workspaceState.update(KEY_ACCUMULATED, acc);
  await ctx.workspaceState.update(KEY_STARTED_AT, null);
  stop_ticking();
  notify();
}

async function toggle() {
  if (is_running()) await pause();
  else await start();
}

async function reset() {
  await ctx.workspaceState.update(KEY_STARTED_AT, null);
  await ctx.workspaceState.update(KEY_ACCUMULATED, 0);
  stop_ticking();
  notify();
}

/** タイマーをリセットして即スタートする（問題を開いたとき用）。 */
async function reset_and_start() {
  await ctx.workspaceState.update(KEY_ACCUMULATED, 0);
  await ctx.workspaceState.update(KEY_STARTED_AT, Date.now());
  start_ticking();
  notify();
}

function activate(context) {
  ctx = context;

  status_item = vscode.window.createStatusBarItem(vscode.StatusBarAlignment.Left, 99);
  status_item.command = "atcoderShojin.open";
  status_item.show();
  render_status();
  context.subscriptions.push(status_item);

  // 計測中だった状態でリロードされた場合は tick を復帰
  if (is_running()) start_ticking();

  context.subscriptions.push(
    vscode.commands.registerCommand("atcoderTimer.toggle", toggle),
    vscode.commands.registerCommand("atcoderTimer.reset", reset)
  );
}

function deactivate() {
  stop_ticking();
}

module.exports = {
  activate,
  deactivate,
  start,
  pause,
  toggle,
  reset,
  reset_and_start,
  elapsed_ms,
  snapshot,
  on_change,
};
