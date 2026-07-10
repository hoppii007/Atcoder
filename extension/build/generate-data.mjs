#!/usr/bin/env node
/**
 * problems/ 以下の問題ノート md を走査し、Chrome 拡張が読む data.json を生成する。
 *
 * 使い方:
 *   node extension/build/generate-data.mjs
 *
 * ノートを更新したらこれを再実行し、chrome://extensions で拡張をリロードする。
 */
import { readFileSync, writeFileSync, readdirSync, statSync } from "node:fs";
import { fileURLToPath } from "node:url";
import { dirname, join, relative } from "node:path";

const __dirname = dirname(fileURLToPath(import.meta.url));
const repo_root = join(__dirname, "..", ".."); // extension/build → repo root
const problems_dir = join(repo_root, "problems");
const out_path = join(__dirname, "..", "data.json");

/** problems/ 以下の *.md を（_template.md を除いて）再帰列挙する。 */
function collect_md_files(dir) {
  const result = [];
  for (const name of readdirSync(dir)) {
    const full = join(dir, name);
    if (statSync(full).isDirectory()) {
      result.push(...collect_md_files(full));
    } else if (name.endsWith(".md") && name !== "_template.md") {
      result.push(full);
    }
  }
  return result;
}

/** `値 # コメント` の末尾コメントを落とす。 */
function strip_comment(value) {
  const idx = value.indexOf("#");
  return (idx === -1 ? value : value.slice(0, idx)).trim();
}

/** `[a, b, c]` 形式を配列に。空/[] は []。 */
function parse_list(value) {
  const v = strip_comment(value).trim();
  const inner = v.replace(/^\[/, "").replace(/\]$/, "").trim();
  if (!inner) return [];
  return inner
    .split(",")
    .map((s) => s.trim())
    .filter(Boolean);
}

/** フロントマター（先頭の --- ... --- ブロック）を key→value で返す。 */
function parse_front_matter(text) {
  const m = text.match(/^---\n([\s\S]*?)\n---/);
  const fm = {};
  if (!m) return fm;
  for (const line of m[1].split("\n")) {
    const kv = line.match(/^([A-Za-z_]+):\s*(.*)$/);
    if (kv) fm[kv[1]] = kv[2];
  }
  return fm;
}

/**
 * 挑戦履歴の各 `### 第N回` ブロックを解析して配列で返す（出現順＝古い→新しい）。
 */
function parse_attempts(text) {
  // 挑戦履歴セクション以降のみ対象にする
  const hist_idx = text.indexOf("## 挑戦履歴");
  const scope = hist_idx === -1 ? text : text.slice(hist_idx);

  const heading_re = /^###\s*第(\d+)回\s*(?:—\s*(\d{4}-\d{2}-\d{2}))?/gm;
  const marks = [];
  let mm;
  while ((mm = heading_re.exec(scope)) !== null) {
    marks.push({ round: Number(mm[1]), date: mm[2] || null, start: mm.index });
  }

  const attempts = [];
  for (let i = 0; i < marks.length; i++) {
    const start = marks[i].start;
    // 次の `### 第N回` または `## ` セクション見出しまで
    let end = i + 1 < marks.length ? marks[i + 1].start : scope.length;
    const next_section = scope.slice(start + 3).search(/^##\s/m);
    if (next_section !== -1) {
      const abs = start + 3 + next_section;
      if (abs < end) end = abs;
    }
    const block = scope.slice(start, end);

    const get = (re) => {
      const r = block.match(re);
      return r ? strip_comment(r[1]) : null;
    };

    const solved = get(/^-\s*solved:\s*(.*)$/m);
    const felt = get(/^-\s*felt:\s*(.*)$/m);
    const time_raw = get(/^-\s*time_spent_min:\s*(.*)$/m);
    const time_spent = time_raw && /\d/.test(time_raw) ? Number(time_raw.match(/\d+/)[0]) : null;
    const stumble_raw = block.match(/^-\s*つまずき分類:\s*(.*)$/m);
    const stumble = stumble_raw ? parse_list(stumble_raw[1]) : [];

    // 考察ノート: **考察ノート** の後、次の **見出し** か ### / ## まで
    let consideration = null;
    const ci = block.indexOf("**考察ノート**");
    if (ci !== -1) {
      const rest = block.slice(ci + "**考察ノート**".length);
      const stop = rest.search(/(^\*\*[^\n]+\*\*\s*$)|(^###\s)|(^##\s)/m);
      consideration = (stop === -1 ? rest : rest.slice(0, stop)).trim();
    }

    attempts.push({
      round: marks[i].round,
      date: marks[i].date,
      solved: solved || null,
      felt: felt || null,
      timeSpent: time_spent,
      stumble,
      consideration: consideration || "",
    });
  }
  return attempts;
}

const files = collect_md_files(problems_dir);
const problems = {};
const tag_index = {};

for (const file of files) {
  const text = readFileSync(file, "utf8").replace(/\r\n/g, "\n").replace(/^﻿/, "");
  const fm = parse_front_matter(text);
  const id = fm.id || null;
  if (!id) {
    console.warn(`[skip] id 無し: ${relative(repo_root, file)}`);
    continue;
  }

  const tags = fm.tags ? parse_list(fm.tags) : [];
  const difficulty = fm.difficulty && /\d/.test(strip_comment(fm.difficulty))
    ? Number(strip_comment(fm.difficulty).match(/-?\d+/)[0])
    : null;
  const attempts = parse_attempts(text);
  const latest = attempts.length ? attempts[attempts.length - 1] : null;
  const first = attempts.length ? attempts[0] : null;

  problems[id] = {
    id,
    title: fm.title ? strip_comment(fm.title) : id,
    url: fm.url ? strip_comment(fm.url) : `https://atcoder.jp/contests/${id.split("_")[0]}/tasks/${id}`,
    difficulty,
    tags,
    confidence: fm.confidence ? strip_comment(fm.confidence) : null,
    attempts: attempts.length,
    latest,
    firstSolved: first ? first.solved : null,
    notePath: relative(repo_root, file).replace(/\\/g, "/"),
  };

  for (const t of tags) {
    (tag_index[t] ||= []).push(id);
  }
}

const data = {
  generatedAt: new Date().toISOString(),
  count: Object.keys(problems).length,
  problems,
  tags: tag_index,
};

writeFileSync(out_path, JSON.stringify(data, null, 2) + "\n", "utf8");
console.log(`生成完了: ${relative(repo_root, out_path)}  (${data.count} 問, ${Object.keys(tag_index).length} タグ)`);
