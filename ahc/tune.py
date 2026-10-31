#!/usr/bin/env python3
"""AHC072: Optuna で main.cpp の焼きなましパラメータを調整する

使い方:
    python ahc/tune.py                       # 既定: 200ケース × 50試行
    python ahc/tune.py --trials 100 --num 300
    python ahc/tune.py --study t0t1          # 別の study として最初から
    python ahc/tune.py --show                # 計測せず、保存済み study の結果だけ表示

仕組み:
  - main.cpp を -DTUNE つきでビルドする。TUNABLE(型, 名前, 既定値) で宣言した定数は、
    このビルドでだけ同名の環境変数で上書きされる(提出用の通常ビルドには影響しない)。
  - 1試行 = 探索空間 SPACE から値を選び、testcases/ の先頭 --num 件を bench.py と同じ
    CPU固定・同じ採点で回し、平均スコア(小さいほど良い)を目的関数にする。
  - ケースを塊ごとに回して途中の平均を報告し、明らかに悪い試行は打ち切る(MedianPruner)。
  - 最初の試行は main.cpp の現在の既定値(基準)。以降の試行はこれと比べられる。
  - study は ahc/tune_results/tune.db (SQLite) に保存され、同じ --study 名なら続きから再開する。
  - 探索空間を変えるときは SPACE を編集し、新しいパラメータは main.cpp 側を TUNABLE にする。

結果は ahc/tune_results/<study>_best.md に、main.cpp に貼れる形で書き出す。
main.cpp への反映は自動では行わない。
"""

import argparse
import functools
import math
import multiprocessing
import os
import re
import subprocess
import sys
import time
from concurrent.futures import ProcessPoolExecutor
from pathlib import Path

import optuna

import bench

HERE = Path(__file__).resolve().parent
OUT_DIR = HERE / "tune_results"

# ============================ 探索空間 ============================
# kind:
#   float / int … そのまま suggest する(log=True で対数スケール)
#   weight      … 同じ group の重みを正規化して確率にする。
#                 name が None の重みは「残り」(どの閾値にも使われない近傍1の分)。
SPACES = {
    # 焼きなましの温度と近傍の確率
    "anneal": [
    {"name": "ANNEAL_EXTRA", "kind": "int", "low": 0, "high": 3},
    {"name": "ANNEAL_T0", "kind": "float", "low": 0.1, "high": 10.0, "log": True},
    {"name": "ANNEAL_T1", "kind": "float", "low": 0.005, "high": 1.0, "log": True},
    {"name": "P_STEINER", "kind": "weight", "group": "nbr", "key": "w_steiner"},
    {"name": "P_SWAP", "kind": "weight", "group": "nbr", "key": "w_swap"},
    {"name": "P_MOVE", "kind": "weight", "group": "nbr", "key": "w_move"},
    {"name": None, "kind": "weight", "group": "nbr", "key": "w_random"},
    ],
    # ビームサーチの切り替えと時間配分
    "beam": [
        {"name": "BEAM_SLIME_MAX", "kind": "int", "low": 30, "high": 80},
        {"name": "BEAM_TL", "kind": "float", "low": 1200, "high": 1700},
        {"name": "TAIL_TOWERS", "kind": "int", "low": 20, "high": 80},
        {"name": "TAIL_TRIAL_TL", "kind": "float", "low": 700, "high": 1500},
    ],
}
SPACE = SPACES["anneal"]   # main() で --space に差し替える
W_LOW, W_HIGH = 0.01, 1.0   # 重みの範囲


def read_defaults(src):
    """main.cpp の TUNABLE(型, 名前, 既定値) から既定値を読む"""
    pat = re.compile(r"^\s*TUNABLE\(\s*\w+\s*,\s*(\w+)\s*,\s*([^)]+?)\s*\)", re.M)
    return {m.group(1): float(m.group(2)) for m in pat.finditer(src.read_text(encoding="utf-8"))}


def check_space(defaults):
    missing = [p["name"] for p in SPACE if p["name"] and p["name"] not in defaults]
    if missing:
        sys.exit(f"main.cpp で TUNABLE になっていないパラメータがあります: {missing}")


def suggest(trial):
    """trial から (環境変数に渡す値の dict) を作る"""
    params, groups = {}, {}
    for p in SPACE:
        if p["kind"] == "int":
            params[p["name"]] = trial.suggest_int(p["name"], p["low"], p["high"])
        elif p["kind"] == "float":
            params[p["name"]] = trial.suggest_float(p["name"], p["low"], p["high"],
                                                    log=p.get("log", False))
        elif p["kind"] == "weight":
            w = trial.suggest_float(p["key"], W_LOW, W_HIGH)
            groups.setdefault(p["group"], []).append((p["name"], w))
    for items in groups.values():
        total = sum(w for _, w in items)
        for name, w in items:
            if name:
                params[name] = w / total
    for k, v in params.items():
        trial.set_user_attr(k, v)
    return params


def default_trial_params(defaults):
    """main.cpp の既定値を再現する suggest 用の値(enqueue 用)"""
    out, groups = {}, {}
    for p in SPACE:
        if p["kind"] in ("int", "float"):
            v = defaults[p["name"]]
            out[p["name"]] = int(v) if p["kind"] == "int" else v
        else:
            groups.setdefault(p["group"], []).append(p)
    for items in groups.values():
        named = sum(defaults[p["name"]] for p in items if p["name"])
        rest = max(0.0, 1.0 - named)
        # 重み = 確率そのもの(合計1)。範囲 [W_LOW, W_HIGH] に収める
        for p in items:
            v = defaults[p["name"]] if p["name"] else rest
            out[p["key"]] = min(W_HIGH, max(W_LOW, v))
    return out


# ============================ 実行 ============================

def build_tune(src, binary, cxx):
    cmd = [cxx, "-std=gnu++20", "-O2", "-DTUNE", "-o", str(binary), str(src)]
    print(f"[build] {' '.join(cmd)}")
    r = subprocess.run(cmd, capture_output=True, text=True)
    if r.returncode != 0:
        print(r.stderr, file=sys.stderr)
        sys.exit("コンパイルに失敗しました")


def run_case_with(params, case, binary, cases_dir, timeout):
    """ワーカー内で環境変数をセットしてから bench.run_case を呼ぶ(子プロセスに継承される)"""
    for k, v in params.items():
        os.environ[k] = repr(v) if isinstance(v, float) else str(v)
    return bench.run_case(binary, case, cases_dir, timeout)


def load_ref(ref_dir):
    """基準の bench 結果(result.json)から、ケース番号 -> スコア"""
    import json
    data = json.loads((Path(ref_dir) / "result.json").read_text(encoding="utf-8"))
    return {r["idx"]: r["score"] for r in data["records"] if r["score"] is not None}


def make_objective(ex, binary, cases, cases_dir, args):
    ref = load_ref(args.ref) if args.ref else None

    def value(rec):
        """1ケースの値。--ref があれば基準との比 ×100(本番の相対評価に近い)、なければスコア"""
        return rec["score"] / ref[rec["idx"]] * 100 if ref else rec["score"]

    chunks = [cases[i:i + args.chunk] for i in range(0, len(cases), args.chunk)]

    def objective(trial):
        params = suggest(trial)
        shown = ", ".join(f"{k}={v:.4g}" for k, v in params.items())
        print(f"[trial {trial.number}] {shown}")
        t0 = time.perf_counter()
        scores, fails = [], 0
        for step, chunk in enumerate(chunks):
            task = functools.partial(run_case_with, params, binary=binary,
                                     cases_dir=cases_dir, timeout=args.timeout)
            for rec in ex.map(task, chunk):
                if rec["score"] is None:
                    fails += 1
                else:
                    scores.append(value(rec))
            if fails:
                raise RuntimeError(f"{fails} 件が失敗しました(TLE/RE/WA)")
            mean = sum(scores) / len(scores)
            trial.report(mean, step)
            if trial.should_prune():
                print(f"  -> 打ち切り ({len(scores)}件時点の平均 {mean:,.1f}, "
                      f"{time.perf_counter() - t0:.0f}s)")
                raise optuna.TrialPruned()
        mean = sum(scores) / len(scores)
        print(f"  -> 平均 {mean:,.1f} ({time.perf_counter() - t0:.0f}s)")
        return mean

    return objective


# ============================ 結果 ============================

def completed(study):
    return [t for t in study.trials if t.state == optuna.trial.TrialState.COMPLETE]


def write_best(study, defaults, path):
    done = completed(study)
    if not done:
        print("[best] 完了した試行がありません")
        return
    best = study.best_trial
    base = next((t for t in done if t.user_attrs.get("baseline")), None)
    lines = [f"# tune {study.study_name}", ""]
    lines.append(f"- 完了 {len(done)} 試行 / 全 {len(study.trials)} 試行, ケース数 {best.user_attrs.get('num')}")
    lines.append(f"- 最良: trial {best.number} 平均 {best.value:,.1f}")
    if base:
        diff = best.value - base.value
        lines.append(f"- 基準(main.cpp の既定値): trial {base.number} 平均 {base.value:,.1f} "
                     f"(差 {diff:+,.1f}, {diff / base.value * 100:+.2f}%)")
    lines += ["", "## 最良値(main.cpp に貼る形)", "", "```cpp"]
    for p in SPACE:
        if not p["name"]:
            continue
        v = best.user_attrs[p["name"]]
        typ = "ll" if p["kind"] == "int" else "double"
        val = str(int(v)) if p["kind"] == "int" else f"{v:.4g}"
        lines.append(f"TUNABLE({typ}, {p['name']}, {val});   // 既定 {defaults[p['name']]:g}")
    lines += ["```", "", "## 上位10試行", "",
              "| trial | 平均 | " + " | ".join(p["name"] for p in SPACE if p["name"]) + " |",
              "|---:|---:|" + "---:|" * sum(1 for p in SPACE if p["name"])]
    for t in sorted(done, key=lambda t: t.value)[:10]:
        vals = " | ".join(f"{t.user_attrs[p['name']]:.4g}" for p in SPACE if p["name"])
        mark = " (基準)" if t.user_attrs.get("baseline") else ""
        lines.append(f"| {t.number}{mark} | {t.value:,.1f} | {vals} |")
    try:
        imp = optuna.importance.get_param_importances(study)
        lines += ["", "## パラメータ重要度 (fANOVA)", "", "| パラメータ | 重要度 |", "|---|---:|"]
        lines += [f"| {k} | {v:.3f} |" for k, v in imp.items()]
    except Exception:
        pass   # 試行が少ないと計算できない
    text = "\n".join(lines) + "\n"
    path.write_text(text, encoding="utf-8")
    print()
    print(text)
    print(f"[out] {path}")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--trials", type=int, default=50, help="今回追加する試行数 (既定50)")
    ap.add_argument("--num", type=int, default=200, help="1試行で使うケース数 (既定200)")
    ap.add_argument("--chunk", type=int, default=0,
                    help="途中経過を報告する塊のケース数 (0=自動: num/4)")
    ap.add_argument("--study", default="anneal", help="study 名 (既定 anneal)")
    ap.add_argument("--seed", type=int, default=0, help="テストケース生成のマスターシード")
    ap.add_argument("--jobs", type=int, default=0, help="並列数 (0=自動。bench.py と同じ)")
    ap.add_argument("--no-pin", action="store_true", help="CPU固定をしない")
    ap.add_argument("--no-prune", action="store_true", help="悪い試行の打ち切りをしない")
    ap.add_argument("--timeout", type=float, default=5.0, help="1ケースの打ち切り秒数")
    ap.add_argument("--src", default=str(HERE / "main.cpp"))
    ap.add_argument("--cases-dir", default=str(HERE / "testcases"))
    ap.add_argument("--cxx", default=bench.default_cxx())
    ap.add_argument("--show", action="store_true", help="計測せず結果だけ表示")
    ap.add_argument("--space", default="", help=f"探索空間 {list(SPACES)} (既定: study 名と同じものがあればそれ、なければ anneal)")
    ap.add_argument("--ref", default="", help="基準の bench 結果のディレクトリ。与えると目的関数を基準とのスコア比の平均(×100)にする")
    args = ap.parse_args()
    global SPACE
    SPACE = SPACES[args.space or (args.study if args.study in SPACES else "anneal")]

    src = Path(args.src).resolve()
    cases_dir = Path(args.cases_dir).resolve()
    OUT_DIR.mkdir(parents=True, exist_ok=True)
    defaults = read_defaults(src)
    check_space(defaults)

    storage = f"sqlite:///{(OUT_DIR / 'tune.db').as_posix()}"
    pruner = optuna.pruners.NopPruner() if args.no_prune else \
        optuna.pruners.MedianPruner(n_startup_trials=5, n_warmup_steps=1)
    study = optuna.create_study(study_name=args.study, storage=storage, direction="minimize",
                                sampler=optuna.samplers.TPESampler(seed=0, multivariate=True),
                                pruner=pruner, load_if_exists=True)
    best_path = OUT_DIR / f"{args.study}_best.md"
    if args.show:
        write_best(study, defaults, best_path)
        return

    # 同じ study では同じケース数で比べる(混ぜると平均が比較できない)
    prev = {t.user_attrs.get("num") for t in study.trials} - {None}
    if prev and prev != {args.num}:
        sys.exit(f"study '{args.study}' はケース数 {sorted(prev)} で回しています。"
                 f"--num をそろえるか、--study で別名にしてください")

    cases = bench.prepare_cases(cases_dir, args.num, args.seed, False)
    args.chunk = args.chunk or max(1, math.ceil(args.num / 4))
    binary = OUT_DIR / ("tune.exe" if os.name == "nt" else "tune.out")
    build_tune(src, binary, args.cxx)

    solver_cpus, judge_cpus = (None, None) if args.no_pin else bench.plan_cpus()
    limit = len(solver_cpus) if solver_cpus else (os.cpu_count() or 1)
    args.jobs = min(args.jobs or limit, limit)
    if solver_cpus:
        solver_cpus = solver_cpus[:args.jobs]
    print(f"[tune] study={args.study} ケース {len(cases)} 件 × {args.trials} 試行, "
          f"jobs={args.jobs}, ソルバー固定CPU={solver_cpus}")

    if not study.trials:
        study.enqueue_trial(default_trial_params(defaults), user_attrs={"baseline": True})

    slot_queue = multiprocessing.Manager().Queue()
    for cpu in (solver_cpus or [None] * args.jobs):
        slot_queue.put(cpu)
    with ProcessPoolExecutor(max_workers=args.jobs, initializer=bench._init_worker,
                             initargs=(slot_queue, judge_cpus)) as ex:
        objective = make_objective(ex, binary, cases, cases_dir, args)

        def with_num(trial):
            trial.set_user_attr("num", args.num)
            return objective(trial)

        try:
            study.optimize(with_num, n_trials=args.trials, catch=(RuntimeError,))
        except KeyboardInterrupt:
            print("\n[tune] 中断しました(ここまでの試行は保存済み)")

    write_best(study, defaults, best_path)


if __name__ == "__main__":
    main()
