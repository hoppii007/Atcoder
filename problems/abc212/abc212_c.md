---
id: abc212_c
title: Min Difference
url: https://atcoder.jp/contests/abc212/tasks/abc212_c
difficulty: 246 # AtCoder Problems の difficulty 値、なければ空
tags: [near_value, 二分探索] # 必須。三層構造は ../../tags.md 参照。問題レベルで固定
confidence: 確定 # 確定 / 候補 / 未分類（tags の確信度。問題レベル）
---

## 問題

それぞれ N 個、M 個の正整数からなる 2 つの数列 A=(A_1,…,A_N) と B=(B_1,…,B_M) が与えられる。
それぞれの数列から 1 つずつ要素を選んだときの、2 つの値の差の最小値を求める。

入力形式:
```
N M
A_1 A_2 … A_N
B_1 B_2 … B_M
```

**制約**

- 1 ≤ N, M ≤ 2×10^5
- 1 ≤ A_i ≤ 10^9
- 1 ≤ B_i ≤ 10^9
- 入力は全て整数

## 挑戦履歴

### 第1回 — 2026-07-09

- solved: 自力AC # 自力AC / 解説AC / 未AC（弱点分析の要）
- felt: 簡単 # 簡単 / 中 / 難しい（その回の体感難易度。客観 difficulty とは別）
- time_spent_min: # 解くのにかかった時間（分）。空欄可
- つまずき分類: []

**考察ノート**

「一番近い値」が高速に求められればいいことにすぐ気づいて、二分探索でいけるしライブラリがあるのですぐにできた。

**難所**

特になし

## 問題全体のメモ

特になし
