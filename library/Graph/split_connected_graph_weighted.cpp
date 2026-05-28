/**
 * @brief 重み付き隣接リストを連結成分ごとに分割する（split_connected_graph の重み付き版）
 * @param nexts 重み付き隣接リスト（nexts[i] は頂点 i から出る辺の {コスト, 行き先} 一覧）。辺の向きは無視して連結性を判定する
 * @return 成分ごとの重み付き隣接リストの配列。頂点は成分内で 0-indexed に振り直され、各辺のコストはそのまま引き継がれる
 * @note 頂点番号の振り直し規則・成分の並び順・計算量は重みなし版と同じ
 */
vector<vvpll> split_connected_graph(vvpll& nexts){
    ll n = nexts.size();
    UnionFind uf(n);
    rep(i, n) for(auto& [cost, next]: nexts[i]){
        uf.unite(i, next);
    }
    vector<vvpll> ret(uf.size());
    // comp[i]      := 頂点 i が属する成分の番号
    // to_idx[i]    := 頂点 i の成分内でのローカル番号（成分内で昇順）
    // r_to_idx[r]  := root r -> 成分番号（未割り当ては -1）
    // local_cnt[r] := root r の成分で次に割り当てるローカル番号
    vll comp(n), to_idx(n), r_to_idx(n, -1), local_cnt(n, 0);
    ll cnt = 0;
    rep(i, n){
        ll r = uf.root(i);
        if(r_to_idx[r] == -1){
            r_to_idx[r] = cnt;
            ret[cnt].resize(uf.size(r));
            cnt++;
        }
        comp[i] = r_to_idx[r];
        // i を昇順に走査しているので、成分内でも昇順にローカル番号が振られる
        to_idx[i] = local_cnt[r]++;
    }
    rep(i, n) for(auto& [cost, next]: nexts[i]){
        ret[comp[i]][to_idx[i]].pb({cost, to_idx[next]});
    }
    return ret;
}
