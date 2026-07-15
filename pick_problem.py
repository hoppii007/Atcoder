#!/usr/bin/env python3
"""まだ problems/ に記録のない問題からランダムに1問ピックする。

対象: ABC212〜ABC318 の C〜E 問題（デフォルト）。
「problems/abcXXX/abcXXX_y.md が存在しない」= 未挑戦とみなす。
CLAUDE.md の方針により A・B はそもそもノート化しないため既定では除外する。

使い方:
    python3 pick_problem.py              # C〜E からランダムに1問
    python3 pick_problem.py -n 3         # 3問まとめてピック
    python3 pick_problem.py -l cde       # 対象レベルを指定（例: c,d,e）
    python3 pick_problem.py --list       # 未挑戦の候補を全件表示（ピックしない）
    python3 pick_problem.py --from 212 --to 318
"""
import argparse
import random
import sys
from pathlib import Path

# このスクリプトのある場所を基準にする（どこから呼んでも動く）
ROOT = Path(__file__).resolve().parent
PROBLEMS_DIR = ROOT / "problems"

DEFAULT_FROM = 212
DEFAULT_TO = 318
DEFAULT_LEVELS = "cde"


def note_path(contest: int, level: str) -> Path:
    """その問題のノート md のパスを返す（存在するとは限らない）。"""
    return PROBLEMS_DIR / f"abc{contest}" / f"abc{contest}_{level}.md"


def build_candidates(lo: int, hi: int, levels: str) -> list[tuple[int, str]]:
    """記録の無い (contest, level) を列挙する。"""
    candidates = []
    for contest in range(lo, hi + 1):
        for level in levels:
            if not note_path(contest, level).exists():
                candidates.append((contest, level))
    return candidates


def url_of(contest: int, level: str) -> str:
    return f"https://atcoder.jp/contests/abc{contest}/tasks/abc{contest}_{level}"


def fmt(contest: int, level: str) -> str:
    return f"ABC{contest} {level.upper()}  {url_of(contest, level)}"


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__,
                                      formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("-n", "--num", type=int, default=1,
                        help="ピックする問題数（デフォルト1）")
    parser.add_argument("-l", "--levels", default=DEFAULT_LEVELS,
                        help=f"対象レベルの文字列（デフォルト {DEFAULT_LEVELS}）")
    parser.add_argument("--from", dest="lo", type=int, default=DEFAULT_FROM,
                        help=f"開始コンテスト番号（デフォルト {DEFAULT_FROM}）")
    parser.add_argument("--to", dest="hi", type=int, default=DEFAULT_TO,
                        help=f"終了コンテスト番号（デフォルト {DEFAULT_TO}）")
    parser.add_argument("--list", action="store_true",
                        help="未挑戦の候補を全件表示（ランダムピックしない）")
    parser.add_argument("--seed", type=int, default=None,
                        help="乱数シード（再現したいとき用）")
    args = parser.parse_args()

    levels = args.levels.lower()
    candidates = build_candidates(args.lo, args.hi, levels)
    total = (args.hi - args.lo + 1) * len(levels)
    done = total - len(candidates)

    print(f"# 対象: ABC{args.lo}〜ABC{args.hi} の {'/'.join(c.upper() for c in levels)} 問題")
    print(f"# 記録済み {done} / 全 {total}  → 未挑戦の候補 {len(candidates)} 問\n")

    if not candidates:
        print("すべて記録済みです。おめでとうございます。")
        return 0

    if args.list:
        for contest, level in candidates:
            print(fmt(contest, level))
        return 0

    if args.seed is not None:
        random.seed(args.seed)

    n = min(args.num, len(candidates))
    picked = random.sample(candidates, n)
    picked.sort()
    for contest, level in picked:
        print(fmt(contest, level))
    return 0


if __name__ == "__main__":
    sys.exit(main())
