#!/usr/bin/env python3
"""problems/ のノート（solved と time_spent_min）から推定レートを出す。

AtCoder Problems（kenkoooo）の problem-models.json にある 2 つのモデルを尤度として使う。

1. 正答モデル（IRT）: レート θ の人がその問題を解く確率
       P(solve | θ) = 1 / (1 + exp(-discrimination * (θ - difficulty)))
   自力AC = 解けた、解説AC / 未AC = 解けなかった、として扱う（CLAUDE.md の solved 定義）。
2. 速度モデル: レート θ の人がコンテスト中にその問題を解くのにかかる時間（秒）
       ln(time) ~ Normal(slope * θ + intercept, variance)
   自力AC かつ time_spent_min が記録された回だけに使う。

両方の対数尤度と弱い事前分布 N(prior_mean, prior_sd^2) を足し、θ を格子上で評価して
事後分布の最頻値（MAP）と 80% 区間を出す。「正答のみ」「速度のみ」「総合」の 3 本を並べる。

注意:
- 各問題は最新回の結果を使う（--since を付けると、その日以降の回のうち最新）。
- time_spent_min が空の回は既定で使わない（精進パネルで計っていない回。まとめて登録した過去分など）。
  時間の無い自力AC は速度の補正がかからず推定を押し上げるため。--include-untimed で含める。
- START_DATE（2026-11-01）より前の回は既定で使わない。それ以前はまとめて登録した記録で、
  精進パネルで計った回と同じ精度とは言えないため。--start で変えられる（--start 0000 で全期間）。
- 精進パネルのタイマーは問題を読む時間から含む。コンテストでの解答時間（AtCoder Problems 側の定義）
  とは測り方が完全には一致しないので、速度側の推定は目安として読む。
- コンテスト本番は時間制限と他の問題との兼ね合いがあるため、「時間無制限で自力AC」は
  正答モデルの「解けた」より甘い。速度モデルを併用するのはその補正のため。

使い方:
    python3 scripts/estimate_rating.py             # 推定レートを表示
    python3 scripts/estimate_rating.py -v          # 問題ごとの内訳（期待正答率・予測時間）も表示
    python3 scripts/estimate_rating.py --since 2026-11-01   # 最近の回だけで推定
    python3 scripts/estimate_rating.py --json      # 機械可読（精進パネルが使う）
"""
import argparse
import gzip
import json
import math
import re
import sys
import urllib.request
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
PROBLEMS_DIR = ROOT / "problems"
MODELS_URL = "https://kenkoooo.com/atcoder/resources/problem-models.json"
# 精進パネル拡張（difficulty.js）がキャッシュしている場所。無ければ scripts/.cache に取得する
EXTENSION_CACHE = (
    Path.home() / "Library/Application Support/Code/User/globalStorage"
    / "local.atcoder-problem-picker/problem-models.json"
)
LOCAL_CACHE = ROOT / "scripts/.cache/problem-models.json"

START_DATE = "2026-11-01"  # これより前の回は使わない（既定）
THETA_LO, THETA_HI, THETA_STEP = -1000, 4000, 1


def load_models(explicit: str | None = None) -> dict:
    """problem-models.json を読む（指定パス → 拡張のキャッシュ → ローカルキャッシュ → 取得の順）。"""
    candidates = ([Path(explicit)] if explicit else []) + [EXTENSION_CACHE, LOCAL_CACHE]
    for path in candidates:
        if path.exists():
            data = json.loads(path.read_text(encoding="utf-8"))
            return data["models"] if "models" in data else data
    req = urllib.request.Request(MODELS_URL, headers={"Accept-Encoding": "gzip"})
    with urllib.request.urlopen(req, timeout=30) as res:
        raw = res.read()
        if res.headers.get("Content-Encoding") == "gzip":
            raw = gzip.decompress(raw)
    models = json.loads(raw)
    LOCAL_CACHE.parent.mkdir(parents=True, exist_ok=True)
    LOCAL_CACHE.write_text(json.dumps(models), encoding="utf-8")
    return models


def strip_comment(value: str) -> str:
    """'自力AC # ...' のような行末コメントを落とす。"""
    return value.split("#", 1)[0].strip()


def parse_note(path: Path):
    """ノートから (id, 挑戦回のリスト) を返す。挑戦回は {date, solved, time_min}。"""
    text = path.read_text(encoding="utf-8")
    m = re.search(r"^id:\s*(\S+)", text, re.M)
    if not m:
        return None, []
    task_id = m.group(1)
    attempts = []
    # "### 第N回 — YYYY-MM-DD HH:mm" ごとに区切る（時刻は省略可。時刻なしの旧い回は同じ日の先頭に並ぶ）
    heading = r"^### 第\d+回\s*[—-]\s*(\d{4}-\d{2}-\d{2}(?: \d{2}:\d{2})?)\s*$"
    n_heads = len(re.findall(r"^### 第\d+回", text, flags=re.M))
    n_ok = len(re.findall(heading, text, flags=re.M))
    if n_heads != n_ok:
        # 形式の崩れた見出しの回は推定から抜け落ちるので、黙らずに知らせる
        print(f"warning: {path}: 日時を読めない挑戦回の見出しが {n_heads - n_ok} 件あります"
              "（形式は '### 第N回 — YYYY-MM-DD HH:mm'）", file=sys.stderr)
    parts = re.split(heading, text, flags=re.M)
    for date, body in zip(parts[1::2], parts[2::2]):
        sm = re.search(r"^- solved:(.*)$", body, re.M)
        tm = re.search(r"^- time_spent_min:(.*)$", body, re.M)
        solved = strip_comment(sm.group(1)) if sm else ""
        time_str = strip_comment(tm.group(1)) if tm else ""
        try:
            time_min = float(time_str) if time_str else None
        except ValueError:
            time_min = None
        attempts.append({"date": date, "solved": solved, "time_min": time_min})
    return task_id, attempts


def usable(a: dict, include_untimed: bool, start: str) -> bool:
    """推定に使う回か（solved が定義どおりで、start 以降。既定では時間の記録があるもの）。"""
    return (a["solved"] in ("自力AC", "解説AC", "未AC") and a["date"] >= start
            and (include_untimed or a["time_min"] is not None))


def collect(since: str | None, include_untimed: bool = False, start: str = START_DATE) -> list[dict]:
    """各問題の最新回（since 以降に限る）を集める。"""
    rows = []
    for path in sorted(PROBLEMS_DIR.glob("*/*.md")):
        task_id, attempts = parse_note(path)
        if not task_id:
            continue
        attempts = [a for a in attempts if usable(a, include_untimed, start)]
        if since:
            attempts = [a for a in attempts if a["date"] >= since]
        if attempts:
            rows.append({"id": task_id, **attempts[-1]})
    return rows


def all_attempts(include_untimed: bool = False, start: str = START_DATE) -> list[dict]:
    """全ノートの全挑戦回を時系列（日時 → 問題 ID → 回の順）に並べる。"""
    out = []
    for path in sorted(PROBLEMS_DIR.glob("*/*.md")):
        task_id, attempts = parse_note(path)
        if not task_id:
            continue
        for no, a in enumerate(attempts, 1):
            if usable(a, include_untimed, start):
                out.append({"id": task_id, "no": no, **a})
    out.sort(key=lambda a: (a["date"], a["id"], a["no"]))
    return out


def attempt_loglik_grid(models: dict, a: dict) -> list[float]:
    """1回の挑戦の対数尤度（正答 + 速度）を THETAS 上で並べる。"""
    md = models[a["id"]]
    solved = a["solved"] == "自力AC"
    use_time = solved and a["time_min"] and has_time_model(md)
    return [loglik_solve(t, md, solved) + (loglik_time(t, md, a["time_min"]) if use_time else 0.0)
            for t in THETAS]


def history(models: dict, prior_mean: float, prior_sd: float, include_untimed: bool = False,
            start: str = START_DATE) -> list[dict]:
    """挑戦のたびの総合推定（その時点までの記録で、各問題は最新回を使う）。"""
    total = [0.0] * len(THETAS)
    current = {}  # 問題 ID → その問題の最新回の対数尤度の列
    points = []
    for a in all_attempts(include_untimed, start):
        md = models.get(a["id"])
        if not md or md.get("difficulty") is None:
            continue
        grid = attempt_loglik_grid(models, a)
        old = current.get(a["id"])
        if old is None:
            total = [x + y for x, y in zip(total, grid)]
        else:
            total = [x - o + y for x, o, y in zip(total, old, grid)]
        current[a["id"]] = grid
        m, lo, hi = posterior_from_grid(total, prior_mean, prior_sd)
        points.append({"date": a["date"], "id": a["id"], "no": a["no"], "solved": a["solved"],
                       "time_min": a["time_min"], "difficulty": to_display(md["difficulty"]),
                       "rating": to_display(m), "lo": to_display(lo), "hi": to_display(hi),
                       "n": len(current)})
    return points


def log_sigmoid(x: float) -> float:
    return -math.log1p(math.exp(-x)) if x > -30 else x


def loglik_solve(theta: float, model: dict, solved: bool) -> float:
    z = model["discrimination"] * (theta - model["difficulty"])
    return log_sigmoid(z) if solved else log_sigmoid(-z)


def loglik_time(theta: float, model: dict, time_min: float) -> float:
    mu = model["slope"] * theta + model["intercept"]
    var = model["variance"]
    x = math.log(time_min * 60)
    return -0.5 * (x - mu) ** 2 / var


def has_time_model(model: dict) -> bool:
    return all(model.get(k) is not None for k in ("slope", "intercept", "variance"))


THETAS = list(range(THETA_LO, THETA_HI + 1, THETA_STEP))


def posterior(loglik, prior_mean: float, prior_sd: float):
    """格子上で事後分布を作り、(MAP, 10%点, 90%点) を返す。loglik は θ→対数尤度の関数。"""
    return posterior_from_grid([loglik(t) for t in THETAS], prior_mean, prior_sd)


def posterior_from_grid(ll: list[float], prior_mean: float, prior_sd: float):
    """THETAS 上の対数尤度の列から (MAP, 10%点, 90%点) を返す。"""
    thetas = THETAS
    logp = [v - 0.5 * ((t - prior_mean) / prior_sd) ** 2 for t, v in zip(thetas, ll)]
    mx = max(logp)
    w = [math.exp(v - mx) for v in logp]
    total = sum(w)
    map_theta = thetas[logp.index(mx)]
    acc, lo, hi = 0.0, None, None
    for t, wi in zip(thetas, w):
        acc += wi / total
        if lo is None and acc >= 0.10:
            lo = t
        if hi is None and acc >= 0.90:
            hi = t
    return map_theta, lo, hi


def to_display(r: float) -> int:
    """AtCoder Problems と同じく 400 未満を丸めた表示値にする。"""
    return round(r if r >= 400 else 400 / math.exp((400 - r) / 400))


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("-v", "--verbose", action="store_true", help="問題ごとの内訳を表示")
    ap.add_argument("--since", help="この日付（YYYY-MM-DD）以降の回だけを使う")
    ap.add_argument("--prior-mean", type=float, default=1200, help="事前分布の平均（既定 1200）")
    ap.add_argument("--prior-sd", type=float, default=800, help="事前分布の標準偏差（既定 800、弱い事前）")
    ap.add_argument("--models", help="problem-models.json のパス（既定は精進パネル拡張のキャッシュ）")
    ap.add_argument("--json", action="store_true", help="結果を JSON で出力する")
    ap.add_argument("--start", default=START_DATE, help=f"この日付より前の回は使わない（既定 {START_DATE}）")
    ap.add_argument("--include-untimed", action="store_true", help="time_spent_min が空の回も使う")
    ap.add_argument("--history", action="store_true", help="挑戦ごとの総合推定の推移を出す（--json と併用）")
    args = ap.parse_args()

    models = load_models(args.models)
    if args.history:
        points = history(models, args.prior_mean, args.prior_sd, args.include_untimed, args.start)
        if args.json:
            print(json.dumps(points, ensure_ascii=False))
        else:
            for p in points:
                print(f"{p['date']} {p['id']:10s} {p['solved']:6s} -> {p['rating']:5d} [{p['lo']}, {p['hi']}]")
        return 0
    rows = collect(args.since, args.include_untimed, args.start)
    missing = [r["id"] for r in rows if r["id"] not in models or models[r["id"]].get("difficulty") is None]
    rows = [r for r in rows if r["id"] not in missing]
    if not rows:
        if args.json:
            print(json.dumps({"n": 0, "estimates": {}}, ensure_ascii=False))
            return 0
        print("対象の記録がありません。")
        return 1

    solve_rows = rows
    time_rows = [r for r in rows if r["solved"] == "自力AC" and r["time_min"] and has_time_model(models[r["id"]])]

    def ll_solve(t):
        return sum(loglik_solve(t, models[r["id"]], r["solved"] == "自力AC") for r in solve_rows)

    def ll_time(t):
        return sum(loglik_time(t, models[r["id"]], r["time_min"]) for r in time_rows)

    results = [
        ("正答のみ", ll_solve, len(solve_rows)),
        ("速度のみ", ll_time, len(time_rows)),
        ("総合", lambda t: ll_solve(t) + ll_time(t), len(solve_rows)),
    ]

    n_self = sum(r["solved"] == "自力AC" for r in rows)
    if args.json:
        keys = {"正答のみ": "solve", "速度のみ": "speed", "総合": "overall"}
        estimates = {}
        for name, ll, n in results:
            if n == 0:
                estimates[keys[name]] = None
                continue
            m, lo, hi = posterior(ll, args.prior_mean, args.prior_sd)
            estimates[keys[name]] = {"rating": to_display(m), "lo": to_display(lo), "hi": to_display(hi), "n": n}
        print(json.dumps({"n": len(rows), "n_self": n_self, "n_timed": len(time_rows), "since": args.since,
                          "excluded": missing, "estimates": estimates}, ensure_ascii=False))
        return 0
    print(f"対象: {len(rows)} 問（自力AC {n_self} / 解説AC・未AC {len(rows) - n_self}、"
          f"時間つき自力AC {len(time_rows)}）" + (f"  since {args.since}" if args.since else ""))
    if missing:
        print(f"  difficulty モデルが無く除外: {', '.join(missing)}")
    print(f"事前分布: N({args.prior_mean:.0f}, {args.prior_sd:.0f}^2)\n")
    print(f"{'':8s}{'推定':>6s}   80%区間")
    for name, ll, n in results:
        if n == 0:
            print(f"{name:8s}{'—':>6s}   データなし")
            continue
        m, lo, hi = posterior(ll, args.prior_mean, args.prior_sd)
        print(f"{name:8s}{to_display(m):6d}   [{to_display(lo)}, {to_display(hi)}]")

    if args.verbose:
        theta, _, _ = posterior(results[2][1], args.prior_mean, args.prior_sd)
        print(f"\n内訳（総合推定 θ={to_display(theta)} での期待値）")
        print(f"{'id':10s} {'diff':>5s} {'結果':6s} {'P(解)':>6s} {'時間':>5s} {'予測':>5s}")
        for r in sorted(rows, key=lambda r: models[r["id"]]["difficulty"]):
            md = models[r["id"]]
            p = math.exp(loglik_solve(theta, md, True))
            pred = (f"{math.exp(md['slope'] * theta + md['intercept']) / 60:5.0f}"
                    if has_time_model(md) else "    —")
            t = f"{r['time_min']:5.0f}" if r["time_min"] else "    —"
            print(f"{r['id']:10s} {to_display(md['difficulty']):5d} {r['solved']:6s} {p:6.2f} {t} {pred}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
