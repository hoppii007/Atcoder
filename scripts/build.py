#!/usr/bin/env python3
"""
template.hpp のプリコンパイル済みヘッダ（PCH）を必要なときだけ作り直してから、main.cpp をビルドする

使い方:
  python scripts/build.py <ソース> <出力 exe> [--cxx <コンパイラ>]

- コンパイラは --cxx 省略時 scripts/cxx.py が決める（CXX 環境変数 → WinLibs の 64bit g++ → PATH の g++）。

- template.hpp.gch が無い・template.hpp より古い・別のコンパイラで作られたときだけ PCH を再生成する（約 3 秒）。
- -static でリンクする（PATH 上の別の MinGW の DLL を読んで起動に失敗するのを防ぐ）。
- PCH はコンパイルオプションが一致しないと使われないため、PCH 生成とビルドは同じ CXXFLAGS で行う。
  オプションを変えるときは CXXFLAGS だけを書き換えること。
"""

import argparse
import subprocess
import sys
from pathlib import Path

from cxx import find_cxx

ROOT_DIR = Path(__file__).resolve().parent.parent
TEMPLATE = ROOT_DIR / "template.hpp"
PCH = ROOT_DIR / "template.hpp.gch"
PCH_STAMP = ROOT_DIR / "template.hpp.gch.cxx"  # PCH を作ったコンパイラのパス
CXXFLAGS = ["-std=gnu++20"]


def ensure_pch(cxx: str) -> bool:
    """@brief PCH が古ければ再生成する。@return 成功したか"""
    stamp = PCH_STAMP.read_text(encoding="utf-8") if PCH_STAMP.exists() else ""
    if PCH.exists() and PCH.stat().st_mtime >= TEMPLATE.stat().st_mtime and stamp == cxx:
        return True
    print("template.hpp の PCH を生成中...", flush=True)
    r = subprocess.run([cxx, *CXXFLAGS, "-x", "c++-header", str(TEMPLATE), "-o", str(PCH)])
    if r.returncode != 0:
        return False
    PCH_STAMP.write_text(cxx, encoding="utf-8")
    return True


def main():
    ap = argparse.ArgumentParser(description="PCH を使って main.cpp をビルドする")
    ap.add_argument("source")
    ap.add_argument("output")
    ap.add_argument("--cxx", default=None)
    args = ap.parse_args()

    cxx = args.cxx or find_cxx()
    if not ensure_pch(cxx):
        sys.exit(1)
    # -Winvalid-pch: PCH が使われなかったときに警告を出す（オプション不一致などに気づけるように）
    r = subprocess.run([cxx, *CXXFLAGS, "-Winvalid-pch", "-I", str(ROOT_DIR), args.source, "-o", args.output, "-static"])
    sys.exit(r.returncode)


if __name__ == "__main__":
    main()
