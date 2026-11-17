#pragma once
/**
 * @brief 木の直径
 *
 *   tree_diameter(nexts)                   … 木の直径（辺数）  O(N)
 *   tree_longest_distance_from_root(nexts, s) … s から最も遠い頂点の {距離, 頂点番号}  O(N)
 *
 * @code
 * ll d = tree_diameter(nexts);
 * @endcode
 */
pll tree_longest_distance_from_root(vvll &nexts, ll now, ll prev=-1){
    ll ret = 0;
    ll reti = now;
    for(ll next: nexts[now]) if(next != prev){
        pll nl = tree_longest_distance_from_root(nexts, next, now);
        if(nl.first+1 >= ret){
            ret = nl.first+1;
            reti = nl.second;
        }
    }
    return {ret, reti};
}
ll tree_diameter(vvll &nexts){
    pll p1 = tree_longest_distance_from_root(nexts, 0);
    pll p2 = tree_longest_distance_from_root(nexts, p1.snd);
    return p2.fst;
}
