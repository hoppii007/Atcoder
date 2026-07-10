---
name: ask-solved-felt-if-empty
description: レビュー時 solved/felt が未記入なら必ず本人に質問してから記録する
metadata:
  type: feedback
---

honban-review で第N回を記録する際、honban ブロックの `solved` と `felt` が
埋まっていない場合は、憶測で断定せず **必ず本人に質問して確定させてから** 記録する。

**Why:** `solved` は弱点分析の要（CLAUDE.md 目的#1）、`felt` は体感と客観 difficulty の
ズレ分析の材料。ここを憶測で埋めると分析全体が歪む。

**How to apply:** 記録前に両項目の有無を確認し、空なら質問。両方揃ってから第N回を追記する。
skill 側（`.claude/skills/honban-review/SKILL.md` 手順4・注意）にも反映済み。
