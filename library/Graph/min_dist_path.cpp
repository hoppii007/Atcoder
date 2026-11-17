#pragma once
#include "library/Graph/dijkstra.cpp"
/**
 * @brief s から g への最短経路を 1 つ復元
 *
 *   min_dist_path(nexts, s, g) … 最短経路の頂点列 s, …, g（到達不能なら空）  O((V+E) log V)
 *
 * @param nexts 隣接リスト nexts[u] = {cost, v}（非負辺）
 *
 * @code
 * vvpll nexts(n);
 * vll path = min_dist_path(nexts, 0, n-1);
 * @endcode
 */
vll min_dist_path(vvpll &nexts, ll s, ll g){
    vll ret(1, s);
    vvpll inexts(nexts.size());
    rep(i, nexts.size()) for(pll nxt: nexts[i]){
        inexts[nxt.snd].pb({nxt.fst, i});
    }
    vll dijk = dijkstra(inexts, {g});
    if(dijk[s] == inf) return vll();

    while(s != g){
        // 「辺のコスト + 行き先から g までの距離 == s から g までの距離」となる辺は最短路上にある
        for(pll cand: nexts[s]) if(dijk[cand.snd] != inf && cand.fst + dijk[cand.snd] == dijk[s]){
            s = cand.snd;
            break;
        }
        ret.pb(s);
    }
    return ret;
}
