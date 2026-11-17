"""
ビルドに使う C++ コンパイラを決める

優先順:
  1. 環境変数 CXX
  2. Windows: winget で入れた WinLibs（64bit g++。__int128 が使える）
     macOS:   Homebrew の GNU g++（g++-15 など。版番号の大きいものを優先）
              ※ mac の /usr/bin/g++ は実体が Apple clang で bits/stdc++.h が無いため使わない
  3. PATH 上の g++
"""

import os
import re
import shutil
import sys
from pathlib import Path


def _find_winlibs():
    packages = Path(os.environ.get("LOCALAPPDATA", "")) / "Microsoft" / "WinGet" / "Packages"
    if packages.is_dir():
        # 新しい版を優先（ディレクトリ名の降順）
        for pkg in sorted(packages.glob("BrechtSanders.WinLibs*"), reverse=True):
            gxx = pkg / "mingw64" / "bin" / "g++.exe"
            if gxx.is_file():
                return str(gxx)
    return None


def _find_homebrew_gxx():
    # Apple Silicon は /opt/homebrew、Intel は /usr/local
    found = []
    for d in (Path("/opt/homebrew/bin"), Path("/usr/local/bin")):
        if not d.is_dir():
            continue
        for p in d.glob("g++-*"):
            if (m := re.fullmatch(r"g\+\+-(\d+)", p.name)) and os.access(p, os.X_OK):
                found.append((int(m.group(1)), str(p)))
    return max(found)[1] if found else None


def find_cxx() -> str:
    """@brief 使用するコンパイラのパスを返す"""
    if os.environ.get("CXX"):
        return os.environ["CXX"]
    if sys.platform == "win32":
        gxx = _find_winlibs()
    elif sys.platform == "darwin":
        gxx = _find_homebrew_gxx()
    else:
        gxx = None
    return gxx or shutil.which("g++") or "g++"


if __name__ == "__main__":
    print(find_cxx())
