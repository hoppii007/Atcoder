#pragma once
/**
 * @brief ベルマンフォード（負辺ありの単一始点最短距離）
 *
 *   bellman_ford(nexts, starts) … starts からの最短距離（到達不能は inf）  O(VE)
 *
 * @param nexts 隣接リスト nexts[u] = {cost, v}
 * @note starts から負の閉路に到達できるときは値が不正確（検出は関数末尾の cnt == V で判定）
 *
 * @code
 * vvpll nexts(n);
 * nexts[u].pb({c, v});
 * vll dist = bellman_ford(nexts, {0});
 * @endcode
 */
vll bellman_ford(vvpll &nexts, vll starts){
    vll ret(nexts.size(), inf);
    for(ll s: starts) ret[s] = 0;
    ll cnt = 0;
    while(cnt < nexts.size()) {
        bool is_end = true;
        rep(i,nexts.size()){
            if(ret[i] == inf) continue;
            for(pll next: nexts[i]){
                if(ret[next.snd] > ret[i]+next.fst){
                    is_end = false;
                    ret[next.snd] = ret[i]+next.fst;
                }
            }
        }
        if(is_end) break;
        cnt++;
    }
    //ここでcnt == nexts.size()となっていれば負の閉路が存在する
    return ret;
}
