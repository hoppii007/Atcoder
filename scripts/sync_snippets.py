#!/usr/bin/env python3
"""
library/ フォルダからVSCodeスニペットを自動生成するスクリプト

使い方:
  python scripts/sync_snippets.py

ライブラリファイルのフォーマット:
  Doxygenの @brief -> description
  それ以外 -> スニペット本体

オプションでファイル先頭にメタデータを記述可能:
  // @snippet prefix: カスタムプレフィックス
  // @snippet description: カスタム説明
"""

import json
import os
import re
from pathlib import Path
from typing import Optional, List, Dict

LIBRARY_DIR = Path(__file__).parent.parent / "library"
OUTPUT_FILE = Path(__file__).parent.parent / ".vscode" / "snippets.code-snippets"



def parse_library_file(filepath: Path) -> Optional[dict]:
    """ライブラリファイルを解析してスニペット情報を返す"""
    try:
        content = filepath.read_text(encoding="utf-8")
    except Exception as e:
        print(f"Warning: Could not read {filepath}: {e}")
        return None

    lines = content.splitlines()
    if not lines:
        return None

    # メタデータを探す
    prefix = None
    description = None
    body_start = 0

    for i, line in enumerate(lines):
        if match := re.match(r"//\s*@snippet\s+prefix:\s*(.+)", line):
            prefix = match.group(1).strip()
            body_start = i + 1
        elif match := re.match(r"//\s*@snippet\s+description:\s*(.+)", line):
            description = match.group(1).strip()
            body_start = i + 1
        elif not line.startswith("//") or not line.strip().startswith("// @snippet"):
            # メタデータ以外の行が出てきたらメタデータ解析終了
            if line.strip() and not line.startswith("// @snippet"):
                break

    # ファイル名からスニペット名を生成
    relative_path = filepath.relative_to(LIBRARY_DIR)
    name = filepath.stem  # 拡張子なしのファイル名

    # prefixが指定されていなければファイル名から生成
    if prefix is None:
        prefix = f"sni_{name}"

    # descriptionが指定されていなければ @brief または最初のコメント行から取得
    if description is None:
        # まず @brief を探す（Doxygenスタイル）
        for line in lines:
            if match := re.search(r"@brief\s+(.+)", line):
                description = match.group(1).strip()
                break

        # @brief がなければ最初のコメント行から取得
        if description is None:
            for line in lines:
                line = line.strip()
                # Doxygenブロックコメントの開始/終了や装飾行はスキップ
                if line in ("/**", "*/", "*"):
                    continue
                if line.startswith("* ") and not line.startswith("* @"):
                    description = line[2:].strip()
                    break
                if line.startswith("//") and not line.startswith("// @snippet"):
                    description = line.lstrip("/").strip()
                    break

    # bodyを構築（前後の空行と不要な引用符を除去）
    body_lines = lines[body_start:]

    # 先頭・末尾の空行を除去
    while body_lines and not body_lines[0].strip():
        body_lines.pop(0)
    while body_lines and not body_lines[-1].strip():
        body_lines.pop()

    # 古い形式（前後に引用符がある）の処理
    if body_lines:
        first_line = body_lines[0].strip()
        last_line = body_lines[-1].strip()
        if first_line in ('",', '"'):
            body_lines.pop(0)
        if last_line in ('",', '"'):
            body_lines.pop()

    if not body_lines:
        return None

    snippet = {
        "prefix": prefix,
        "body": body_lines,
    }
    if description:
        snippet["description"] = description

    return {"name": name, "snippet": snippet}


def collect_library_files() -> List[Path]:
    """library/ 以下の全 .cpp ファイルを収集"""
    return sorted(LIBRARY_DIR.rglob("*.cpp"))


def generate_snippets() -> Dict:
    """全ライブラリファイルからスニペットを生成"""
    snippets = {}

    for filepath in collect_library_files():
        result = parse_library_file(filepath)
        if result:
            snippets[result["name"]] = result["snippet"]
            print(f"  + {result['name']} ({filepath.relative_to(LIBRARY_DIR)})")

    return snippets


def main():
    print("Syncing snippets from library/...")
    print()

    snippets = generate_snippets()

    # JSON出力（読みやすい形式で）
    output = json.dumps(snippets, ensure_ascii=False, indent=2)

    OUTPUT_FILE.parent.mkdir(parents=True, exist_ok=True)
    OUTPUT_FILE.write_text(output, encoding="utf-8")

    print()
    print(f"Generated {len(snippets)} snippets -> {OUTPUT_FILE}")


if __name__ == "__main__":
    main()
