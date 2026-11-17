#pragma once
/**
 * @brief 有向グラフのサイクルを 1 つ取得
 *
 *   find_cycle(nexts, root) … root から到達できるサイクルの頂点列（無ければ空）  O(V+E)
 *
 * @note 返り値は cycle[0] → cycle[1] → … → cycle.back() → cycle[0] の順
 * @note root から到達できないサイクルは見つからない
 *
 * @code
 * vvll nexts(n);
 * vll cycle = find_cycle(nexts, 0);
 * @endcode
 */
vll find_cycle(vvll &nexts, ll root){
    vpll ret(nexts.size(), {-1,-1});
    ll order = 0;
    vll cycle;
    std::function<ll(ll)> dfs = [&](ll now){
        ret[now].fst = order++;
        for(ll next: nexts[now]){
            if(ret[next].fst == -1){
                ll result = dfs(next);
                //ループを見つけて帰ってきた時
                if(result != -1){
                    //ループ元に辿り着いてなければ自分もループの仲間
                    if(result != inf) cycle.push_back(now);
                    if(result == now) return inf;
                    else return result;
                }
            }
            else{
                //きたことはあって終わってないところに来たらループ！
                if(ret[next].snd == -1){
                    cycle.push_back(now);
                    return next;
                }
            }
        }
        ret[now].snd = order;
        return -1LL;
    };
    dfs(root);
    //最後についたところから保存されるので、ループ順にするには逆にしないといけない
    reverse(all(cycle));
    return cycle;
}