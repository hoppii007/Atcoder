# AtCoder 精進パネル（VSCode 拡張）

AtCoder の精進フロー（問題を選ぶ → 解く → 記録する）を、サイドバーの **精進パネル** 1つで管理する拡張。
あわせて `library/` の include 補完とホバー表示を持つ。

## 使い方

### 精進パネル

アクティビティバー（左端の縦アイコン列）の **「精進」**（時計のアイコン）を開く。
ステータスバー左下のタイマー表示をクリックしても開く。

1. **問題を選ぶ**
   - `ランダムに選んで開始` … `problems/` にノートが無い問題から1問（範囲は設定の from / to / levels）
   - `URL・問題 ID で開始` … 問題 URL、または `abc318_f` / `ABC318F` の形で入力
   - どちらも問題ページがブラウザで開き、タイマーが自動で始まる
2. **解く**
   - タイマー（一時停止／再開できる）と **思考ログ**。考えたことを書いて「追加」（Ctrl+Enter）すると、
     タイマーの経過時間つきで1件ずつ積まれる（× で削除可）。入力途中の文も自動保存される
   - 解いている間は Claude に答えを求めない前提。ログは原文のままノートに残り、レビュー時に
     Claude がこれを分析する
   - リロードしても進行中の問題・タイマー・考察は消えない（`workspaceState` に保存）
   - **コードの推移** も自動で記録される。解き始め・`honban/main.cpp` の保存・思考ログの追加・
     タスク（build / input / run / expand / submit）の終了・「解き終わった」のたびに、main.cpp を
     経過時間つきで `honban/.history/<task_id>/NNN.cpp` に保存する（内容が変わらなければ新しいファイルは作らない。
     タスクは終了コードも残すので、コンパイルエラーや実行時エラーの回数が分かる）。レビューで Claude が差分を読む
3. **解き終わった** を押す
   - タイマーが止まり、difficulty を AtCoder Problems から自動取得（`problem-models.json`。
     拡張の globalStorage にキャッシュし、無い問題があれば1時間おきに取り直す）
   - solved（自力AC / 解説AC / 未AC）を選び、所要時間（タイマー値、修正可）を確認して **記録する**
   - `honban/.session.json`（gitignore 済み）に書き出される。**この時点ではまだノートに入っていない**
4. Claude Code に **「レビューして」** と言う
   - `honban-review` skill が session・`honban/main.cpp`・会話を読み、`problems/` のノートに第N回として記録する
   - session が残っている間、パネル上部に **レビュー待ち** が出続ける。「Claude を開く」で直前の
     Claude Code の会話が開き、「レビューして」がクリップボードに入る。レビューが済むと session が消え、表示も消える
   - レビュー待ちのまま次の問題を始めようとすると確認が出る。別の問題を記録すると上書きになるので、その場合も確認が出る

記録は客観的事実だけで作る方針のため、体感（felt）の入力は無い。

その他のコマンド（コマンドパレット）: `AtCoder: 精進パネルを開く`、`AtCoder: 未挑戦の問題をランダムに選んで開始`、
`AtCoder: タイマー開始／一時停止`、`AtCoder: タイマーをリセット`

### ライブラリ include 補完

C++ ファイルで `#include "` と打つと `library/` が、`#include "library/` 以降はフォルダ・`.cpp` ファイルが
候補に出る（ファイルには `@brief` を説明として表示、選択中は Doxygen コメントをプレビュー）。
提出時の展開は [`../scripts/expand.py`](../scripts/expand.py)（`Ctrl+Shift+I` の `build_input` で自動実行）。

### ライブラリの使い方ホバー

`#include "library/..."` の行にカーソルを合わせると、そのライブラリの `@brief`・先頭の Doxygen ブロック
（`@code` の使用例）・定義一覧（シグネチャ＋説明）・依存ライブラリ・ファイルを開くリンクを表示する。

## 設定（Settings → 拡張機能 → AtCoder 精進パネル）

| 設定 | 既定 | 説明 |
|------|------|------|
| `atcoderPicker.from` | 212 | ランダムピックの対象開始コンテスト番号 |
| `atcoderPicker.to` | 318 | ランダムピックの対象終了コンテスト番号 |
| `atcoderPicker.levels` | `cde` | ランダムピックの対象レベル |

## インストール

`~/.vscode/extensions/atcoder-problem-picker` からこのフォルダへシンボリックリンク済み
（mac では `bash scripts/setup_mac.sh` がリンクを張る）。
VSCode を再起動（またはコマンドパレットで `Developer: Reload Window`）すると有効になる。

編集後は `Developer: Reload Window` で反映される。

Windows / mac で同じ構成になる（拡張は Node の `path` / `fs` だけを使い、タスクの label は `.vscode/tasks.json` で両 OS 共通）。
mac 側は `git pull` → `Developer: Reload Window` で、コードの推移の記録も含めて同じ状態になる。
