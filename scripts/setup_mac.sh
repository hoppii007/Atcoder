#!/usr/bin/env bash
# mac で精進環境をセットアップする（clone 直後に 1 回実行。何度実行しても安全）
#
#   bash scripts/setup_mac.sh
#
# 1. Homebrew の GNU g++ を確認（無ければ brew install gcc）
# 2. 精進パネル拡張を ~/.vscode/extensions にシンボリックリンク
# 3. clangd 用 compile_flags.txt を生成
# 4. template.hpp の PCH を作って honban/main.cpp をビルドできるか確認
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT"

echo "== 1. GNU g++ =="
if ! ls /opt/homebrew/bin/g++-* /usr/local/bin/g++-* >/dev/null 2>&1; then
  if command -v brew >/dev/null 2>&1; then
    brew install gcc
  else
    echo "Homebrew がありません。https://brew.sh を入れてから再実行してください" >&2
    exit 1
  fi
fi
echo "使用するコンパイラ: $(python3 scripts/cxx.py)"

echo "== 2. 精進パネル拡張 =="
EXT_DIR="$HOME/.vscode/extensions"
LINK="$EXT_DIR/atcoder-problem-picker"
mkdir -p "$EXT_DIR"
if [ -L "$LINK" ] || [ ! -e "$LINK" ]; then
  ln -sfn "$ROOT/vscode-extension" "$LINK"
  echo "$LINK -> $ROOT/vscode-extension"
else
  echo "$LINK が実体のフォルダとして既にあります。消してから再実行してください" >&2
  exit 1
fi

echo "== 3. clangd =="
python3 scripts/setup_clangd.py

echo "== 4. ビルド確認 =="
python3 scripts/build.py honban/main.cpp honban/main.out
echo "OK"

cat <<'EOS'

残りの手作業:
  - VSCode を再起動（拡張と推奨拡張の読み込み）
  - キーバインド: Cmd+Shift+P →「Preferences: Open Keyboard Shortcuts (JSON)」に以下を追加
      { "key": "ctrl+shift+i", "command": "workbench.action.tasks.runTask", "args": "build_input" },
      { "key": "ctrl+shift+r", "command": "workbench.action.tasks.runTask", "args": "build_run" }
EOS
