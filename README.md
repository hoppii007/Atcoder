
# セットアップ（Windows / mac 共通の構成）

`.vscode/tasks.json` は同じタスク名（`build` / `build_input` / `build_run` / `submit` / `sync_snippets`）が
OS ごとに切り替わる。コンパイラは `scripts/cxx.py` が自動で選ぶ（Windows: WinLibs の g++ / mac: Homebrew の `g++-N`）。

## mac
```sh
git clone <このリポジトリ> && cd Atcoder
bash scripts/setup_mac.sh   # g++ 確認・精進パネル拡張のリンク・clangd 設定・ビルド確認
```
最後に表示されるキーバインドを `keybindings.json` に追加し、VSCode を再起動する。
`brew upgrade gcc` した後は `python3 scripts/setup_clangd.py` を再実行する（`compile_flags.txt` が版番号つきのパスを持つため）。

## Windows
- `winget install BrechtSanders.WinLibs.POSIX.UCRT`（g++）
- `python scripts/setup_clangd.py`（clangd 用 `compile_flags.txt` を生成）
- `~/.vscode/extensions/atcoder-problem-picker` から `vscode-extension/` へシンボリックリンク

## マシン間で引き継がれないもの
- `honban/.session.json`（レビュー待ちの記録）… 記録した PC でレビューまで済ませてから移る
- 精進パネルの進行中の問題・タイマー（VSCode の workspaceState）
- Claude Code のユーザー側メモリ（`~/.claude/projects/...`）。PC をまたいで効かせたいルールは CLAUDE.md か skill に書く（こちらは git で共有される）

# 環境構築（旧メモ）
- npmのインストールは(これ)[https://qiita.com/fehde/items/17a2bfff8f5a5767c600]
- atcoder-cliとonline-judge-toolsは(これ)[https://qiita.com/natsuozawa/items/c97f374900312fc4d8ca]
  - VScodeのターミナルでaccが読み込まれないのはPC再起動出なおる
- atcoder-cliのテンプレート設定はこれ[C:\Users\hoppii\Desktop\GitHub\Atcoder\Template]
- vscodeでのc++の実行とか拡張機能入れるやつ[https://qiita.com/ochx/items/01449d09777187790ee4]
- bitsの読み込みはここ[https://qiita.com/mtcd2426/items/00b22591f0917b1d9329]
  - bitsで赤の波線が消えない場合、c_cpp_properties.jsonの/include/bits/*のところを/include/*とすれば行ける
- テストケースの実行とかを簡単にやりたい場合はこれ[https://www.pike8.com/vscode-task/]
- task.jsonで使える変数とかはここ[https://qiita.com/ShortArrow/items/dc0c8cacd696154510f1]
- タスクのキー設定はこれ[https://qiita.com/sakano/items/685b6a15dd600ccd34bd]
- 問題のダウンロードを全部ダウンロードするようにする[https://qiita.com/NaokiOsako/items/dcbc0a91e1bbca8ee45d]
- 提出時の確認をしないようにするにはこれ[https://www.blog.harashio.jp/engineering/kyopro/2023/xkw5hw7mxyzf2a3rz55c/]
- accでログインできない時はこれ[https://github.com/Tatamo/atcoder-cli/issues/66]
- macでg++がclangを読み込んでしまうやつは、brew install gccでg++を入れて、/usr/loca/bin/g++-{version}を使えばいい
- tasks.json,keybinding.jsonの格納場所[https://chaba-lab.com/vscode-setting-files-path-20240808/]
- macでのkeybinding.jsonは素直にctrl+shift+pで出てくるやつを編集するのが良さそう

# How To Use?
## abc335の作り方
- acc new abc335
- templateを変えたい場合はTemplateフォルダ内のmain.cppを編集する
## テストケースの試しかた（ビルドも同時にやってくれる）
- Ctrl + Shift + T
- 変更したい場合は.vscode内のtasks.jsonとかC:\Users\hoppii\AppData\Roaming\Code\User\keybindings.jsonとかを変更する
## 普通の実行
- 普通に右上のデバッグ実行
## 提出方法
- Ctrl + Shift + S

## ランダムテストの方法
- Ctrl + Shift + D
- main.cppと同じ階層にsolver.cppとcrate_test.py（ランダムな1つぶんのテストケースを標準出力する）を作成してから行う