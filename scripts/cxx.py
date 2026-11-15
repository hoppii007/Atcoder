"""
ビルドに使う C++ コンパイラを決める

優先順:
  1. 環境変数 CXX
  2. winget で入れた WinLibs（64bit g++。__int128 が使える）
  3. PATH 上の g++
"""

import os
import shutil
from pathlib import Path


def find_cxx() -> str:
    """@brief 使用するコンパイラのパスを返す"""
    if os.environ.get("CXX"):
        return os.environ["CXX"]
    packages = Path(os.environ.get("LOCALAPPDATA", "")) / "Microsoft" / "WinGet" / "Packages"
    if packages.is_dir():
        # 新しい版を優先（ディレクトリ名の降順）
        for pkg in sorted(packages.glob("BrechtSanders.WinLibs*"), reverse=True):
            gxx = pkg / "mingw64" / "bin" / "g++.exe"
            if gxx.is_file():
                return str(gxx)
    return shutil.which("g++") or "g++"
