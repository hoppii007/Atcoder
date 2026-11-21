// 精進パネル — サイドバーのビューで「問題を選ぶ → 解く（タイマー）→ 記録する」を一続きで管理する。
//
// 画面は3つ:
//   idle      … ランダムピック or URL 入力で問題を選ぶ
//   solving   … タイマーと思考ログ（経過時間つきで1件ずつ追加。入力中の下書きは自動保存）
//   finishing … difficulty（AtCoder Problems から自動取得）・solved・時間を確認して記録
//
// 記録すると honban/.session.json に書き出す。Claude に「レビューして」と言うと、
// honban-review skill がこれを読み、思考ログを分析・レビューして
// problems/ のノートに記録する（記録は客観的事実だけ。felt は持たない）。
const vscode = require("vscode");
const fs = require("fs");
const path = require("path");
const crypto = require("crypto");
const timer = require("./timer");
const picker = require("./picker");
const difficulty = require("./difficulty");
const code_history = require("./code_history");
const rating = require("./rating");

const VIEW_ID = "atcoderShojin.panel";
const KEY_CURRENT = "atcoderShojin.current"; // 進行中の問題（無ければ null）

let ctx = null;
let view = null;
let fetching = false;

/**
 * 進行中の問題。思考ログ log: [{ms, text}]（ms はタイマーの経過時間）と、未投稿の入力 draft を持つ。
 * 旧形式（単一テキストの notes）が残っていれば、0:00 のログ1件として読み替える。
 */
function get_current() {
  const cur = ctx.workspaceState.get(KEY_CURRENT, null);
  if (cur && !Array.isArray(cur.log)) {
    cur.log = cur.notes ? [{ ms: 0, text: cur.notes }] : [];
    cur.draft = "";
    delete cur.notes;
  }
  return cur;
}

/** 経過時間 ms を "mm:ss"（1時間以上は "h:mm:ss"）にする。 */
function fmt_elapsed(ms) {
  const t = Math.floor(ms / 1000), h = Math.floor(t / 3600), m = Math.floor((t % 3600) / 60), s = t % 60;
  const mm = String(m).padStart(2, "0"), ss = String(s).padStart(2, "0");
  return h > 0 ? `${h}:${mm}:${ss}` : `${mm}:${ss}`;
}

/** 思考ログに1件追加する（空文字は無視）。 */
function push_log(cur, text) {
  const t = String(text || "").trim();
  if (!t) return false;
  cur.log.push({ ms: timer.elapsed_ms(), text: t });
  return true;
}
/** エディタで開いている honban/main.cpp の内容（未保存の編集を含む）。開いていなければ undefined。 */
function editor_main_text(root) {
  const doc = vscode.workspace.textDocuments.find((d) => code_history.same_path(d.uri.fsPath, code_history.main_path(root)));
  return doc ? doc.getText() : undefined;
}

/**
 * 進行中の問題があれば main.cpp のスナップショットを取る（コードの推移の記録。code_history.js 参照）。
 * 画面の再描画はしない（入力中の思考ログを上書きしないため）。
 */
async function record_code(event, text, exit) {
  const cur = get_current();
  const root = picker.find_root();
  if (!cur || !root) return;
  if (code_history.snapshot(root, cur, timer.elapsed_ms(), event, text, exit)) {
    await ctx.workspaceState.update(KEY_CURRENT, cur);
  }
}

async function set_current(cur) {
  // 呼び出し側が古い cur を持っている間（モーダル表示中など）に増えたスナップショットを失わない
  const stored = ctx.workspaceState.get(KEY_CURRENT, null);
  if (cur && stored && stored.problem && stored.problem.task_id === cur.problem.task_id &&
      (stored.code_timeline || []).length > (cur.code_timeline || []).length) {
    cur.code_timeline = stored.code_timeline;
  }
  await ctx.workspaceState.update(KEY_CURRENT, cur);
  post_state();
}

function today_iso() {
  const d = new Date();
  const m = String(d.getMonth() + 1).padStart(2, "0");
  const day = String(d.getDate()).padStart(2, "0");
  return `${d.getFullYear()}-${m}-${day}`;
}

function session_path(root) {
  return path.join(root, "honban", ".session.json");
}

/**
 * レビュー待ちのセッション（記録済みで、まだ Claude にレビューされていないもの）。
 * honban-review skill はノートに記録した後で .session.json を消すので、ファイルが残っていればレビュー待ち。
 * @return {{task_id: string, solved: string, time_spent_min: number|null}|null}
 */
function read_pending(root) {
  if (!root) return null;
  try {
    const s = JSON.parse(fs.readFileSync(session_path(root), "utf8"));
    return { task_id: s.task_id, solved: s.solved, time_spent_min: s.time_spent_min };
  } catch {
    return null;
  }
}

/** Claude Code（直前の会話）を開き、「レビューして」をクリップボードに入れる。 */
async function open_claude_for_review() {
  await vscode.env.clipboard.writeText("レビューして");
  try {
    await vscode.commands.executeCommand("claude-vscode.editor.openLast");
  } catch {
    vscode.window.showWarningMessage("精進パネル: Claude Code を開けませんでした。Claude Code に「レビューして」と貼り付けてください。");
    return;
  }
  vscode.window.showInformationMessage("「レビューして」をコピーしました。Claude Code に貼り付けて送信してください。");
}

function post_state() {
  if (!view) return;
  const root = picker.find_root();
  const cur = get_current();
  view.webview.postMessage({
    type: "state",
    current: cur,
    pending: read_pending(root),
    timer: timer.snapshot(),
    remaining: root ? picker.candidates(root).length : null,
    note_exists: !!(root && cur && fs.existsSync(picker.note_path(root, cur.problem))),
    fetching,
    rating: rating.snapshot(),
  });
}

/** 推定レートを計算し直す（終わったら画面を更新）。 */
function refresh_rating() {
  rating.refresh(picker.find_root(), ctx.globalStorageUri.fsPath, post_state);
}

/** 問題を開いて解答を始める。 */
async function begin(problem) {
  const cur = get_current();
  if (cur) {
    const ok = await vscode.window.showWarningMessage(
      `${cur.problem.label} が進行中です。破棄して ${problem.label} を始めますか？`,
      { modal: true },
      "破棄して始める"
    );
    if (!ok) return;
  } else {
    const pending = read_pending(picker.find_root());
    if (pending) {
      const choice = await vscode.window.showWarningMessage(
        `${pending.task_id} がまだレビューされていません。解いたときの会話が残っているうちに、先に Claude に「レビューして」と伝えるのがおすすめです。`,
        { modal: true },
        "先にレビューする",
        "このまま始める"
      );
      if (choice === "先にレビューする") await open_claude_for_review();
      if (choice !== "このまま始める") return;
    }
  }
  await vscode.env.openExternal(vscode.Uri.parse(problem.url));
  const root = picker.find_root();
  if (cur) code_history.clear(root, cur.problem.task_id);
  code_history.clear(root, problem.task_id);
  await set_current({ problem, phase: "solving", date: today_iso(), log: [], draft: "", difficulty: null, code_timeline: [] });
  await timer.reset_and_start();
  await record_code("start", editor_main_text(root));
}

async function begin_random() {
  const root = picker.find_root();
  if (!root) {
    vscode.window.showErrorMessage("精進パネル: problems/ を含むワークスペースが開かれていません。");
    return;
  }
  const p = picker.pick_random(root);
  if (!p) {
    vscode.window.showInformationMessage("精進パネル: 未挑戦の問題はありません。");
    return;
  }
  await begin(p);
}

async function begin_from_text(text) {
  const p = picker.parse_problem(text);
  if (!p) {
    vscode.window.showErrorMessage("精進パネル: URL か問題 ID（例 abc318_f）として解釈できませんでした。");
    return;
  }
  await begin(p);
}

/** 解き終わり: タイマーを止め、difficulty を取りに行く。 */
async function finish() {
  const cur = get_current();
  if (!cur) return;
  await timer.pause();
  await record_code("finish", editor_main_text(picker.find_root()));
  cur.phase = "finishing";
  cur.minutes = Math.max(1, Math.round(timer.elapsed_ms() / 60000));
  await set_current(cur);
  if (cur.difficulty === null) await fetch_difficulty();
}

async function fetch_difficulty() {
  const cur = get_current();
  if (!cur) return;
  fetching = true;
  post_state();
  try {
    const r = await difficulty.get_difficulty(ctx.globalStorageUri.fsPath, cur.problem.task_id);
    const now = get_current();
    if (now && now.problem.task_id === cur.problem.task_id) {
      now.difficulty = r.difficulty === null ? "" : String(r.difficulty);
      now.experimental = r.experimental;
      await ctx.workspaceState.update(KEY_CURRENT, now);
    }
  } catch (e) {
    vscode.window.showWarningMessage(`精進パネル: difficulty を取得できませんでした（${e.message}）。手入力してください。`);
  } finally {
    fetching = false;
    post_state();
  }
}

/** 記録: honban/.session.json に書き出して、進行中の問題を閉じる。 */
async function record(msg) {
  const cur = get_current();
  if (!cur) return;
  const root = picker.find_root();
  if (!root) return;
  const minutes = parseInt(msg.minutes, 10);
  const diff = String(msg.difficulty).trim() === "" ? null : parseInt(msg.difficulty, 10);
  push_log(cur, msg.draft); // 送信し忘れの入力があればログの最後に入れる
  const session = {
    task_id: cur.problem.task_id,
    contest_id: cur.problem.contest_id,
    url: cur.problem.url,
    note_path: path.relative(root, picker.note_path(root, cur.problem)).split(path.sep).join("/"),
    date: cur.date,
    difficulty: Number.isFinite(diff) ? diff : null,
    solved: msg.solved,
    time_spent_min: Number.isFinite(minutes) ? minutes : null,
    // 思考ログ（時刻付きの原文）。ノートに原文で残り、レビュー時に Claude が分析する
    thought_log: cur.log.map((e) => ({ elapsed: fmt_elapsed(e.ms), text: e.text })),
    // コードの推移（区切りの操作ごとの main.cpp のスナップショット。files は code_history_dir 以下）
    code_history_dir: code_history.rel_history_dir(cur.problem.task_id),
    code_timeline: code_history.to_session(get_current() || cur, fmt_elapsed),
  };
  const pending = read_pending(root);
  if (pending && pending.task_id !== session.task_id) {
    const ok = await vscode.window.showWarningMessage(
      `${pending.task_id} がまだレビューされていません。記録するとその分は上書きされて消えます。`,
      { modal: true },
      "上書きして記録"
    );
    if (!ok) return;
    code_history.clear(root, pending.task_id);
  }
  fs.mkdirSync(path.join(root, "honban"), { recursive: true });
  fs.writeFileSync(session_path(root), JSON.stringify(session, null, 2) + "\n", "utf8");
  await timer.reset();
  await set_current(null);
  const choice = await vscode.window.showInformationMessage(
    `${cur.problem.label} を記録しました（${session.solved} / ${session.time_spent_min}分）。まだノートには入っていません。Claude に「レビューして」と伝えてください。`,
    "Claude を開く"
  );
  if (choice) await open_claude_for_review();
}

async function abort() {
  const cur = get_current();
  if (!cur) return;
  const ok = await vscode.window.showWarningMessage(
    `${cur.problem.label} を記録せずに破棄しますか？`,
    { modal: true },
    "破棄"
  );
  if (!ok) return;
  code_history.clear(picker.find_root(), cur.problem.task_id);
  await timer.reset();
  await set_current(null);
}

async function on_message(msg) {
  const cur = get_current();
  switch (msg.type) {
    case "ready":
      post_state();
      break;
    case "random":
      await begin_random();
      break;
    case "start_url":
      await begin_from_text(msg.text);
      break;
    case "toggle":
      await timer.toggle();
      break;
    case "open_problem":
      if (cur) await vscode.env.openExternal(vscode.Uri.parse(cur.problem.url));
      break;
    case "draft":
      // 入力欄の自動保存（状態の再送はしない。入力中のテキストを上書きしないため）
      if (cur) {
        cur.draft = msg.text;
        await ctx.workspaceState.update(KEY_CURRENT, cur);
      }
      break;
    case "log_add":
      if (cur && push_log(cur, msg.text)) {
        cur.draft = "";
        await set_current(cur);
        await record_code("log", editor_main_text(picker.find_root()));
      }
      break;
    case "log_delete":
      if (cur && Number.isInteger(msg.index) && cur.log[msg.index]) {
        cur.log.splice(msg.index, 1);
        await set_current(cur);
      }
      break;
    case "finish":
      await finish();
      break;
    case "back":
      if (cur) {
        cur.phase = "solving";
        await set_current(cur);
        await timer.start();
      }
      break;
    case "refetch":
      await fetch_difficulty();
      break;
    case "record":
      await record(msg);
      break;
    case "abort":
      await abort();
      break;
    case "review":
      await open_claude_for_review();
      break;
    case "rating_refresh":
      refresh_rating();
      break;
  }
}

class ShojinViewProvider {
  resolveWebviewView(webview_view) {
    view = webview_view;
    webview_view.webview.options = { enableScripts: true };
    webview_view.webview.html = render_html(webview_view.webview);
    webview_view.webview.onDidReceiveMessage(on_message);
    webview_view.onDidDispose(() => {
      if (view === webview_view) view = null;
    });
    webview_view.onDidChangeVisibility(() => {
      if (webview_view.visible) post_state();
    });
  }
}

function render_html(webview) {
  const nonce = crypto.randomBytes(16).toString("hex");
  const csp = `default-src 'none'; style-src ${webview.cspSource} 'unsafe-inline'; script-src 'nonce-${nonce}';`;
  return /* html */ `<!doctype html>
<html lang="ja">
<head>
<meta charset="utf-8">
<meta http-equiv="Content-Security-Policy" content="${csp}">
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
  body { padding: 8px 12px 16px; font-family: var(--vscode-font-family); font-size: var(--vscode-font-size); color: var(--vscode-foreground); }
  h2 { font-size: 1em; font-weight: 600; margin: 18px 0 8px; text-transform: none; }
  h2:first-child { margin-top: 4px; }
  .muted { color: var(--vscode-descriptionForeground); font-size: 0.92em; }
  button { font: inherit; border: none; border-radius: 3px; padding: 6px 12px; cursor: pointer;
    background: var(--vscode-button-background); color: var(--vscode-button-foreground); }
  button:hover { background: var(--vscode-button-hoverBackground); }
  button.secondary { background: var(--vscode-button-secondaryBackground); color: var(--vscode-button-secondaryForeground); }
  button.secondary:hover { background: var(--vscode-button-secondaryHoverBackground); }
  button.link { background: none; color: var(--vscode-textLink-foreground); padding: 0; }
  button.link:hover { text-decoration: underline; background: none; }
  button.wide { width: 100%; padding: 9px 12px; }
  input, textarea { font: inherit; width: 100%; box-sizing: border-box; padding: 5px 7px; border-radius: 2px;
    color: var(--vscode-input-foreground); background: var(--vscode-input-background);
    border: 1px solid var(--vscode-input-border, transparent); }
  input:focus, textarea:focus { outline: 1px solid var(--vscode-focusBorder); outline-offset: -1px; }
  textarea { min-height: 160px; resize: vertical; font-family: var(--vscode-editor-font-family); }
  .row { display: flex; gap: 6px; align-items: center; }
  .row > input { flex: 1; }
  .stack > * + * { margin-top: 8px; }
  .problem { font-size: 1.35em; font-weight: 600; }
  .clock { font-size: 2.6em; font-variant-numeric: tabular-nums; letter-spacing: 0.02em; margin: 6px 0 2px; }
  .clock.paused { color: var(--vscode-descriptionForeground); }
  .seg { display: flex; gap: 4px; }
  .seg button { flex: 1; background: var(--vscode-button-secondaryBackground); color: var(--vscode-button-secondaryForeground); }
  .seg button.on { background: var(--vscode-button-background); color: var(--vscode-button-foreground); }
  label.field { display: block; margin: 14px 0 4px; font-weight: 600; }
  .divider { border-top: 1px solid var(--vscode-panel-border, rgba(128,128,128,.3)); margin: 18px 0 4px; }
  .warn { color: var(--vscode-editorWarning-foreground); }
  .hidden { display: none; }
  .pending { border-left: 3px solid var(--vscode-editorWarning-foreground); padding: 8px 10px; margin-bottom: 12px;
    background: var(--vscode-textBlockQuote-background, rgba(128,128,128,.1)); }
  .pending b { display: block; margin-bottom: 4px; }
  .pending button { margin-top: 8px; }
  .log { margin: 6px 0 8px; }
  .log-empty { color: var(--vscode-descriptionForeground); font-size: 0.92em; }
  .entry { display: flex; gap: 8px; padding: 5px 0; border-bottom: 1px solid var(--vscode-panel-border, rgba(128,128,128,.2)); }
  .entry .t { color: var(--vscode-descriptionForeground); font-variant-numeric: tabular-nums; flex: none; min-width: 3.2em; }
  .entry .x { flex: 1; white-space: pre-wrap; word-break: break-word; }
  .entry .del { flex: none; padding: 0 2px; color: var(--vscode-descriptionForeground); }
  textarea.input { min-height: 64px; }
  .rating .head { display: flex; justify-content: space-between; align-items: baseline; }
  .rating .big { font-size: 1.9em; font-weight: 700; font-variant-numeric: tabular-nums; }
  .rating .delta { font-variant-numeric: tabular-nums; margin-left: 6px; color: var(--vscode-descriptionForeground); }
  .rating .highest { color: var(--vscode-descriptionForeground); font-variant-numeric: tabular-nums; }
  .rating .sub { font-variant-numeric: tabular-nums; margin-top: 2px; }
  #r-chart { width: 100%; margin: 4px 0 2px; }
  #r-chart svg { display: block; width: 100%; user-select: none; }
  #r-chart svg text { font-family: var(--vscode-font-family); }
  .r-status { border-left: 4px solid var(--rank, transparent); padding: 4px 8px; margin: 4px 0 6px;
    background: var(--vscode-textBlockQuote-background, rgba(128,128,128,.1)); font-variant-numeric: tabular-nums; }
  .r-status .rr { font-weight: 700; }
</style>
</head>
<body>

<div id="pending" class="pending hidden">
  <b>レビュー待ち</b>
  <div id="pending-text"></div>
  <div class="muted">まだノートには記録されていません。Claude に「レビューして」と伝えると問題ノートに記録されます。</div>
  <button class="wide" id="btn-review">Claude を開く（「レビューして」をコピー）</button>
</div>

<section id="idle" class="hidden">
  <h2>推定レート</h2>
  <div class="rating" id="rating">
    <div class="head">
      <div><span class="big" id="r-main">—</span><span class="delta" id="r-delta"></span></div>
      <button class="link" id="btn-rating">再計算</button>
    </div>
    <div class="highest" id="r-highest"></div>
    <div id="r-chart"></div>
    <div class="r-status" id="r-status"></div>
    <div class="sub" id="r-ci"></div>
    <div class="sub" id="r-parts"></div>
    <div class="sub" id="r-recent"></div>
    <div class="muted" id="r-note"></div>
  </div>
  <div class="muted">各問題の最新回の solved（自力AC か）と所要時間を、AtCoder Problems の difficulty モデルに当てはめた推定（2026-11-01 以降で、所要時間の記録がある回だけを使う）。グラフの点は挑戦ごと（その時点までの記録での推定）。括弧は 80% 区間。</div>
  <h2>問題を選ぶ</h2>
  <div class="stack">
    <button class="wide" id="btn-random">ランダムに選んで開始</button>
    <div class="muted" id="remaining"></div>
  </div>
  <h2>URL・問題 ID で開始</h2>
  <div class="row">
    <input id="url" placeholder="https://atcoder.jp/... または abc318_f">
    <button id="btn-url">開始</button>
  </div>
  <div class="divider"></div>
  <p class="muted">開始すると問題がブラウザで開き、タイマーが動き始めます。</p>
</section>

<section id="solving" class="hidden">
  <div class="problem" id="s-label"></div>
  <button class="link" id="s-open">問題ページを開く</button>
  <div class="muted warn hidden" id="s-exists">この問題のノートは既にあります（記録すると次の回として追記）。</div>
  <div class="clock" id="s-clock">00:00</div>
  <div class="row">
    <button class="secondary" id="btn-toggle">一時停止</button>
  </div>
  <label class="field" for="s-input">思考ログ</label>
  <div class="muted">考えたことをその都度書いて追加（Ctrl+Enter）。経過時間つきで残ります。ログはそのままノートに残り、解いた後に Claude が分析します。</div>
  <div class="log" id="s-log"></div>
  <textarea class="input" id="s-input" placeholder="いま考えていること"></textarea>
  <div class="row" style="margin-top:4px"><button class="secondary" id="s-add">追加</button></div>
  <div class="stack" style="margin-top:12px">
    <button class="wide" id="btn-finish">解き終わった</button>
    <button class="link" id="btn-abort-s">記録せずに破棄</button>
  </div>
</section>

<section id="finishing" class="hidden">
  <div class="problem" id="f-label"></div>
  <div class="muted" id="f-date"></div>

  <label class="field">solved</label>
  <div class="seg" id="f-solved">
    <button data-v="自力AC">自力AC</button>
    <button data-v="解説AC">解説AC</button>
    <button data-v="未AC">未AC</button>
  </div>
  <div class="muted" style="margin-top:4px">解説AC = 鍵となる考察・実装を解説や指摘（Claude を含む）に頼った</div>

  <label class="field" for="f-diff">difficulty（AtCoder Problems）</label>
  <div class="row">
    <input id="f-diff" inputmode="numeric">
    <button class="secondary" id="btn-refetch">再取得</button>
  </div>
  <div class="muted" id="f-diff-note"></div>

  <label class="field" for="f-min">所要時間（分）</label>
  <input id="f-min" inputmode="numeric">

  <label class="field" for="f-input">思考ログ</label>
  <div class="log" id="f-log"></div>
  <textarea class="input" id="f-input" placeholder="書き足すことがあれば（解き終わった時点の時刻で入ります）"></textarea>
  <div class="row" style="margin-top:4px"><button class="secondary" id="f-add">追加</button></div>

  <div class="stack" style="margin-top:14px">
    <button class="wide" id="btn-record">記録する</button>
    <div class="row">
      <button class="secondary" id="btn-back">解答に戻る（タイマー再開）</button>
    </div>
    <button class="link" id="btn-abort-f">記録せずに破棄</button>
  </div>
</section>

<script nonce="${nonce}">
  const vscode = acquireVsCodeApi();
  const $ = (id) => document.getElementById(id);
  let state = null;
  let solved = null;
  let shown_task = null;   // 下書き欄に読み込んだ問題（別問題になったら入れ替える）
  let shown_phase = null;

  function fmt(ms) {
    const t = Math.floor(ms / 1000), h = Math.floor(t / 3600), m = Math.floor((t % 3600) / 60), s = t % 60;
    const mm = String(m).padStart(2, "0"), ss = String(s).padStart(2, "0");
    return h > 0 ? h + ":" + mm + ":" + ss : mm + ":" + ss;
  }
  function elapsed() {
    const t = state && state.timer;
    if (!t) return 0;
    return t.running ? t.accumulated + (Date.now() - t.started_at) : t.accumulated;
  }
  function show(id) {
    for (const s of ["idle", "solving", "finishing"]) $(s).classList.toggle("hidden", s !== id);
  }

  function render_log(id, log) {
    const box = $(id);
    box.textContent = "";
    if (!log.length) {
      const d = document.createElement("div");
      d.className = "log-empty";
      d.textContent = "（まだありません）";
      box.appendChild(d);
      return;
    }
    log.forEach((e, i) => {
      const row = document.createElement("div");
      row.className = "entry";
      const t = document.createElement("span"); t.className = "t"; t.textContent = fmt(e.ms);
      const x = document.createElement("span"); x.className = "x"; x.textContent = e.text;
      const del = document.createElement("button"); del.className = "link del"; del.textContent = "×"; del.title = "削除";
      del.onclick = () => vscode.postMessage({ type: "log_delete", index: i });
      row.append(t, x, del);
      box.appendChild(row);
    });
  }

  // AtCoder のレート色（点・文字）と、グラフ背景の帯の色（レーティンググラフと同じ淡色）
  const RANK = ["#808080", "#804000", "#008000", "#00C0C0", "#0000FF", "#C0C000", "#FF8000", "#FF0000"];
  const BAND = ["#D8D8D8", "#D8C5B2", "#B2D8B2", "#B2ECEC", "#B2B2FF", "#ECECB2", "#FFD8B2", "#FFB2B2"];
  const rank_of = (r) => Math.max(0, Math.min(7, Math.floor(r / 400)));
  const rank_color = (r) => RANK[rank_of(r)];
  const SVG_NS = "http://www.w3.org/2000/svg";
  let selected = null;   // グラフでホバー中の点（null なら最新）

  function svg_el(tag, attrs, parent) {
    const e = document.createElementNS(SVG_NS, tag);
    for (const k in attrs) e.setAttribute(k, attrs[k]);
    if (parent) parent.appendChild(e);
    return e;
  }
  function signed(d) { return (d > 0 ? "+" : d < 0 ? "−" : "±") + Math.abs(d); }

  /** 選択中（無ければ最新）の点の詳細を、グラフ下のボックスに出す。 */
  function render_status() {
    const h = state.rating && state.rating.history;
    const box = $("r-status");
    if (!h || !h.length) { box.classList.add("hidden"); return; }
    box.classList.remove("hidden");
    const i = selected === null ? h.length - 1 : selected, p = h[i];
    const d = i > 0 ? p.rating - h[i - 1].rating : null;
    box.style.setProperty("--rank", rank_color(p.rating));
    box.textContent = "";
    const l1 = document.createElement("div");
    const rr = document.createElement("span");
    rr.className = "rr"; rr.textContent = p.rating; rr.style.color = rank_color(p.rating);
    l1.append(rr, document.createTextNode((d === null ? "" : "（" + signed(d) + "）") + "  " + p.date.replace(/-/g, "/")));
    const l2 = document.createElement("div");
    const m = p.id.match(/^([a-z]+)(\\d+)_(\\w+)$/);
    const label = m ? m[1].toUpperCase() + m[2] + " " + m[3].toUpperCase() : p.id;
    l2.textContent = label + (p.no > 1 ? "（第" + p.no + "回）" : "") + " ／ " + p.solved
      + (p.time_min ? " " + p.time_min + "分" : "") + " ／ diff " + p.difficulty;
    const l3 = document.createElement("div");
    l3.className = "muted";
    l3.textContent = "80%区間 [" + p.lo + "–" + p.hi + "] ／ " + p.n + " 問時点";
    box.append(l1, l2, l3);
  }

  /** AtCoder のレーティンググラフ風の折れ線を描く。 */
  function draw_chart() {
    const box = $("r-chart");
    const h = state && state.rating && state.rating.history;
    box.textContent = "";
    if (!h || !h.length) return;
    const W = Math.max(200, box.clientWidth || 280);
    const H = Math.round(Math.min(260, Math.max(170, W * 0.6)));
    const L = 34, R = 10, T = 8, B = 20;
    const pw = W - L - R, ph = H - T - B;

    // x は挑戦の順番（等間隔）。同じ日に何問解いても潰れないようにする
    const xs = h.map((_, i) => i);
    const x0 = -0.6, x1 = Math.max(h.length - 1, 4) + 0.6;
    const rs = h.map((p) => p.rating);
    let y0 = Math.max(0, Math.floor((Math.min(...rs) - 100) / 400) * 400);
    let y1 = Math.ceil((Math.max(...rs) + 100) / 400) * 400;
    if (y1 - y0 < 800) y1 = y0 + 800;
    const X = (x) => L + (x - x0) / (x1 - x0) * pw;
    const Y = (r) => T + (y1 - r) / (y1 - y0) * ph;

    const svg = svg_el("svg", { viewBox: "0 0 " + W + " " + H, width: W, height: H }, box);
    const fg = getComputedStyle(document.body).getPropertyValue("--vscode-descriptionForeground") || "#888";

    // 色帯
    for (let b = Math.floor(y0 / 400); b * 400 < y1; b++) {
      const lo = Math.max(y0, b * 400), hi = Math.min(y1, (b + 1) * 400);
      svg_el("rect", { x: L, y: Y(hi), width: pw, height: Y(lo) - Y(hi), fill: BAND[Math.min(7, b)] }, svg);
    }
    // 横の目盛り（400 ごと）
    for (let r = Math.ceil(y0 / 400) * 400; r <= y1; r += 400) {
      if (r > y0 && r < y1) svg_el("line", { x1: L, x2: L + pw, y1: Y(r), y2: Y(r), stroke: "#FFFFFF", "stroke-width": 1 }, svg);
      const t = svg_el("text", { x: L - 4, y: Y(r) + 3.5, "text-anchor": "end", "font-size": 10, fill: fg }, svg);
      t.textContent = r;
    }
    // 縦の目盛り: 月が替わった挑戦の手前に線を引き、月を書く（詰まるラベルは飛ばす）
    let last_label = -Infinity;
    h.forEach((p, i) => {
      const ym = p.date.slice(0, 7);
      if (i > 0 && ym === h[i - 1].date.slice(0, 7)) return;
      const gx = i === 0 ? L : X(i - 0.5);
      if (i > 0) svg_el("line", { x1: gx, x2: gx, y1: T, y2: T + ph, stroke: "#FFFFFF", "stroke-width": 1 }, svg);
      const mo = +p.date.slice(5, 7);
      const label = (i === 0 || mo === 1 ? p.date.slice(0, 4) + "/" : "") + mo + "月";
      if (gx - last_label < label.length * 6 + 6) return;
      const t = svg_el("text", { x: gx + 2, y: H - 6, "text-anchor": "start", "font-size": 10, fill: fg }, svg);
      t.textContent = label;
      last_label = gx;
    });
    svg_el("rect", { x: L, y: T, width: pw, height: ph, fill: "none", stroke: "#888888", "stroke-width": 0.5 }, svg);

    // 折れ線と点
    svg_el("polyline", {
      points: h.map((p, i) => X(xs[i]) + "," + Y(p.rating)).join(" "),
      fill: "none", stroke: "#AAAAAA", "stroke-width": 2, "stroke-linejoin": "round",
    }, svg);
    const rad = h.length > 60 ? 2.5 : 3.5;
    h.forEach((p, i) => {
      svg_el("circle", { cx: X(xs[i]), cy: Y(p.rating), r: rad, fill: rank_color(p.rating), stroke: "#FFFFFF", "stroke-width": 0.8 }, svg);
    });

    // Highest（最初に最高値に達した点）
    let hi_i = 0;
    h.forEach((p, i) => { if (p.rating > h[hi_i].rating) hi_i = i; });
    const hx = X(xs[hi_i]), hy = Y(h[hi_i].rating);
    svg_el("circle", { cx: hx, cy: hy, r: rad + 3, fill: "none", stroke: rank_color(h[hi_i].rating), "stroke-width": 1.5 }, svg);
    const tag = "Highest: " + h[hi_i].rating;
    const tw = tag.length * 5.6 + 8, tx = Math.min(Math.max(hx - tw / 2, L + 2), L + pw - tw - 2);
    const ty = hy - 22 < T + 2 ? hy + 9 : hy - 22;
    svg_el("rect", { x: tx, y: ty, width: tw, height: 14, rx: 3, fill: "#FFFFFF", stroke: rank_color(h[hi_i].rating), "stroke-width": 1 }, svg);
    const ht = svg_el("text", { x: tx + tw / 2, y: ty + 10.5, "text-anchor": "middle", "font-size": 10, "font-weight": 700, fill: rank_color(h[hi_i].rating) }, svg);
    ht.textContent = tag;

    // 選択中の点の強調とホバー
    const sel = svg_el("circle", { r: rad + 2.5, fill: "none", stroke: "#000000", "stroke-width": 1.5, visibility: "hidden" }, svg);
    const place = () => {
      if (selected === null) { sel.setAttribute("visibility", "hidden"); return; }
      sel.setAttribute("cx", X(xs[selected])); sel.setAttribute("cy", Y(h[selected].rating));
      sel.setAttribute("visibility", "visible");
    };
    place();
    const hit = svg_el("rect", { x: L, y: T, width: pw, height: ph, fill: "transparent" }, svg);
    hit.addEventListener("mousemove", (ev) => {
      const rect = svg.getBoundingClientRect();
      const mx = (ev.clientX - rect.left) * W / rect.width;
      let best = 0;
      xs.forEach((x, i) => { if (Math.abs(X(x) - mx) < Math.abs(X(xs[best]) - mx)) best = i; });
      if (best !== selected) { selected = best; place(); render_status(); }
    });
    hit.addEventListener("mouseleave", () => { selected = null; place(); render_status(); });
  }

  function render_rating() {
    const r = state.rating;
    const est = (e) => e ? e.rating + " [" + e.lo + "–" + e.hi + "]" : "—";
    if (!r || !r.all) {
      $("r-main").textContent = "—";
      for (const id of ["r-delta", "r-highest", "r-ci", "r-parts", "r-recent"]) $(id).textContent = "";
      $("r-chart").textContent = "";
      $("r-status").classList.add("hidden");
      $("r-note").textContent = r && r.error ? "計算できませんでした: " + r.error : "計算中…";
      return;
    }
    const a = r.all, o = a.estimates.overall, h = r.history || [];
    $("r-main").textContent = o ? o.rating : "—";
    $("r-main").style.color = o ? rank_color(o.rating) : "";
    $("r-delta").textContent = h.length >= 2 ? "（" + signed(h[h.length - 1].rating - h[h.length - 2].rating) + "）" : "";
    const hi = h.length ? Math.max(...h.map((p) => p.rating)) : null;
    $("r-highest").textContent = hi === null ? "" : "Highest " + hi;
    $("r-ci").textContent = o ? "80%区間 [" + o.lo + "–" + o.hi + "]" : "記録なし";
    $("r-parts").textContent = o ? "正答のみ " + est(a.estimates.solve) + " ／ 速度のみ " + est(a.estimates.speed) : "";
    const ro = r.recent && r.recent.estimates.overall;
    $("r-recent").textContent = "直近" + r.recent_days + "日: " + (ro ? est(ro) + "（" + r.recent.n + "問）" : "記録なし");
    $("r-note").textContent = a.n + " 問（自力AC " + a.n_self + "・時間つき " + a.n_timed + "）"
      + (r.error ? " ／ 再計算に失敗: " + r.error : "");
    if (selected !== null && selected >= h.length) selected = null;
    if (r.computed_at !== drawn_at) {   // 再計算されたときだけ描き直す（タイマーの状態更新では描かない）
      drawn_at = r.computed_at;
      selected = null;
      draw_chart();
    }
    render_status();
  }
  let drawn_at = null;
  // パネルの幅が変わったら描き直す
  new ResizeObserver(() => { if (state && state.rating) draw_chart(); }).observe($("r-chart"));

  function render() {
    render_rating();
    const p = state.pending;
    $("pending").classList.toggle("hidden", !p);
    if (p) $("pending-text").textContent = p.task_id + "（" + p.solved + " / " + p.time_spent_min + "分）";
    const cur = state.current;
    if (!cur) {
      show("idle");
      shown_task = null; shown_phase = null;
      $("remaining").textContent = state.remaining === null ? "" : "未挑戦の候補 " + state.remaining + " 問（設定の範囲）";
      return;
    }
    const first = shown_task !== cur.problem.task_id || shown_phase !== cur.phase;
    if (cur.phase === "solving") {
      show("solving");
      $("s-label").textContent = cur.problem.label;
      $("s-exists").classList.toggle("hidden", !state.note_exists);
      $("btn-toggle").textContent = state.timer.running ? "一時停止" : "再開";
      render_log("s-log", cur.log);
      if (first) $("s-input").value = cur.draft || "";
    } else {
      show("finishing");
      $("f-label").textContent = cur.problem.label;
      $("f-date").textContent = cur.date + (state.note_exists ? "（既存ノートに次の回として追記）" : "（新規ノート）");
      if (first) {
        $("f-input").value = cur.draft || "";
        $("f-min").value = cur.minutes || "";
        solved = null;
      }
      if (state.fetching) {
        $("f-diff-note").textContent = "取得中…";
      } else {
        if (first || $("f-diff").value === "") $("f-diff").value = cur.difficulty === null ? "" : cur.difficulty;
        $("f-diff-note").textContent = cur.difficulty === "" ? "AtCoder Problems に値がありません（空欄のまま記録できます）"
          : cur.experimental ? "参考値（experimental）" : "";
      }
      render_log("f-log", cur.log);
      for (const b of $("f-solved").children) b.classList.toggle("on", b.dataset.v === solved);
    }
    shown_task = cur.problem.task_id; shown_phase = cur.phase;
  }

  setInterval(() => {
    if (state && state.current && state.current.phase === "solving") {
      $("s-clock").textContent = fmt(elapsed());
      $("s-clock").classList.toggle("paused", !state.timer.running);
    }
  }, 250);

  window.addEventListener("message", (e) => {
    if (e.data.type === "state") { state = e.data; render(); }
  });

  let save_timer = null;
  function save_draft(text) {
    clearTimeout(save_timer);
    save_timer = setTimeout(() => vscode.postMessage({ type: "draft", text }), 400);
  }
  function flush_draft(id) {
    clearTimeout(save_timer);
    vscode.postMessage({ type: "draft", text: $(id).value });
  }
  function add_log(id) {
    const text = $(id).value;
    if (!text.trim()) return;
    clearTimeout(save_timer);
    $(id).value = "";
    vscode.postMessage({ type: "log_add", text });
  }
  for (const k of ["s", "f"]) {
    $(k + "-input").oninput = (e) => save_draft(e.target.value);
    $(k + "-input").onkeydown = (e) => { if (e.key === "Enter" && (e.ctrlKey || e.metaKey)) { e.preventDefault(); add_log(k + "-input"); } };
    $(k + "-add").onclick = () => add_log(k + "-input");
  }

  $("btn-review").onclick = () => vscode.postMessage({ type: "review" });
  $("btn-rating").onclick = () => vscode.postMessage({ type: "rating_refresh" });
  $("btn-random").onclick = () => vscode.postMessage({ type: "random" });
  $("btn-url").onclick = () => vscode.postMessage({ type: "start_url", text: $("url").value });
  $("url").onkeydown = (e) => { if (e.key === "Enter") $("btn-url").click(); };
  $("s-open").onclick = () => vscode.postMessage({ type: "open_problem" });
  $("btn-toggle").onclick = () => vscode.postMessage({ type: "toggle" });
  $("btn-finish").onclick = () => {
    flush_draft("s-input");
    vscode.postMessage({ type: "finish" });
  };
  $("btn-back").onclick = () => {
    flush_draft("f-input");
    vscode.postMessage({ type: "back" });
  };
  $("btn-refetch").onclick = () => { $("f-diff").value = ""; vscode.postMessage({ type: "refetch" }); };
  $("btn-abort-s").onclick = () => vscode.postMessage({ type: "abort" });
  $("btn-abort-f").onclick = () => vscode.postMessage({ type: "abort" });
  for (const b of $("f-solved").children) {
    b.onclick = () => { solved = b.dataset.v; render(); };
  }
  $("btn-record").onclick = () => {
    if (!solved) { $("f-solved").scrollIntoView(); $("f-solved").animate([{ opacity: 0.3 }, { opacity: 1 }], 300); return; }
    if (!/^\\d+$/.test($("f-min").value.trim())) { $("f-min").focus(); return; }
    clearTimeout(save_timer);
    vscode.postMessage({
      type: "record", solved,
      difficulty: $("f-diff").value, minutes: $("f-min").value, draft: $("f-input").value,
    });
  };

  vscode.postMessage({ type: "ready" });
</script>
</body>
</html>`;
}

function activate(context) {
  ctx = context;

  // ステータスバー左下の入口ボタン（クリックで精進パネルを開く）
  const button = vscode.window.createStatusBarItem(vscode.StatusBarAlignment.Left, 100);
  button.text = "$(rocket) 精進";
  button.tooltip = "精進パネルを開く（問題選択・タイマー・記録）";
  button.command = "atcoderShojin.open";
  button.show();
  context.subscriptions.push(button);

  context.subscriptions.push(
    vscode.window.registerWebviewViewProvider(VIEW_ID, new ShojinViewProvider(), {
      webviewOptions: { retainContextWhenHidden: true },
    }),
    timer.on_change(post_state),
    // レビューで .session.json が消えた／記録で作られたら「レビュー待ち」表示を更新する
    (() => {
      const w = vscode.workspace.createFileSystemWatcher("**/honban/.session.json");
      w.onDidCreate(post_state);
      w.onDidChange(post_state);
      w.onDidDelete(post_state);
      return w;
    })(),
    // 推定レート: ノートが増えた・書き換わったら計算し直す
    (() => {
      const w = vscode.workspace.createFileSystemWatcher("**/problems/**/*.md");
      w.onDidCreate(refresh_rating);
      w.onDidChange(refresh_rating);
      w.onDidDelete(refresh_rating);
      return w;
    })(),
    // コードの推移: main.cpp の保存と、ビルド・実行などのタスク終了ごとにスナップショットを取る
    vscode.workspace.onDidSaveTextDocument((doc) => {
      const root = picker.find_root();
      if (root && code_history.same_path(doc.uri.fsPath, code_history.main_path(root))) {
        record_code("save", doc.getText());
      }
    }),
    vscode.tasks.onDidEndTaskProcess((e) => {
      const name = e.execution.task.name;
      if (code_history.TRACKED_TASKS.has(name)) record_code(name, undefined, e.exitCode);
    }),
    vscode.commands.registerCommand("atcoderShojin.open", () =>
      vscode.commands.executeCommand(`${VIEW_ID}.focus`)
    ),
    vscode.commands.registerCommand("atcoderPicker.pick", async () => {
      await vscode.commands.executeCommand(`${VIEW_ID}.focus`);
      await begin_random();
    })
  );
  refresh_rating();
}

module.exports = { activate };
