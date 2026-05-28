/**
 * @brief 隣接リストを連結成分ごとの隣接リストに分割する
 * @param nexts 隣接リスト（nexts[i] は頂点 i から出る辺の行き先一覧）。辺の向きは無視して連結性を判定する
 * @return 成分ごとの隣接リストの配列。ret[c] は c 番目の成分の隣接リストで、頂点は成分内で 0-indexed に振り直される
 * @note 各成分内の頂点番号は、元グラフでの頂点番号の昇順に 0, 1, 2, ... と割り当てられる
 * @note 成分の並び順（ret の添字）は、その成分に含まれる元の最小頂点番号の昇順
 * @note 計算量: O(V α(V) + E)
 * @note 連結性は UnionFind による弱連結（辺の向きを無視）。強連結成分が必要な場合は別アルゴリズムを使うこと
 */
vector<vvll> split_connected_graph(vvll& nexts){
    ll n = nexts.size();
    UnionFind uf(n);
    rep(i, n) for(ll next: nexts[i]){
        uf.unite(i, next);
    }
    vector<vvll> ret(uf.size());
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
    rep(i, n) for(ll next: nexts[i]){
        ret[comp[i]][to_idx[i]].pb(to_idx[next]);
    }
    return ret;
}
