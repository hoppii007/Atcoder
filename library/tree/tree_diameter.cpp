#pragma once
/**
 * @brief 木の直径
 *
 *   treeDiameter(nexts)                   … 木の直径（辺数）  O(N)
 *   treeLongestDistanceFromRoot(nexts, s) … s から最も遠い頂点の {距離, 頂点番号}  O(N)
 *
 * @code
 * ll d = treeDiameter(nexts);
 * @endcode
 */
pll treeLongestDistanceFromRoot(vvll &nexts, ll now, ll prev=-1){
    ll ret = 0;
    ll reti = now;
    for(ll next: nexts[now]) if(next != prev){
        pll nl = treeLongestDistanceFromRoot(nexts, next, now);
        if(nl.first+1 >= ret){
            ret = nl.first+1;
            reti = nl.second;
        }
    }
    return {ret, reti};
}
ll treeDiameter(vvll &nexts){
    pll p1 = treeLongestDistanceFromRoot(nexts, 0);
    pll p2 = treeLongestDistanceFromRoot(nexts, p1.snd);
    return p2.fst;
}
