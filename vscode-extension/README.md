# AtCoder Problem Picker（VSCode 拡張）

`problems/` に記録の無い AtCoder ABC 問題からランダムに1問選び、ブラウザで開く VSCode 拡張。
判定ロジックは [`../pick_problem.py`](../pick_problem.py) と同じ（md ファイルの有無で未挑戦を判定）。

## 使い方

- **ステータスバー左下の `🚀 ATC ランダム` ボタン** をクリック → 未挑戦の C〜E 問題からランダムに1問開く。
- コマンドパレット（`Cmd+Shift+P`）:
  - `AtCoder: 未挑戦の問題をランダムに開く`
  - `AtCoder: 未挑戦の候補を一覧から選んで開く`

## 設定（Settings → 拡張機能 → AtCoder Problem Picker）

| 設定 | 既定 | 説明 |
|------|------|------|
| `atcoderPicker.from` | 212 | 対象開始コンテスト番号 |
| `atcoderPicker.to` | 318 | 対象終了コンテスト番号 |
| `atcoderPicker.levels` | `cde` | 対象レベル |

## インストール

`~/.vscode/extensions/atcoder-problem-picker` からこのフォルダへシンボリックリンク済み。
VSCode を再起動（またはコマンドパレットで `Developer: Reload Window`）すると有効になる。

編集後は `Developer: Reload Window` で反映される。
