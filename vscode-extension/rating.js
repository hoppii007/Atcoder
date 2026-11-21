// 推定レート — scripts/estimate_rating.py --json を呼んで、精進パネルに出す値を作る。
// 推定のロジックは Python 側だけに置く（ここでは呼び出しと結果のキャッシュだけ）。
const { execFile } = require("child_process");
const path = require("path");
const difficulty = require("./difficulty");

const RECENT_DAYS = 30; // 「直近」として別に出す期間

let memo = null;      // { all, recent, history, error, computed_at }
let running = null;   // 計算中の Promise（多重起動を防ぐ）
let dirty = false;    // 計算中にノートが変わったら、終わった後にもう一度計算する

function python_cmd() {
  return process.platform === "win32" ? "python" : "python3";
}

function days_ago_iso(days) {
  const d = new Date(Date.now() - days * 24 * 60 * 60 * 1000);
  const m = String(d.getMonth() + 1).padStart(2, "0");
  const day = String(d.getDate()).padStart(2, "0");
  return `${d.getFullYear()}-${m}-${day}`;
}

/** estimate_rating.py を1回実行して JSON を返す。 */
function run_script(root, storage_dir, extra_args) {
  const script = path.join(root, "scripts", "estimate_rating.py");
  const args = [script, "--json", "--models", difficulty.cache_path(storage_dir), ...extra_args];
  return new Promise((resolve, reject) => {
    execFile(python_cmd(), args, { cwd: root, timeout: 60000, maxBuffer: 1 << 20 }, (err, stdout, stderr) => {
      if (err) {
        reject(new Error((stderr || err.message).trim().split("\n").pop()));
        return;
      }
      try {
        resolve(JSON.parse(stdout));
      } catch (e) {
        reject(e);
      }
    });
  });
}

/**
 * @brief 全期間・直近 RECENT_DAYS 日の推定レートと、挑戦ごとの推移を計算し直す
 * @param root ワークスペースのルート（problems/ を持つ）
 * @param storage_dir 拡張の globalStorage（difficulty モデルのキャッシュがある）
 * @param on_done 計算が終わったら呼ぶ（画面の更新用）
 */
function refresh(root, storage_dir, on_done) {
  if (!root) return;
  if (running) {
    dirty = true;
    return;
  }
  running = Promise.all([
    run_script(root, storage_dir, []),
    run_script(root, storage_dir, ["--since", days_ago_iso(RECENT_DAYS)]),
    run_script(root, storage_dir, ["--history"]),
  ])
    .then(([all, recent, history]) => {
      memo = { all, recent, history, recent_days: RECENT_DAYS, error: null, computed_at: Date.now() };
    })
    .catch((e) => {
      memo = { ...(memo || {}), error: e.message };
    })
    .finally(() => {
      running = null;
      on_done();
      if (dirty) {
        dirty = false;
        refresh(root, storage_dir, on_done);
      }
    });
}

/** 画面に渡す現在の値（未計算なら null）。 */
function snapshot() {
  return memo;
}

module.exports = { refresh, snapshot };
