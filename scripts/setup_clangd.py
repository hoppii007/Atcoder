#!/usr/bin/env python3
"""
clangd 用の compile_flags.txt をこの PC 向けに生成する（PC ごとに 1 回実行すればよい）

使い方:
  python scripts/setup_clangd.py        (mac は python3)

compile_flags.txt はコンパイラの場所に依存するため git では追跡しない。
clangd（clang）は GCC の bits/stdc++.h を自前で持たないので、scripts/cxx.py が選ぶ g++ の
ヘッダを見に行くよう設定する。

- Windows: WinLibs の mingw64 を --sysroot に、--target=x86_64-w64-mingw32 を指定
- macOS:   Homebrew g++ の libstdc++ ヘッダ（.../include/c++/N 系）を -isystem で追加し、
           clang 自身の libc++ は -nostdinc++ で外す（C ヘッダは macOS SDK のものを使う）
"""

import subprocess
import sys
from pathlib import Path

from cxx import find_cxx

ROOT_DIR = Path(__file__).resolve().parent.parent
OUTPUT = ROOT_DIR / "compile_flags.txt"
BASE = ["-xc++", "-std=gnu++20"]


def gcc_include_dirs(cxx: str) -> list:
    """@brief g++ の #include <...> 探索パスを順に返す"""
    r = subprocess.run([cxx, "-xc++", "-E", "-v", "-"], input="", capture_output=True, text=True)
    dirs, inside = [], False
    for line in r.stderr.splitlines():
        if line.startswith("#include <...> search starts here:"):
            inside = True
        elif line.startswith("End of search list."):
            break
        elif inside:
            dirs.append(line.strip().removesuffix(" (framework directory)"))
    return dirs


def windows_flags(cxx: str) -> list:
    # g++.exe は <sysroot>/bin/g++.exe にある
    sysroot = Path(cxx).resolve().parent.parent.as_posix()
    return [*BASE, "--target=x86_64-w64-mingw32", f"--sysroot={sysroot}", "-I."]


def mac_flags(cxx: str) -> list:
    dirs = [d for d in gcc_include_dirs(cxx) if "/include/c++/" in d]
    if not dirs:
        sys.exit(f"{cxx} の libstdc++ ヘッダが見つかりません（brew install gcc 済みか確認）")
    return [*BASE, "-nostdinc++", *(f"-isystem{d}" for d in dirs), "-I."]


def main():
    cxx = find_cxx()
    if sys.platform == "win32":
        flags = windows_flags(cxx)
    elif sys.platform == "darwin":
        flags = mac_flags(cxx)
    else:
        flags = [*BASE, "-I."]
    OUTPUT.write_text("\n".join(flags) + "\n", encoding="utf-8", newline="\n")
    print(f"コンパイラ: {cxx}")
    print(f"出力: {OUTPUT}")
    print("\n".join(flags))
    print("\nVSCode で `clangd: Restart language server` を実行すると反映されます")


if __name__ == "__main__":
    main()
