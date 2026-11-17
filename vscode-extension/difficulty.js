// AtCoder Problems（kenkoooo）の problem-models.json から difficulty を引く。
// ファイルが大きいので globalStorage にキャッシュし、無い問題があったときだけ取り直す。
const https = require("https");
const zlib = require("zlib");
const fs = require("fs");
const path = require("path");

const MODELS_URL = "https://kenkoooo.com/atcoder/resources/problem-models.json";
const REFETCH_AFTER_MS = 60 * 60 * 1000; // キャッシュに無い問題は、1時間以上経っていれば取り直す

let memo = null; // { fetched_at, models }

function fetch_json(url) {
  return new Promise((resolve, reject) => {
    const req = https.get(url, { headers: { "Accept-Encoding": "gzip" } }, (res) => {
      if (res.statusCode !== 200) {
        res.resume();
        reject(new Error(`HTTP ${res.statusCode}`));
        return;
      }
      const stream = res.headers["content-encoding"] === "gzip" ? res.pipe(zlib.createGunzip()) : res;
      const chunks = [];
      stream.on("data", (c) => chunks.push(c));
      stream.on("end", () => {
        try {
          resolve(JSON.parse(Buffer.concat(chunks).toString("utf8")));
        } catch (e) {
          reject(e);
        }
      });
      stream.on("error", reject);
    });
    req.on("error", reject);
    req.setTimeout(20000, () => req.destroy(new Error("timeout")));
  });
}

/**
 * @brief AtCoder Problems の表示と同じ補正をかける
 * @note 400 未満は 400 / exp((400 - d) / 400) に丸める（サイト表示と一致させる）
 */
function clip(d) {
  return Math.round(d >= 400 ? d : 400 / Math.exp((400 - d) / 400));
}

function cache_file(storage_dir) {
  return path.join(storage_dir, "problem-models.json");
}

function load_cache(storage_dir) {
  if (memo) return memo;
  try {
    memo = JSON.parse(fs.readFileSync(cache_file(storage_dir), "utf8"));
  } catch (e) {
    memo = null;
  }
  return memo;
}

async function refresh(storage_dir) {
  const models = await fetch_json(MODELS_URL);
  memo = { fetched_at: Date.now(), models };
  try {
    fs.mkdirSync(storage_dir, { recursive: true });
    fs.writeFileSync(cache_file(storage_dir), JSON.stringify(memo));
  } catch (e) {
    // キャッシュに書けなくても値は返せる
  }
  return memo;
}

/**
 * @brief 問題の difficulty を返す
 * @param storage_dir キャッシュを置くディレクトリ
 * @param task_id 例 "abc318_f"
 * @return { difficulty: number|null, experimental: bool }。取得に失敗したら例外
 */
async function get_difficulty(storage_dir, task_id) {
  let data = load_cache(storage_dir);
  const stale = !data || Date.now() - data.fetched_at > REFETCH_AFTER_MS;
  if (!data || (!(task_id in data.models) && stale)) data = await refresh(storage_dir);
  const model = data.models[task_id];
  if (!model || typeof model.difficulty !== "number") return { difficulty: null, experimental: false };
  return { difficulty: clip(model.difficulty), experimental: !!model.is_experimental };
}

module.exports = { get_difficulty };
