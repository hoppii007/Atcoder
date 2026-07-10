# AtCoder Problems ノート反映 拡張機能

`problems/` 以下の自作ノート（solved 状態・タグ・考察）を、
[AtCoder Problems](https://kenkoooo.com/atcoder) の **Table** ページに反映する Chrome 拡張です。

## できること

- **Table のセルにマーカー**: 各問題セルの左上に三角＋外枠を付け、最新回の `solved` を色分け表示
  - 🟩 緑 = 自力AC / 🟧 橙 = 解説AC / 🟥 赤 = 未AC
- **ホバーでノート表示**: セルにマウスを乗せると、タイトル・タグ・体感・つまずき分類・考察の冒頭をツールチップ表示
- **タグでフィルタ**: ツールバーのポップアップでタグを選ぶと、一致する問題セルを青枠で強調（非一致は淡色化）。solved の件数統計も表示

## セットアップ

1. データを生成する（`problems/` を JSON に変換して同梱）:

   ```
   node extension/build/generate-data.mjs
   ```

   （アイコンは生成済みだが、作り直す場合は `node extension/build/generate-icons.mjs`）

2. Chrome で `chrome://extensions` を開く → 右上の **デベロッパーモード** を ON
3. **パッケージ化されていない拡張機能を読み込む** → この `extension/` フォルダを選択
4. `https://kenkoooo.com/atcoder#/table/` を開く

## ノートを更新したら

`problems/` の md を編集したら **手順1を再実行**し、`chrome://extensions` で本拡張の
リロードボタン（↻）を押すと反映される。

## 仕組み

- Chrome 拡張はローカルの `problems/` フォルダを直接読めないため、
  `build/generate-data.mjs` が md をパースして `extension/data.json` に固める。
- `content.js` が kenkoooo.com 上の `a[href*="/tasks/"]` から問題 id（例 `abc212_c`）を拾い、
  `data.json` と突き合わせてセルを装飾する。SPA のため MutationObserver で再描画にも追従。
- タグ選択は `chrome.storage.local` を介してポップアップ ↔ content script 間で共有。

## ファイル構成

```
extension/
├── manifest.json          MV3 マニフェスト
├── data.json              生成物（build スクリプトが出力）
├── content.js / content.css   Table セルの装飾・ツールチップ・フィルタ
├── popup.html / popup.css / popup.js   タグフィルタ & 統計
├── icons/                 アイコン（生成物）
└── build/
    ├── generate-data.mjs  problems/*.md → data.json
    └── generate-icons.mjs アイコン生成
```
