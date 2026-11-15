#!/usr/bin/env python3
"""
#include "library/..." を展開して提出用の単一ファイルを作るスクリプト

使い方:
  python scripts/expand.py [入力=honban/main.cpp] [-o 出力=<入力と同じフォルダ>/submit.cpp]
                           [--clip] [--check] [--strip-doc]

  --clip       展開結果をクリップボードへコピー（Windows: Set-Clipboard / mac: pbcopy）
  --check      展開結果を g++ でコンパイルできるか確認（ライブラリ側で未定義が無いか等）
  --strip-doc  展開したライブラリから Doxygen ブロックコメント（/** ... */）を除去

main.cpp 側の書き方:
  テンプレート（ll, vll, mod などの定義）の後で
      #include "library/tree/union_find.cpp"
  のように書く。ビルド時は g++ に -I <リポジトリ直下> を付ければそのままコンパイルできる。

展開ルール:
  - #include "..." は (1) 書かれたファイルのフォルダ (2) リポジトリ直下 (3) library/ の順で解決し、
    見つかったものを再帰的に展開する（同じファイルは 1 回だけ）。
  - 見つからない #include "..." は警告を出してそのまま残す。
  - #include <...> は既に出力済みなら削除（ライブラリ先頭の #include<bits/stdc++.h> の重複除去）。
  - ライブラリ内の #pragma once は削除。
"""

import argparse
import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

from cxx import find_cxx

ROOT_DIR = Path(__file__).resolve().parent.parent
LIBRARY_DIR = ROOT_DIR / "library"

LOCAL_INCLUDE = re.compile(r'^\s*#\s*include\s*"([^"]+)"\s*;?\s*(//.*)?$')
SYSTEM_INCLUDE = re.compile(r"^\s*#\s*include\s*<([^>]+)>")
PRAGMA_ONCE = re.compile(r"^\s*#\s*pragma\s+once\b")
DOC_COMMENT = re.compile(r"/\*\*.*?\*/[ \t]*\n?", re.S)


def rel(p: Path) -> str:
    try:
        return p.relative_to(ROOT_DIR).as_posix()
    except ValueError:
        return str(p)


class Expander:
    def __init__(self, strip_doc: bool):
        self.strip_doc = strip_doc
        self.expanded = set()  # 展開済みファイル（resolve 済みパス）
        self.system_includes = set()
        self.warnings = []

    def resolve(self, name: str, base_dir: Path):
        for d in (base_dir, ROOT_DIR, LIBRARY_DIR):
            p = (d / name).resolve()
            if p.is_file():
                return p
        return None

    def expand_file(self, path: Path, is_root: bool) -> list:
        text = path.read_text(encoding="utf-8")
        if not is_root and self.strip_doc:
            text = DOC_COMMENT.sub("", text)
        out = []
        for lineno, line in enumerate(text.splitlines(), 1):
            if (m := LOCAL_INCLUDE.match(line)):
                target = self.resolve(m.group(1), path.parent)
                if target is None:
                    self.warnings.append(f"{rel(path)}:{lineno}: 見つからない include \"{m.group(1)}\" をそのまま残しました")
                    out.append(line)
                    continue
                if target in self.expanded:
                    continue
                self.expanded.add(target)
                out.append(f"// ===== begin {rel(target)} =====")
                out.extend(self.expand_file(target, is_root=False))
                out.append(f"// ===== end {rel(target)} =====")
                continue
            if (m := SYSTEM_INCLUDE.match(line)):
                header = m.group(1).strip()
                if header in self.system_includes:
                    continue
                self.system_includes.add(header)
            if not is_root and PRAGMA_ONCE.match(line):
                continue
            out.append(line)
        return out


def copy_to_clipboard(path: Path) -> bool:
    if sys.platform == "win32":
        cmd = ["powershell", "-NoProfile", "-Command",
               f"Get-Content -Raw -Encoding UTF8 -LiteralPath '{path}' | Set-Clipboard"]
        return subprocess.run(cmd).returncode == 0
    if shutil.which("pbcopy"):
        return subprocess.run(["pbcopy"], input=path.read_bytes()).returncode == 0
    if shutil.which("xclip"):
        return subprocess.run(["xclip", "-selection", "clipboard"], input=path.read_bytes()).returncode == 0
    return False


def compile_check(path: Path) -> bool:
    # -I は付けない（展開漏れがあればここで落ちるように）。別フォルダから実行して相対 include も効かなくする
    with tempfile.TemporaryDirectory() as tmp:
        r = subprocess.run([find_cxx(), "-std=gnu++20", "-fsyntax-only", str(path)], cwd=tmp)
        return r.returncode == 0


def main():
    ap = argparse.ArgumentParser(description="#include \"library/...\" を展開して提出用コードを作る")
    ap.add_argument("input", nargs="?", default=str(ROOT_DIR / "honban" / "main.cpp"))
    ap.add_argument("-o", "--output")
    ap.add_argument("--clip", action="store_true", help="結果をクリップボードへコピー")
    ap.add_argument("--check", action="store_true", help="展開結果が g++ でコンパイルできるか確認")
    ap.add_argument("--strip-doc", action="store_true", help="ライブラリの /** ... */ を除去")
    args = ap.parse_args()

    src = Path(args.input).resolve()
    dst = Path(args.output).resolve() if args.output else src.parent / "submit.cpp"

    ex = Expander(strip_doc=args.strip_doc)
    ex.expanded.add(src)
    lines = ex.expand_file(src, is_root=True)
    dst.write_text("\n".join(lines) + "\n", encoding="utf-8", newline="\n")

    for w in ex.warnings:
        print(f"warning: {w}", file=sys.stderr)
    libs = sorted(rel(p) for p in ex.expanded if p != src)
    print(f"展開: {len(libs)} ファイル {libs}" if libs else "展開: なし")
    print(f"出力: {dst}")

    ok = True
    if args.check:
        ok = compile_check(dst)
        print("コンパイル確認: OK" if ok else "コンパイル確認: NG")
    if args.clip:
        print("クリップボードにコピーしました" if copy_to_clipboard(dst) else "クリップボードへのコピーに失敗しました")
    sys.exit(0 if ok else 1)


if __name__ == "__main__":
    main()
