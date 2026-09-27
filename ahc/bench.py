#!/usr/bin/env python3
"""AHC072: テストケース生成 + main.cpp のスコア計測

使い方:
    python3 ahc/bench.py                 # 50件のケースで計測(初回は生成)
    python3 ahc/bench.py --num 100       # 件数を変える
    python3 ahc/bench.py --regen         # テストケースを作り直す
    python3 ahc/bench.py --tag beam      # この実行にラベルを付ける
    python3 ahc/bench.py --jobs 4        # 並列実行(時間依存の解法はスコアがぶれる)

テストケースは固定シードで ahc/testcases/ に保存され、以降の実行では再利用される。
同じケースで測るので、コードを変えた前後のスコアを比較できる。

結果は ahc/bench_results/<日時>_<tag>/ に
  main.cpp   … そのタイミングのコードのスナップショット
  result.json… 各ケースの生データ
  summary.md … 統計とグリッド表示
として残り、ahc/bench_results/history.tsv に1行追記される。
"""

import argparse
import hashlib
import json
import math
import random
import shutil
import statistics
import subprocess
import sys
import time
from collections import deque
from concurrent.futures import ThreadPoolExecutor
from datetime import datetime
from pathlib import Path

HERE = Path(__file__).resolve().parent
PENALTY = 100000   # 残り1匹あたりのペナルティ
MAX_T = 100000     # 操作回数上限
MAX_H = 8          # 塔の高さ上限
DIRS = {"U": (-1, 0), "D": (1, 0), "L": (0, -1), "R": (0, 1)}

N_BUCKETS = [(12, 14), (15, 17), (18, 20)]
K_BUCKETS = [(4, 6), (7, 9), (10, 12)]


# ============================ テストケース生成 ============================
# 問題文の「入力生成方法」に従う(公式generatorとは乱数列が違うので同一の入力にはならない)

def _connected(wall, n, floor_cnt):
    start = None
    for i in range(n):
        for j in range(n):
            if not wall[i][j]:
                start = (i, j)
                break
        if start:
            break
    if start is None:
        return floor_cnt == 0
    seen = [[False] * n for _ in range(n)]
    seen[start[0]][start[1]] = True
    que = deque([start])
    cnt = 1
    while que:
        i, j = que.popleft()
        for di, dj in DIRS.values():
            ni, nj = i + di, j + dj
            if 0 <= ni < n and 0 <= nj < n and not wall[ni][nj] and not seen[ni][nj]:
                seen[ni][nj] = True
                cnt += 1
                que.append((ni, nj))
    return cnt == floor_cnt


def generate_case(seed):
    rng = random.Random(seed)
    n = rng.randint(12, 20)
    k = rng.randint(4, 12)
    rho = rng.uniform(0.12, 0.65)    # スライムの密度
    beta = rng.uniform(0.0, 0.40)    # 壁の提案率

    # 地形: 全マス床から始め、ランダム順に確率betaで壁化を試す
    wall = [[False] * n for _ in range(n)]
    cells = [(i, j) for i in range(n) for j in range(n)]
    rng.shuffle(cells)
    floor_cnt = n * n
    for i, j in cells:
        if rng.random() >= beta:
            continue
        if floor_cnt - 1 < 2 * k:
            continue
        wall[i][j] = True
        if _connected(wall, n, floor_cnt - 1):
            floor_cnt -= 1
        else:
            wall[i][j] = False

    floors = [(i, j) for i in range(n) for j in range(n) if not wall[i][j]]
    # 巣: 選んだ順に色0..K-1
    nests = rng.sample(floors, k)
    nest_set = set(nests)
    rest = [c for c in floors if c not in nest_set]
    F = len(rest)
    M = max(k, math.floor(rho * F + 0.5))   # スライム総数
    spots = rng.sample(rest, M)

    # 色の割り当て
    s = sorted(rng.randint(0, M - k) for _ in range(k - 1))
    s = [0] + s + [M - k]
    colors = []
    for c in range(k):
        colors += [c] * (s[c + 1] - s[c] + 1)
    assert len(colors) == M
    rng.shuffle(colors)

    grid = [["#" if wall[i][j] else "." for j in range(n)] for i in range(n)]
    for c, (i, j) in enumerate(nests):
        grid[i][j] = chr(ord("A") + c)
    for c, (i, j) in zip(colors, spots):
        grid[i][j] = chr(ord("a") + c)
    return n, k, M, ["".join(row) for row in grid]


def write_case(path, n, k, grid):
    path.write_text(f"{n} {k}\n" + "\n".join(grid) + "\n", encoding="utf-8")


def prepare_cases(cases_dir, num, master_seed, regen):
    cases_dir.mkdir(parents=True, exist_ok=True)
    meta_path = cases_dir / "meta.json"
    meta = {}
    if meta_path.exists() and not regen:
        meta = json.loads(meta_path.read_text(encoding="utf-8"))
    if regen or meta.get("master_seed") != master_seed or meta.get("num", 0) < num:
        meta = {"master_seed": master_seed, "num": num, "cases": []}
        for x in range(num):
            seed = master_seed * 1000000 + x
            n, k, M, grid = generate_case(seed)
            write_case(cases_dir / f"{x:04d}.txt", n, k, grid)
            meta["cases"].append({"idx": x, "seed": seed, "n": n, "k": k, "slime": M})
        meta_path.write_text(json.dumps(meta, indent=1), encoding="utf-8")
        print(f"[gen] {num} 件のテストケースを {cases_dir} に生成しました")
    else:
        print(f"[gen] {cases_dir} の既存テストケースを再利用します"
              f"(master_seed={master_seed}, {meta['num']}件)")
    return meta["cases"][:num]


# ============================ 採点 ============================

class Illegal(Exception):
    pass


def judge(n, k, grid, out_text):
    """出力を検証しつつシミュレートして (T, E) を返す。不正なら Illegal。"""
    towers = [[[] for _ in range(n)] for _ in range(n)]   # 下から順に色
    nest = [[-1] * n for _ in range(n)]
    is_floor = [[False] * n for _ in range(n)]
    remain = 0
    for i in range(n):
        for j in range(n):
            ch = grid[i][j]
            if ch == "#":
                continue
            is_floor[i][j] = True
            if ch == ".":
                continue
            if ch.islower():
                towers[i][j].append(ord(ch) - ord("a"))
                remain += 1
            else:
                nest[i][j] = ord(ch) - ord("A")

    def home(i, j):
        nonlocal remain
        c = nest[i][j]
        if c < 0:
            return
        t = towers[i][j]
        while t and t[-1] == c:
            t.pop()
            remain -= 1

    lines = [ln for ln in out_text.splitlines() if ln.strip()]
    if len(lines) > MAX_T:
        raise Illegal(f"操作回数が上限超過: {len(lines)} > {MAX_T}")

    for no, ln in enumerate(lines, 1):
        parts = ln.split()
        if len(parts) != 5:
            raise Illegal(f"{no}行目: 項目数が5でない: {ln!r}")
        try:
            i, j, kk = int(parts[0]), int(parts[1]), int(parts[2])
            l = int(parts[4])
        except ValueError:
            raise Illegal(f"{no}行目: 整数として読めない: {ln!r}")
        d = parts[3]
        if d not in DIRS:
            raise Illegal(f"{no}行目: 方向が UDLR でない: {d!r}")
        if not (0 <= i < n and 0 <= j < n) or not is_floor[i][j]:
            raise Illegal(f"{no}行目: 出発点が盤面内の床でない: ({i},{j})")
        h = len(towers[i][j])
        if h == 0:
            raise Illegal(f"{no}行目: 出発点にスライムがいない: ({i},{j})")
        if not (0 <= kk < h):
            raise Illegal(f"{no}行目: kが範囲外: k={kk}, h={h}")
        if not (1 <= l <= kk + 1):
            raise Illegal(f"{no}行目: lが範囲外: l={l}, k={kk}")
        di, dj = DIRS[d]
        for step in range(1, l + 1):
            ni, nj = i + di * step, j + dj * step
            if not (0 <= ni < n and 0 <= nj < n) or not is_floor[ni][nj]:
                raise Illegal(f"{no}行目: 経路 {step} マス目が盤面内の床でない")
        ti, tj = i + di * l, j + dj * l
        g = len(towers[ti][tj])
        if g + (h - kk) > MAX_H:
            raise Illegal(f"{no}行目: 着地後の高さが8超過: {g}+{h - kk}")
        # 上側 h-kk 匹が上下反転して着地点に乗る
        group = towers[i][j][kk:]
        del towers[i][j][kk:]
        towers[ti][tj].extend(reversed(group))
        home(i, j)
        home(ti, tj)

    return len(lines), remain


# ============================ 実行 ============================

def build(src, binary, cxx):
    cmd = [cxx, "-std=gnu++20", "-O2", "-o", str(binary), str(src)]
    print(f"[build] {' '.join(cmd)}")
    r = subprocess.run(cmd, capture_output=True, text=True)
    if r.returncode != 0:
        print(r.stderr, file=sys.stderr)
        sys.exit("コンパイルに失敗しました")


def run_case(binary, case, cases_dir, timeout):
    in_path = cases_dir / f"{case['idx']:04d}.txt"
    n, k = case["n"], case["k"]
    grid = in_path.read_text(encoding="utf-8").splitlines()[1:]
    rec = dict(case)
    with in_path.open("rb") as fin:
        t0 = time.perf_counter()
        try:
            proc = subprocess.run([str(binary)], stdin=fin,
                                  capture_output=True, timeout=timeout)
        except subprocess.TimeoutExpired:
            rec.update(status="TLE", ms=timeout * 1000, T=None, E=None, score=None)
            return rec
        ms = (time.perf_counter() - t0) * 1000
    if proc.returncode != 0:
        rec.update(status=f"RE({proc.returncode})", ms=ms, T=None, E=None, score=None)
        return rec
    try:
        T, E = judge(n, k, grid, proc.stdout.decode())
    except Illegal as e:
        rec.update(status="WA", ms=ms, T=None, E=None, score=None, message=str(e))
        return rec
    rec.update(status="OK", ms=ms, T=T, E=E, score=T + PENALTY * E)
    return rec


# ============================ 集計 ============================

def bucket_of(v, buckets):
    for idx, (lo, hi) in enumerate(buckets):
        if lo <= v <= hi:
            return idx
    return len(buckets) - 1


def ci(v):
    """整数を3桁区切りにする"""
    return "-" if v is None else f"{int(round(v)):,}"


def cf(v, digits=1):
    """小数を3桁区切りにする"""
    return "-" if v is None else f"{v:,.{digits}f}"


def grid_cells(records):
    """N×K の各バケツに入るスコアを集める"""
    cells = [[[] for _ in K_BUCKETS] for _ in N_BUCKETS]
    for r in records:
        if r["score"] is None:
            continue
        cells[bucket_of(r["n"], N_BUCKETS)][bucket_of(r["k"], K_BUCKETS)].append(r["score"])
    return cells


def _cell_text(v):
    return f"{ci(statistics.mean(v))} ({len(v)})" if v else "- (0)"


def format_grid_text(records):
    """端末表示用のおおまかなグリッド(平均スコア / 件数)"""
    cells = grid_cells(records)
    w = 18
    head = " " * 9 + "".join(f"| K:{lo}-{hi}".ljust(w) for lo, hi in K_BUCKETS)
    lines = [head, "-" * len(head)]
    for bi, (lo, hi) in enumerate(N_BUCKETS):
        row = f"N:{lo}-{hi}".ljust(9)
        for bj in range(len(K_BUCKETS)):
            row += ("| " + _cell_text(cells[bi][bj]).rjust(w - 3)).ljust(w)
        lines.append(row)
    return "\n".join(lines)


def format_grid_md(records):
    """summary.md 用のグリッド(Markdownの表にしてプレビューで格子になるようにする)"""
    cells = grid_cells(records)
    head = "| N \\ K | " + " | ".join(f"K:{lo}-{hi}" for lo, hi in K_BUCKETS) + " |"
    sep = "|---|" + "".join("---:|" for _ in K_BUCKETS)
    lines = [head, sep]
    for bi, (lo, hi) in enumerate(N_BUCKETS):
        lines.append(f"| **N:{lo}-{hi}** | "
                     + " | ".join(_cell_text(cells[bi][bj]) for bj in range(len(K_BUCKETS)))
                     + " |")
    return "\n".join(lines)


def summarize(records):
    ok = [r for r in records if r["score"] is not None]
    scores = [r["score"] for r in ok]
    bad = [r for r in records if r["score"] is None]
    st = {
        "cases": len(records),
        "ok": len(ok),
        "failed": len(bad),
        "sum": sum(scores) if scores else None,
        "mean": statistics.mean(scores) if scores else None,
        "median": statistics.median(scores) if scores else None,
        "min": min(scores) if scores else None,
        "max": max(scores) if scores else None,
        "stdev": statistics.pstdev(scores) if len(scores) > 1 else 0.0,
        "solved": sum(1 for r in ok if r["E"] == 0),
        "mean_ms": statistics.mean([r["ms"] for r in records]) if records else None,
        "max_ms": max([r["ms"] for r in records]) if records else None,
    }
    return st, ok, bad


def write_history_md(out_root):
    """各runの result.json を集めて、スコア履歴の md を組み立て直す"""
    rows = []
    for rj in sorted(out_root.glob("*/result.json")):
        try:
            d = json.loads(rj.read_text(encoding="utf-8"))
        except (OSError, json.JSONDecodeError):
            continue
        rows.append((rj.parent.name, d))
    if not rows:
        return None

    sums = [d["stats"]["sum"] for _, d in rows if d["stats"]["sum"] is not None]
    best = min(sums) if sums else None

    rows.reverse()   # 新しい順(プレビューで最新が一番上に来るように)

    md = ["# スコア履歴", "",
          "1行が1回の計測。**新しい順**に並べている。スコアは小さいほど良い。",
          "`★` は合計スコアが最小(=最良)の回。",
          "テストケースは seed とケース数が同じ回どうしでしか比較できない点に注意。", "",
          "| | 日時 | tag | sha1 | seed | ケース | 全員帰巣 | 合計 | 平均 | 中央値 | 最小 | 最大 | 最大時間 | 詳細 |",
          "|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|"]
    for name, d in rows:
        st = d["stats"]
        stamp = d.get("stamp", name[:15])
        when = (f"{stamp[0:4]}-{stamp[4:6]}-{stamp[6:8]} "
                f"{stamp[9:11]}:{stamp[11:13]}:{stamp[13:15]}") if len(stamp) >= 15 else stamp
        cases = ci(st["cases"]) + (f" (失敗 {ci(st['failed'])})" if st["failed"] else "")
        mark = "★" if st["sum"] is not None and st["sum"] == best else ""
        md.append(f"| {mark} | {when} | {d.get('tag','')} | `{d.get('sha1','')}` | "
                  f"{d.get('master_seed','')} | {cases} | "
                  f"{ci(st['solved'])} / {ci(st['ok'])} | {ci(st['sum'])} | {cf(st['mean'])} | "
                  f"{cf(st['median'])} | {ci(st['min'])} | {ci(st['max'])} | "
                  f"{ci(st['max_ms'])} ms | [summary]({name}/summary.md) |")
    md.append("")
    path = out_root / "history.md"
    path.write_text("\n".join(md) + "\n", encoding="utf-8")
    return path


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--num", type=int, default=50, help="テストケース数 (既定50)")
    ap.add_argument("--seed", type=int, default=0, help="生成のマスターシード (既定0)")
    ap.add_argument("--regen", action="store_true", help="テストケースを作り直す")
    ap.add_argument("--tag", default="", help="この実行のラベル")
    ap.add_argument("--jobs", type=int, default=1,
                    help="並列数 (既定1。増やすと速いが時間依存の解法はスコアがぶれる)")
    ap.add_argument("--timeout", type=float, default=5.0, help="1ケースの打ち切り秒数")
    ap.add_argument("--src", default=str(HERE / "main.cpp"))
    ap.add_argument("--cases-dir", default=str(HERE / "testcases"))
    ap.add_argument("--out-dir", default=str(HERE / "bench_results"))
    ap.add_argument("--cxx", default="/usr/local/bin/g++-12")
    ap.add_argument("--no-build", action="store_true", help="ビルドを省略して既存のバイナリを使う")
    ap.add_argument("--history-only", action="store_true",
                    help="計測はせず、既存のrunから履歴mdを作り直すだけ")
    args = ap.parse_args()

    if args.history_only:
        path = write_history_md(Path(args.out_dir).resolve())
        print(f"[out] {path}" if path else "[out] 対象のrunがありません")
        return

    src = Path(args.src).resolve()
    cases_dir = Path(args.cases_dir).resolve()
    out_root = Path(args.out_dir).resolve()
    stamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    run_dir = out_root / (f"{stamp}_{args.tag}" if args.tag else stamp)
    run_dir.mkdir(parents=True, exist_ok=True)

    cases = prepare_cases(cases_dir, args.num, args.seed, args.regen)

    binary = run_dir / "a.out"
    if args.no_build:
        binary = Path(str(src).replace(".cpp", ".out"))
    else:
        build(src, binary, args.cxx)

    # そのタイミングのコードを保存
    shutil.copy2(src, run_dir / src.name)
    code = src.read_bytes()
    sha = hashlib.sha1(code).hexdigest()[:12]

    print(f"[run] {len(cases)} ケース, jobs={args.jobs}")
    t0 = time.perf_counter()
    if args.jobs <= 1:
        records = [run_case(binary, c, cases_dir, args.timeout) for c in cases]
    else:
        with ThreadPoolExecutor(max_workers=args.jobs) as ex:
            records = list(ex.map(lambda c: run_case(binary, c, cases_dir, args.timeout), cases))
    wall = time.perf_counter() - t0

    st, ok, bad = summarize(records)
    grid_text = format_grid_text(records)
    grid_md = format_grid_md(records)

    # ---- 標準出力 ----
    print()
    print(f"code   : {src.name} sha1={sha} ({len(code)} bytes)")
    print(f"cases  : {st['cases']} (OK {st['ok']} / 失敗 {st['failed']}) 全員帰巣 {st['solved']}")
    if st["mean"] is not None:
        print(f"score  : 平均 {cf(st['mean'])}  中央値 {cf(st['median'])}  "
              f"最小 {ci(st['min'])}  最大 {ci(st['max'])}  標準偏差 {cf(st['stdev'])}")
        print(f"         合計 {ci(st['sum'])}")
    print(f"time   : 平均 {ci(st['mean_ms'])}ms  最大 {ci(st['max_ms'])}ms  全体 {cf(wall)}s")
    print()
    print("=== N×K グリッド (平均スコア / 件数) ===")
    print(grid_text)
    if bad:
        print()
        print("=== 失敗したケース ===")
        for r in bad:
            print(f"  idx={r['idx']:>4} seed={r['seed']} n={r['n']} k={r['k']} "
                  f"{r['status']} {r.get('message','')}")

    # ---- ファイル出力 ----
    (run_dir / "result.json").write_text(json.dumps({
        "stamp": stamp, "tag": args.tag, "src": str(src), "sha1": sha,
        "master_seed": args.seed, "num": args.num, "jobs": args.jobs,
        "wall_sec": wall, "stats": st, "records": records,
    }, indent=1, ensure_ascii=False), encoding="utf-8")

    md = [f"# bench {stamp} {args.tag}".rstrip(), ""]
    md += [f"- code: `{src.name}` sha1=`{sha}` ({ci(len(code))} bytes)",
           f"- cases: {ci(st['cases'])} (OK {ci(st['ok'])} / 失敗 {ci(st['failed'])}), "
           f"全員帰巣 {ci(st['solved'])}",
           f"- master_seed: {args.seed}, jobs: {args.jobs}, 実測 {cf(wall)}s", ""]
    if st["mean"] is not None:
        md += ["## スコア", "",
               "| 指標 | 値 |", "|---|---:|",
               f"| 合計 | {ci(st['sum'])} |",
               f"| 平均 | {cf(st['mean'])} |",
               f"| 中央値 | {cf(st['median'])} |",
               f"| 最小 | {ci(st['min'])} |",
               f"| 最大 | {ci(st['max'])} |",
               f"| 標準偏差 | {cf(st['stdev'])} |",
               f"| 平均実行時間 | {ci(st['mean_ms'])} ms |",
               f"| 最大実行時間 | {ci(st['max_ms'])} ms |", ""]
    md += ["## N×K グリッド (平均スコア / 件数)", "", grid_md, ""]
    md += ["## 各ケース", "",
           "| idx | seed | N | K | slime | T | E | score | ms | status |",
           "|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|"]
    for r in records:
        md.append(f"| {r['idx']} | {r['seed']} | {r['n']} | {r['k']} | {ci(r['slime'])} | "
                  f"{ci(r['T'])} | {ci(r['E'])} | {ci(r['score'])} | {ci(r['ms'])} | "
                  f"{r['status']} |")
    (run_dir / "summary.md").write_text("\n".join(md) + "\n", encoding="utf-8")

    hist = out_root / "history.tsv"
    if not hist.exists():
        hist.write_text("stamp\ttag\tsha1\tcases\tok\tsolved\tsum\tmean\tmedian\tmin\tmax\tmax_ms\n",
                        encoding="utf-8")
    with hist.open("a", encoding="utf-8") as f:
        f.write(f"{stamp}\t{args.tag}\t{sha}\t{st['cases']}\t{st['ok']}\t{st['solved']}\t"
                f"{st['sum']}\t{st['mean']}\t{st['median']}\t{st['min']}\t{st['max']}\t"
                f"{st['max_ms']:.0f}\n")

    hist_md = write_history_md(out_root)

    print()
    print(f"[out] {run_dir}")
    print(f"[out] {hist}")
    print(f"[out] {hist_md}")


if __name__ == "__main__":
    main()
