/** ポップアップ: solved 統計の表示と、タグによる Table 強調フィルタ。 */
(async () => {
  "use strict";

  const res = await fetch(chrome.runtime.getURL("data.json"));
  const data = await res.json();
  const problems = Object.values(data.problems);

  // --- 統計 (各問題の最新回 solved で集計) ---
  const counts = { 自力AC: 0, 解説AC: 0, 未AC: 0 };
  for (const p of problems) {
    const s = p.latest && p.latest.solved;
    if (s && counts[s] != null) counts[s]++;
  }
  document.getElementById("stats").innerHTML = `
    <span class="chip total">計 ${problems.length}</span>
    <span class="chip self">自力 ${counts.自力AC}</span>
    <span class="chip editorial">解説 ${counts.解説AC}</span>
    <span class="chip unsolved">未 ${counts.未AC}</span>
  `;
  document.getElementById("gen").textContent =
    "生成: " + (data.generatedAt || "").replace("T", " ").slice(0, 16);

  // --- タグ一覧 (件数降順→名前順) ---
  const tag_entries = Object.entries(data.tags)
    .map(([name, ids]) => ({ name, count: ids.length }))
    .sort((a, b) => b.count - a.count || a.name.localeCompare(b.name, "ja"));

  const list = document.getElementById("taglist");
  const st = await chrome.storage.local.get(["selectedTags", "showDetails"]);
  let selected = new Set(st.selectedTags || []);

  function render(filter = "") {
    list.innerHTML = "";
    for (const t of tag_entries) {
      if (filter && !t.name.toLowerCase().includes(filter.toLowerCase())) continue;
      const li = document.createElement("li");
      if (selected.has(t.name)) li.classList.add("on");
      li.innerHTML = `<span class="name"></span><span class="count">${t.count}</span>`;
      li.querySelector(".name").textContent = t.name;
      li.addEventListener("click", () => toggle(t.name));
      list.appendChild(li);
    }
  }

  async function toggle(name) {
    if (selected.has(name)) selected.delete(name);
    else selected.add(name);
    await chrome.storage.local.set({ selectedTags: [...selected] });
    render(document.getElementById("search").value);
  }

  // 詳細表示トグル
  const details_box = document.getElementById("showDetails");
  details_box.checked = !!st.showDetails;
  details_box.addEventListener("change", async () => {
    await chrome.storage.local.set({ showDetails: details_box.checked });
  });

  document.getElementById("search").addEventListener("input", (e) => render(e.target.value));
  document.getElementById("clear").addEventListener("click", async () => {
    selected = new Set();
    await chrome.storage.local.set({ selectedTags: [] });
    render(document.getElementById("search").value);
  });

  render();
})();
