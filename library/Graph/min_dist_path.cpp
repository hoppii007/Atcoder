#pragma once
// sからgまでの、最短経路を一つ求め、返す
#include "library/Graph/dijkstra.cpp"
// 辿り着けない場合、空配列を返す
// O(V+E + (V+E)logV)
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
