# AtCoder Problem Picker + Solve Timer（VSCode 拡張）

AtCoder の解答フローを支援する VSCode 拡張。2 機能を持つ。

1. **Problem Picker** … `problems/` に記録の無い ABC 問題からランダムに1問選び、ブラウザで開く。
   判定ロジックは [`../pick_problem.py`](../pick_problem.py) と同じ（md ファイルの有無で未挑戦を判定）。
2. **Solve Timer**（B-4）… 解いている時間を計測し、解き終わりに `solved` / `felt` / `つまずき分類` を
   入力すると、挑戦履歴（第N回）ブロックを組み立ててクリップボードにコピーする。`time_spent_min` は
   タイマーから自動算出。記録の入力漏れを防ぎ、弱点分析（CLAUDE.md 目的#1）に食わせるデータの質を上げる。

## 使い方

### Problem Picker

- **ステータスバー左下の `🚀 ATC ランダム` ボタン** をクリック → 未挑戦の C〜E 問題からランダムに1問開く。
- コマンドパレット（`Cmd+Shift+P`）:
  - `AtCoder: 未挑戦の問題をランダムに開く`
  - `AtCoder: 未挑戦の候補を一覧から選んで開く`

### Solve Timer

- ステータスバー左の **`⌚ 12:34`** をクリック → タイマー開始／一時停止（トグル）。
- **`🚀 ATC ランダム`（および一覧から選択）で問題を開くと、タイマーは自動でリセット＆スタート**する。
- 解き終わったらコマンドパレットで **`AtCoder: 記録する`**:
  1. `solved`（自力AC / 解説AC / 未AC）
  2. `felt`（簡単 / 中 / 難しい）
  3. `つまずき分類`（複数選択。無ければそのまま Enter → `[]`）※語彙は [`../tags.md`](../tags.md) の専用表と一致
  4. `time_spent_min`（タイマーから自動、必要なら修正）
  → 第N回ブロックを **クリップボードにコピー** し、`honban/.session.json`（gitignore 済み）に構造化保存。
  そのまま md に貼るか、`終わりました。レビューして`（honban-review）に渡す。
- リロードしてもタイマーは消えない（`workspaceState` に永続化）。
- その他コマンド: `AtCoder: タイマー開始／一時停止`, `AtCoder: タイマーをリセット`

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
