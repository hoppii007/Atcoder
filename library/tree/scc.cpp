/**
 * @brief 強連結成分分解（Kosaraju 法）
 *
 * 有向グラフを強連結成分（SCC）に分解し、各頂点に SCC ID を振った配列を返す。
 * ID はトポロジカル順で 0 から振られる（同じ SCC 内の頂点は同じ ID、
 * SCC u → SCC v に辺があるなら id(u) < id(v)）。
 *
 * 計算量: O(V + E)
 *
 * @param nexts nexts[i] := 頂点 i から有向辺で行ける頂点のリスト
 * @return ret[i] := 頂点 i が属する SCC の ID（0-indexed、トポロジカル順）
 *
 * @code
 * vvll nexts(n);
 * for(auto& [u, v] : edges) nexts[u].push_back(v);
 * vll ids = scc(nexts);
 * // ids[i] == ids[j] ⇔ i と j は同じ SCC
 * @endcode
 */
vll scc(vvll &nexts){
    ll n = nexts.size();

    // 最終的に SCC ID を格納する配列（未割当は -1）
    vll scc_ids(n, -1);

    // 逆辺グラフを作成（2 回目の DFS で使う）
    vvll inv_nexts(n);
    rep(i, n){
        for(ll next: nexts[i]){
            inv_nexts[next].push_back(i);
        }
    }

    // 1 回目の DFS: 元のグラフで帰りがけ順を記録
    ll order = 0;
    vll orders(n);
    vb visited(n, false);
    std::function<void(ll)> dfs1 = [&](ll now){
        visited[now] = true;
        for(ll next: nexts[now]){
            if(visited[next]) continue;
            dfs1(next);
        }
        orders[now] = order++;
    };
    rep(i, n) if(!visited[i]) dfs1(i);

    // 2 回目の DFS: 逆辺グラフで、帰りがけ順が大きい頂点から訪問
    ll scc_id = 0;
    std::function<void(ll)> dfs2 = [&](ll now){
        scc_ids[now] = scc_id;
        for(ll next: inv_nexts[now]){
            if(scc_ids[next] != -1) continue;
            dfs2(next);
        }
    };

    // {帰りがけ順, 頂点番号} を降順ソート
    vpll order_ids(n);
    rep(i, n) order_ids[i] = {orders[i], i};
    sort(all(order_ids));
    reverse(all(order_ids));

    rep(i, n) if(scc_ids[order_ids[i].second] == -1){
        dfs2(order_ids[i].second);
        scc_id++;
    }
    return scc_ids;
}
