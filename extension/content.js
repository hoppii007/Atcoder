/**
 * AtCoder Problems (kenkoooo.com) の Table 上に、自作ノートの情報を反映する content script。
 *
 * - 各問題セルに solved 状態（自力AC/解説AC/未AC）のマーカーを付ける
 * - ホバーでノート内容（タグ・体感・考察の冒頭）をツールチップ表示
 * - ポップアップで選んだタグに一致するセルを強調（非一致は淡色化）
 */
(() => {
  "use strict";

  const SOLVED_CLASS = {
    自力AC: "apn-solved-self",
    解説AC: "apn-solved-editorial",
    未AC: "apn-unsolved",
  };

  // solved 状態ごとの基準色（RGB）。直近に解いたほど濃く、時間経過で薄れる。
  const SOLVED_RGB = {
    自力AC: [46, 125, 50],
    解説AC: [239, 108, 0],
    未AC: [198, 40, 40],
  };

  // 記憶の減衰パラメータ（すべて調整可）
  const MAX_ALPHA = 0.55; // 解いた直後の濃さ
  const FADE_DAYS = 182; // これを過ぎると真っ白（半年）
  const KNEE_DAYS = 30; // 最初の1ヶ月は速めに薄くなる
  const KNEE_FRAC = 0.6; // 30日時点で残す割合（<この分だけ最初の月で早く落とす）

  /** 経過日数 → 残存割合 f(t)∈[0,1]。最初の1ヶ月は急、以降は残りを線形に。 */
  function decay_fraction(days) {
    if (days <= 0) return 1;
    if (days >= FADE_DAYS) return 0;
    if (days <= KNEE_DAYS) {
      // 1.0 → KNEE_FRAC（勾配きつめ）
      return 1 - (1 - KNEE_FRAC) * (days / KNEE_DAYS);
    }
    // KNEE_FRAC → 0（残りを線形に）
    return KNEE_FRAC * (1 - (days - KNEE_DAYS) / (FADE_DAYS - KNEE_DAYS));
  }

  /** 'YYYY-MM-DD' から今日までの経過日数。無効なら null。 */
  function days_since(date_str) {
    if (!date_str) return null;
    const t = Date.parse(date_str + "T00:00:00");
    if (Number.isNaN(t)) return null;
    return Math.floor((Date.now() - t) / 86400000);
  }

  /** 最新挑戦日からの経過に応じて薄れる背景色を返す。該当外/退色済みは null。 */
  function bg_color(solved, latest) {
    const rgb = SOLVED_RGB[solved];
    if (!rgb) return null;
    const days = days_since(latest && latest.date);
    const frac = days == null ? 1 : decay_fraction(days);
    const alpha = MAX_ALPHA * frac;
    if (alpha < 0.02) return null; // ほぼ真っ白なら塗らない
    return `rgba(${rgb[0]}, ${rgb[1]}, ${rgb[2]}, ${alpha.toFixed(3)})`;
  }

  let DATA = null; // { problems: {id: {...}}, tags: {tag: [id]} }
  let selected_tags = []; // タグフィルタで選択中
  let show_details = false; // ツールチップにタグ・つまずきを出すか

  /** href から問題 id（例: abc212_c）を取り出す。該当しなければ null。 */
  function id_from_href(href) {
    if (!href) return null;
    const m = href.match(/\/tasks\/([a-z0-9_]+)(?:[?#].*)?$/i);
    return m ? m[1] : null;
  }

  /** ツールチップ要素（シングルトン）。 */
  function tooltip_el() {
    let el = document.getElementById("apn-tooltip");
    if (!el) {
      el = document.createElement("div");
      el.id = "apn-tooltip";
      el.style.display = "none";
      document.body.appendChild(el);
    }
    return el;
  }

  function build_tooltip_html(p) {
    const L = p.latest || {};
    const solved = L.solved || "—";
    const badge = SOLVED_CLASS[L.solved] ? `apn-badge ${SOLVED_CLASS[L.solved]}` : "apn-badge";
    const felt = L.felt ? `体感: ${esc(L.felt)}` : "";
    const time = L.timeSpent != null ? ` / ${L.timeSpent}分` : "";
    const attempts = `${p.attempts}回挑戦` + (p.attempts > 1 ? `（初回 ${esc(p.firstSolved || "—")}）` : "");

    // 詳細（タグ・つまずき）はトグル ON のときだけ
    let details = "";
    if (show_details) {
      const tags = (p.tags || []).map((t) => `<span class="apn-tag">${esc(t)}</span>`).join("");
      if (tags) details += `<div class="apn-tt-tags">${tags}</div>`;
      if (L.stumble && L.stumble.length) {
        details += `<div class="apn-tt-stumble">つまずき: ${L.stumble.map(esc).join(", ")}</div>`;
      }
    }
    return `
      <div class="apn-tt-head">
        <span class="${badge}">${esc(solved)}</span>
        <strong>${esc(p.title)}</strong>
        <span class="apn-tt-id">${esc(p.id)}</span>
      </div>
      <div class="apn-tt-meta">${esc(attempts)}${felt ? " / " + esc(felt) : ""}${esc(time)}</div>
      ${details}
      <div class="apn-tt-foot">クリックで AtCoder の問題へ</div>
    `;
  }

  function esc(s) {
    return String(s).replace(/[&<>"']/g, (c) =>
      ({ "&": "&amp;", "<": "&lt;", ">": "&gt;", '"': "&quot;", "'": "&#39;" }[c])
    );
  }
  function trunc(s, n) {
    return s.length > n ? s.slice(0, n) + "…" : s;
  }

  function show_tooltip(p, x, y) {
    const el = tooltip_el();
    el.innerHTML = build_tooltip_html(p);
    el.style.display = "block";
    position_tooltip(el, x, y);
  }
  function position_tooltip(el, x, y) {
    const pad = 14;
    const rect = el.getBoundingClientRect();
    let left = x + pad;
    let top = y + pad;
    if (left + rect.width > window.innerWidth) left = x - rect.width - pad;
    if (top + rect.height > window.innerHeight) top = y - rect.height - pad;
    el.style.left = Math.max(4, left) + "px";
    el.style.top = Math.max(4, top) + "px";
  }
  function hide_tooltip() {
    const el = document.getElementById("apn-tooltip");
    if (el) el.style.display = "none";
  }

  /** 1つの問題リンクにマーカーとイベントを適用する。 */
  function decorate(anchor) {
    const id = id_from_href(anchor.getAttribute("href") || anchor.href);
    if (!id) return;
    const p = DATA.problems[id];
    if (!p) return;

    const cell = anchor.closest("td") || anchor;
    cell.classList.add("apn-cell");
    // solved マーカー
    for (const c of Object.values(SOLVED_CLASS)) cell.classList.remove(c);
    const solved = p.latest && p.latest.solved;
    const cls = SOLVED_CLASS[solved];
    if (cls) cell.classList.add(cls);
    // 最新挑戦日からの経過に応じた濃さでセル(td)全体を塗る（時間で退色）。
    // <a> は塗らない（塗ると文字周りだけ二重に濃くなり箱状に見えるため）。
    const bg = bg_color(solved, p.latest);
    if (bg) cell.style.setProperty("background-color", bg, "important");
    else cell.style.removeProperty("background-color");
    cell.dataset.apnId = id;
    cell.dataset.apnTags = (p.tags || []).join("|");

    if (!cell.dataset.apnBound) {
      cell.dataset.apnBound = "1";
      cell.addEventListener("mouseenter", (e) => show_tooltip(p, e.clientX, e.clientY));
      cell.addEventListener("mousemove", (e) => {
        const el = document.getElementById("apn-tooltip");
        if (el && el.style.display === "block") position_tooltip(el, e.clientX, e.clientY);
      });
      cell.addEventListener("mouseleave", hide_tooltip);
    }
    apply_filter_to_cell(cell);
  }

  function apply_filter_to_cell(cell) {
    if (!selected_tags.length) {
      cell.classList.remove("apn-dim", "apn-hit");
      return;
    }
    const tags = (cell.dataset.apnTags || "").split("|").filter(Boolean);
    const hit = selected_tags.some((t) => tags.includes(t));
    cell.classList.toggle("apn-hit", hit);
    cell.classList.toggle("apn-dim", !hit);
  }

  function decorate_all() {
    if (!DATA) return;
    document.querySelectorAll('a[href*="/tasks/"]').forEach(decorate);
  }

  function reapply_filter() {
    document.querySelectorAll("td.apn-cell").forEach(apply_filter_to_cell);
  }

  // SPA なので DOM 変化を監視して再適用（デバウンス）
  let timer = null;
  function schedule() {
    clearTimeout(timer);
    timer = setTimeout(decorate_all, 200);
  }

  async function init() {
    try {
      const res = await fetch(chrome.runtime.getURL("data.json"));
      DATA = await res.json();
    } catch (e) {
      console.error("[apn] data.json 読み込み失敗", e);
      return;
    }
    const st = await chrome.storage.local.get(["selectedTags", "showDetails"]);
    selected_tags = st.selectedTags || [];
    show_details = !!st.showDetails;

    decorate_all();
    new MutationObserver(schedule).observe(document.body, { childList: true, subtree: true });

    chrome.storage.onChanged.addListener((changes, area) => {
      if (area !== "local") return;
      if (changes.selectedTags) {
        selected_tags = changes.selectedTags.newValue || [];
        reapply_filter();
      }
      if (changes.showDetails) {
        show_details = !!changes.showDetails.newValue;
      }
    });
  }

  init();
})();
