// AtCoder 精進支援 — 精進パネル（問題選択・タイマー・記録）と、library の include 補完・ホバー。
const vscode = require("vscode");
const timer = require("./timer");
const shojin = require("./shojin");
const include_completion = require("./include_completion");
const library_hover = require("./library_hover");

function activate(context) {
  // 解答タイマー（ステータスバー表示。クリックで精進パネルを開く）
  timer.activate(context);

  // 精進パネル（サイドバーのビュー）
  shojin.activate(context);

  // #include "library/..." のパス補完
  include_completion.activate(context);

  // #include "library/..." にカーソルを合わせると使い方を表示
  library_hover.activate(context);
}

function deactivate() {
  timer.deactivate();
}

module.exports = { activate, deactivate };
