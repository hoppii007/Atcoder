#!/usr/bin/env python3
"""
AtCoder Problems API から提出履歴を取得し、ABC ごとに
「各問題を通した時刻（コンテスト開始からの分）」と「ペナルティ数」を集計する。

usage:
    python abc_analyze.py hoppii            # 直近20回
    python abc_analyze.py hoppii --n 40     # 直近40回
    python abc_analyze.py hoppii --csv out.csv
"""

import argparse
import csv
import json
import re
import sys
import time
import urllib.request

BASE = "https://kenkoooo.com/atcoder"
UA = {"User-Agent": "abc-analyze/1.0"}
ABC_RE = re.compile(r"^abc(\d+)$")
LETTERS = "abcdefg"


def fetch_json(url):
    req = urllib.request.Request(url, headers=UA)
    with urllib.request.urlopen(req) as res:
        return json.load(res)


def fetch_contests():
    """contest_id -> (start_epoch, duration_sec)"""
    data = fetch_json(f"{BASE}/resources/contests.json")
    return {c["id"]: (c["start_epoch_second"], c["duration_second"]) for c in data}


def fetch_submissions(user):
    """500件ずつページングして全提出を取得（1秒スリープ）"""
    out, cursor = [], 0
    while True:
        url = f"{BASE}/atcoder-api/v3/user/submissions?user={user}&from_second={cursor}"
        batch = fetch_json(url)
        if not batch:
            break
        out.extend(batch)
        newest = max(s["epoch_second"] for s in batch)
        if newest <= cursor or len(batch) < 500:
            out_cursor = newest
            if len(batch) < 500:
                break
        cursor = newest + 1
        print(f"  ... {len(out)} 件取得", file=sys.stderr)
        time.sleep(1)
    # 重複排除
    return list({s["id"]: s for s in out}.values())


def analyze(user, n):
    print("コンテスト情報を取得中...", file=sys.stderr)
    contests = fetch_contests()
    print(f"提出履歴を取得中 ({user})...", file=sys.stderr)
    subs = fetch_submissions(user)

    # ABC のコンテスト時間内の提出のみ残す
    per_contest = {}
    for s in subs:
        cid = s["contest_id"]
        if not ABC_RE.match(cid) or cid not in contests:
            continue
        start, dur = contests[cid]
        if not (start <= s["epoch_second"] <= start + dur):
            continue
        per_contest.setdefault(cid, []).append((s, start))

    if not per_contest:
        print("該当する ABC 本番中の提出が見つかりませんでした。", file=sys.stderr)
        return []

    rows = []
    for cid in sorted(per_contest, key=lambda c: contests[c][0], reverse=True)[:n]:
        entries = per_contest[cid]
        start = contests[cid][0]
        row = {"contest": cid}
        for L in LETTERS:
            pid = f"{cid}_{L}"
            mine = sorted(
                (s for s, _ in entries if s["problem_id"] == pid),
                key=lambda s: s["epoch_second"],
            )
            if not mine:
                row[L.upper()] = ""
                row[L.upper() + "_pen"] = ""
                continue
            ac = next((s for s in mine if s["result"] == "AC"), None)
            if ac:
                row[L.upper()] = round((ac["epoch_second"] - start) / 60)
                row[L.upper() + "_pen"] = sum(
                    1 for s in mine
                    if s["epoch_second"] < ac["epoch_second"] and s["result"] != "AC"
                )
            else:
                row[L.upper()] = "-"  # 提出したが未AC
                row[L.upper() + "_pen"] = len(mine)
        rows.append(row)
    return rows


def summarize(rows):
    def vals(key):
        return [r[key] for r in rows if isinstance(r[key], int)]

    print()
    print("=== ABC ごとの通過時刻（分） / () 内はAC前のペナルティ数 ===")
    print("空欄=未提出, -=提出したが未AC")
    header = "contest  " + "".join(f"{L.upper():>10}" for L in LETTERS)
    print(header)
    for r in rows:
        line = f"{r['contest']:<9}"
        for L in LETTERS:
            k = L.upper()
            v = r[k]
            p = r[k + "_pen"]
            cell = "" if v == "" else (f"{v}({p})" if p else f"{v}")
            line += f"{cell:>10}"
        print(line)

    print()
    print("=== サマリ ===")
    total = len(rows)
    for L in LETTERS:
        k = L.upper()
        solved = vals(k)
        attempted = [r for r in rows if r[k] != ""]
        if not attempted:
            continue
        rate = len(solved) / total * 100
        avg = sum(solved) / len(solved) if solved else None
        med = sorted(solved)[len(solved) // 2] if solved else None
        avg_s = f"平均 {avg:5.1f}分 / 中央 {med:3}分" if solved else "　"
        print(f"{k}: AC {len(solved):2}/{total} ({rate:5.1f}%)   {avg_s}")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("user")
    ap.add_argument("--n", type=int, default=20, help="直近何回分を見るか")
    ap.add_argument("--csv", help="CSV 出力先")
    args = ap.parse_args()

    rows = analyze(args.user, args.n)
    if not rows:
        return
    summarize(rows)

    if args.csv:
        keys = ["contest"] + [
            f"{L.upper()}{suf}" for L in LETTERS for suf in ("", "_pen")
        ]
        with open(args.csv, "w", newline="", encoding="utf-8") as f:
            w = csv.DictWriter(f, fieldnames=keys)
            w.writeheader()
            w.writerows(rows)
        print(f"\nCSV: {args.csv}")


if __name__ == "__main__":
    main()