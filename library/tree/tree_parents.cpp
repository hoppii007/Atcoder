#pragma once
/**
 * @brief 根付き木の親
 *
 *   tree_parents(root, nexts) … p[v] := v の親（root は -1、到達不能は inf）  O(N)
 *
 * @code
 * vll p = tree_parents(0, nexts);
 * @endcode
 */
vll tree_parents(ll root, vvll &nexts){
    vll ret(nexts.size(), inf);
    std::function<void(ll,ll)> sub_dfs = [&](ll now, ll parent){
        ret[now] = parent;
        for(ll next: nexts[now]) if(ret[next] == inf) sub_dfs(next, now);
    };
    //親の親を-1にしたりしたいときはここの第二引数を変更
    //infにするときは注意
    sub_dfs(root, -1);
    return ret;
}
