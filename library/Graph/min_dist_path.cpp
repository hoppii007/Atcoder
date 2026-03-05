
// sからgまでの、最短経路を一つ求め、返す
// depends: [dijkstra]
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
        pll nxt = {inf, -1};
        for(pll cand: nexts[s]) chmin(nxt, {dijk[cand.snd], cand.snd});
        s = nxt.snd;
        ret.pb(s);
    }
    return ret;
}
