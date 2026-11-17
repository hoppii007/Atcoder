#pragma once
#include "library/tree/union_find.cpp"
/**
 * @brief クラスカル法で最小全域木（非連結なら最小全域森）を求める  O(E log E)
 *
 *   auto [cost, tree] = kruskal(nexts)
 *     cost … 採用した辺のコスト合計
 *     tree … 最小全域木の隣接リスト tree[u] = {cost, v}（無向。両向きに入る）
 *
 * @tparam T コストの型（ll / double など）
 * @param nexts 隣接リスト nexts[u] = {cost, v}。無向グラフは両向きに入っていてよい
 *
 * @code
 * vvpll nexts(n);
 * nexts[0].pb({3, 1}); nexts[1].pb({3, 0});
 * auto [cost, tree] = kruskal(nexts);
 * @endcode
 */
template<typename T>
pair<T, vector<vector<pair<T, ll>>>> kruskal(const vector<vector<pair<T, ll>>> &nexts){
    ll n = nexts.size();
    vector<tuple<T, ll, ll>> edges;  // {cost, u, v}
    for(ll u = 0; u < n; u++) for(auto &[c, v]: nexts[u]) edges.emplace_back(c, u, v);
    sort(edges.begin(), edges.end());

    union_find uf(n);
    T cost = 0;
    vector<vector<pair<T, ll>>> tree(n);
    for(auto &[c, u, v]: edges){
        if(uf.is_same(u, v)) continue;
        uf.unite(u, v);
        cost += c;
        tree[u].push_back({c, v});
        tree[v].push_back({c, u});
    }
    return {cost, tree};
}
