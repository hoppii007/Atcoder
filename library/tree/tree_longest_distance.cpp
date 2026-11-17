#pragma once
/**
 * @brief 木で最も遠い頂点
 *
 *   longest_distance_from_root(nexts, s) … s から最も遠い頂点の {距離, 頂点番号}（複数あればどれか 1 つ）  O(N)
 *
 * @code
 * auto [d, far] = longest_distance_from_root(nexts, 0);
 * @endcode
 */
pll longest_distance_from_root(vvll &nexts, ll now, ll prev=-1){
    ll ret = 0;
    ll reti = now;
    for(ll next: nexts[now]) if(next != prev){
        pll nl = longest_distance_from_root(nexts, next, now);
        if(nl.first+1 >= ret){
            ret = nl.first+1;
            reti = nl.second;
        }
    }
    return {ret, reti};
}
