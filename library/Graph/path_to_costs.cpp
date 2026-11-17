#pragma once
/**
 * @brief パスの合計コスト
 *
 *   path_to_costs(nexts, path) … path[0] → path[1] → … → path.back() の辺コストの合計  O(V+E)
 *
 * @param nexts 隣接リスト nexts[u] = {cost, v}
 * @note 同じ u → v の辺が複数あると全部足される
 *
 * @code
 * vvpll nexts(n);
 * vll path = {0, 2, 1};
 * ll cost = path_to_costs(nexts, path);
 * @endcode
 */
ll path_to_costs(vvpll& nexts, vll& path){
    ll ret = 0;
    rep(i, (ll)path.size()-1) {
        for(pll p: nexts[path[i]]) if(p.snd == path[i+1]) ret += p.fst;
    }
    return ret;
}
