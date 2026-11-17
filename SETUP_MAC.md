# mac セットアップ手順

Windows と同じ精進環境（精進パネル・ビルドタスク・ライブラリ include・clangd・Claude Code のレビュー）を mac で使えるようにする手順。
上から順に実行すればよい。各手順の最後に「確認」を置いてあるので、通らなければ下の「トラブルシュート」を見る。

---

## 0. 前提ツール

| ツール | 入れ方 | 確認 |
|---|---|---|
| Xcode Command Line Tools | `xcode-select --install` | `xcode-select -p` がパスを返す |
| Homebrew | https://brew.sh のワンライナー | `brew --version` |
| git | CLT に同梱 | `git --version` |
| Python 3 | CLT に同梱（無ければ `brew install python`） | `python3 --version` |
| VSCode | https://code.visualstudio.com | — |
| `code` コマンド | VSCode で Cmd+Shift+P →「Shell Command: Install 'code' command in PATH」 | `code --version` |

> mac の `g++` は実体が Apple clang で `bits/stdc++.h` が無い。GNU g++ は手順 2 のスクリプトが `brew install gcc` で入れる。

---

## 1. リポジトリを取得

```sh
mkdir -p ~/GitHub && cd ~/GitHub
git clone https://github.com/hoppii007/Atcoder.git
cd Atcoder
```

GitHub の認証を求められたら `brew install gh && gh auth login` で済ませるのが楽。

**確認**: `ls` で `honban/ library/ problems/ scripts/ vscode-extension/` が見える。

---

## 2. セットアップスクリプトを実行

```sh
bash scripts/setup_mac.sh
```

やること（何度実行しても安全）:

1. Homebrew の GNU g++（`g++-15` など）が無ければ `brew install gcc`（数分かかる）
2. 精進パネル拡張を `~/.vscode/extensions/atcoder-problem-picker` → `vscode-extension/` にシンボリックリンク
3. clangd 用の `compile_flags.txt` を生成（`scripts/setup_clangd.py`）
4. `template.hpp` の PCH を作り、`honban/main.cpp` をビルドできるか確認

**確認**: 最後に `OK` と、キーバインドの案内が出る。

---

## 3. VSCode の設定

```sh
code .
```

1. 右下に「推奨拡張機能をインストールしますか」が出たら **すべてインストール**
   （clangd / Run on Save / Claude Code / Python。`.vscode/extensions.json` に定義）
   - 出なければ拡張機能ビューで `@recommended` を検索して入れる
2. clangd 拡張が「clangd が無い。ダウンロードするか」と聞いてきたら **ダウンロード**
3. Microsoft の C/C++ 拡張（ms-vscode.cpptools）を入れている場合は、clangd と衝突するので
   ユーザー設定（Cmd+Shift+P →「Preferences: Open User Settings (JSON)」）に追加:
   ```json
   "C_Cpp.intelliSenseEngine": "disabled"
   ```
4. キーバインドを追加: Cmd+Shift+P →「Preferences: Open Keyboard Shortcuts (JSON)」で、`[ ]` の中に:
   ```json
   { "key": "ctrl+shift+i", "command": "workbench.action.tasks.runTask", "args": "build_input" },
   { "key": "ctrl+shift+r", "command": "workbench.action.tasks.runTask", "args": "build_run" }
   ```
   （Cmd にしたい場合は `cmd+shift+i` などに変えてよい。ただし既存のショートカットと被らないか確認）
5. Cmd+Shift+P →「Developer: Reload Window」

**確認**:
- 左のアクティビティバーに **「精進」**（時計アイコン）がある
- `honban/main.cpp` を開いて `#include "library/` と打つと候補が出る
- `honban/main.cpp` に赤い波線が出ていない（出たらトラブルシュート参照）

---

## 4. 動作確認（精進の一連の流れ）

1. `honban/in` に入力例を書き、`honban/main.cpp` を開いた状態で **Ctrl+Shift+I**
   → ビルド → `submit.cpp` 生成＆クリップボードへコピー → `in` を入力して実行、まで流れる
2. Cmd+Shift+P →「Tasks: Run Task」→ `submit` で、展開後のコードがコンパイルできるか確認しつつクリップボードへ
3. `library/` の `.cpp` を保存すると `.vscode/snippets.code-snippets` が自動更新される（Run on Save）
4. 精進パネルで問題を選ぶ → 解く → 「解き終わった」→ solved を選んで「記録する」
   → `honban/.session.json` ができる

---

## 5. Claude Code

1. VSCode の Claude Code 拡張を開き、ログインする
2. このフォルダで開けば `CLAUDE.md` と `.claude/skills/honban-review/` は git から読まれるので、
   **「レビューして」で honban-review skill がそのまま動く**
3. 引き継がれないもの:
   - ユーザー側の自動メモリ（`~/.claude/projects/...`）は PC ごと。Windows 側の AHC 用メモリ
     （main.cpp 先頭の方針コメント / 実装後は計測まで自走 / 旧解法コードは消す）が必要なら、mac の Claude に
     同じ内容を「覚えておいて」と伝え直す
   - `.claude/settings.local.json`（atcoder.jp の WebFetch 許可）も PC ごと。初回に許可を求められたら許可する

---

## 6. Windows と mac を行き来するときのルール

- **作業を始める前に `git pull`、終わったら commit & push**
- `honban/.session.json`（レビュー待ちの記録）は git で運ばれない。**記録した PC でレビューまで済ませてから移る**
- 精進パネルの進行中の問題・タイマー・書きかけの思考ログも PC ごと（VSCode の workspaceState）
- ビルド成果物（`main.exe` / `main.out`）・PCH・`compile_flags.txt`・`submit.cpp` は追跡しない。各 PC で生成される
- `.vscode/tasks.json` と `settings.json` は両 OS 共通。片方で編集したら、もう片方でも動くか意識する
  （OS ごとの差は tasks.json の `"windows"` / `"osx"` に書く）

---

## トラブルシュート

| 症状 | 対処 |
|---|---|
| `setup_mac.sh` で `brew: command not found` | Homebrew を入れ、表示される `eval "$(/opt/homebrew/bin/brew shellenv)"` を `~/.zprofile` に追記して新しいターミナルで再実行 |
| ビルドで `bits/stdc++.h: No such file` | Apple clang が使われている。`python3 scripts/cxx.py` が `/opt/homebrew/bin/g++-15` のようなパスを返すか確認。返さなければ `brew install gcc`。強制したいときは `export CXX=/opt/homebrew/bin/g++-15` |
| `ld: library not found` などリンクエラー | Xcode CLT が古い・壊れている。`sudo rm -rf /Library/Developer/CommandLineTools && xcode-select --install` |
| `warning: ... template.hpp.gch ... not used`（-Winvalid-pch） | `rm template.hpp.gch template.hpp.gch.cxx` して再ビルド（自動で作り直される） |
| エディタに赤い波線（`bits/stdc++.h` が見つからない等） | `python3 scripts/setup_clangd.py` を再実行 → Cmd+Shift+P「clangd: Restart language server」。g++ を brew upgrade で更新したときも再実行が必要。それでも消えなければ、`compile_flags.txt` の中身とエラー文を Claude に渡して相談 |
| 精進パネルが出ない | `ls -l ~/.vscode/extensions/atcoder-problem-picker` がリンクになっているか確認 → 「Developer: Reload Window」 |
| Ctrl+Shift+I で何も起きない | keybindings.json の追記漏れ。Cmd+Shift+P「Tasks: Run Task」→ `build_input` が動くならキー設定の問題 |
| スニペットが更新されない | Run on Save 拡張が入っているか確認。手動なら「Tasks: Run Task」→ `sync_snippets` |
| `main.out` 実行で「開発元を検証できません」 | 自分でビルドしたバイナリには通常出ない。出たら `xattr -d com.apple.quarantine honban/main.out` |
