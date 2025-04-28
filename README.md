
# 環境構築
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