/**
 * 依存なしで拡張のアイコン PNG を生成する（icons/icon48.png, icon128.png）。
 * 背景 + solved 3色（緑/橙/赤）の縦ストライプという素朴な意匠。
 *   node extension/build/generate-icons.mjs
 */
import { deflateSync } from "node:zlib";
import { writeFileSync, mkdirSync } from "node:fs";
import { fileURLToPath } from "node:url";
import { dirname, join } from "node:path";

const __dirname = dirname(fileURLToPath(import.meta.url));
const icons_dir = join(__dirname, "..", "icons");
mkdirSync(icons_dir, { recursive: true });

function crc32(buf) {
  let c = ~0;
  for (let i = 0; i < buf.length; i++) {
    c ^= buf[i];
    for (let k = 0; k < 8; k++) c = (c >>> 1) ^ (0xedb88320 & -(c & 1));
  }
  return ~c >>> 0;
}
function chunk(type, data) {
  const len = Buffer.alloc(4);
  len.writeUInt32BE(data.length, 0);
  const td = Buffer.concat([Buffer.from(type, "ascii"), data]);
  const crc = Buffer.alloc(4);
  crc.writeUInt32BE(crc32(td), 0);
  return Buffer.concat([len, td, crc]);
}
function png(size, pixel) {
  const bg = [0x1e, 0x1e, 0x2e];
  const cols = [
    [0x2e, 0x7d, 0x32],
    [0xef, 0x6c, 0x00],
    [0xc6, 0x28, 0x28],
  ];
  const raw = Buffer.alloc((size * 3 + 1) * size);
  let o = 0;
  for (let y = 0; y < size; y++) {
    raw[o++] = 0; // filter
    for (let x = 0; x < size; x++) {
      const [r, g, b] = pixel(x, y, size, bg, cols);
      raw[o++] = r;
      raw[o++] = g;
      raw[o++] = b;
    }
  }
  const sig = Buffer.from([0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a]);
  const ihdr = Buffer.alloc(13);
  ihdr.writeUInt32BE(size, 0);
  ihdr.writeUInt32BE(size, 4);
  ihdr[8] = 8; // bit depth
  ihdr[9] = 2; // color type: truecolor
  const idat = deflateSync(raw);
  return Buffer.concat([sig, chunk("IHDR", ihdr), chunk("IDAT", idat), chunk("IEND", Buffer.alloc(0))]);
}

// 下部に 3本のストライプ、それ以外は背景
function pixel(x, y, size, bg, cols) {
  const m = Math.round(size * 0.16);
  if (x < m || x >= size - m || y < m || y >= size - m) return bg;
  const inner = size - 2 * m;
  const band = Math.floor(((x - m) / inner) * 3);
  return cols[Math.min(2, Math.max(0, band))];
}

for (const s of [48, 128]) {
  writeFileSync(join(icons_dir, `icon${s}.png`), png(s, pixel));
}
console.log("icons 生成完了: icon48.png, icon128.png");
