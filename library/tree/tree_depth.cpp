#pragma once
/**
 * @brief 根からの深さ
 *
 *   tree_depth(root, nexts) … d[v] := root から v までの辺数（到達不能は inf）  O(N)
 *
 * @code
 * vll d = tree_depth(0, nexts);
 * @endcode
 */
vll tree_depth(ll root, vvll &nexts){
    vll ret(nexts.size(), inf);
    std::function<void(ll,ll)> sub_dfs = [&](ll now, ll d){
        ret[now] = d;
        for(ll next: nexts[now]) if(ret[next] == inf) sub_dfs(next, d+1);
    };
    sub_dfs(root, 0);
    return ret;
}
