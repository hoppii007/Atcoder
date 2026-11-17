#pragma once
#include "library/tree/union_find.cpp"
/**
 * @brief 連結成分ごとに重み付き隣接リストを分割
 *
 *   split_connected_graph(nexts) … ret[c] = c 番目の成分の隣接リスト {cost, v}  O(V α(V) + E)
 *
 * @param nexts 隣接リスト nexts[u] = {cost, v}
 * @note 成分内の頂点番号は元の番号の昇順に 0, 1, 2, … と振り直す
 * @note 成分の順は各成分の最小頂点番号の昇順。連結性は辺の向きを無視して判定
 *
 * @code
 * vvpll nexts(n);
 * vector<vvpll> comps = split_connected_graph(nexts);
 * @endcode
 */
vector<vvpll> split_connected_graph(vvpll& nexts){
    ll n = nexts.size();
    union_find uf(n);
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
