#pragma once
/**
 * @brief トポロジカルソート
 *
 *   topological_sort(nexts) … 辺 u → v で u が v より前に来る頂点列  O(V+E)
 *
 * @note DAG 専用。閉路があると返り値の長さが V 未満になる
 *
 * @code
 * vvll nexts(n);
 * vll order = topological_sort(nexts);
 * @endcode
 */
vll topological_sort(vvll &nexts){
    vll in(nexts.size());
    rep(i,nexts.size()) for(ll next: nexts[i]) in[next]++;

    //入次数が0の集合
    queue<ll> s;
    rep(i,nexts.size()) if(in[i] == 0) s.push(i);

    vll ret;

    while(!s.empty()){
        ll now = s.front();
        s.pop();
        ret.push_back(now);

        for(ll next: nexts[now]) {
            in[next]--;
            if(in[next] == 0) s.push(next);
        }
    }

    return ret;
}

/**
 * @brief トポロジカル順序が一意に定まるかを判定する
 *
 *   is_unique_topological_order(nexts, order) … order（topological_sort の結果）が唯一の順序なら true  O(V+E)
 *
 * @note 一意 ⇔ order で隣り合うすべての組 (order[i], order[i+1]) に辺 order[i] → order[i+1] がある
 *       （どこかに辺が無ければ、その2頂点を入れ替えた別の順序も成り立つ）
 * @note 閉路があって order の長さが V 未満なら、順序が存在しないので false
 *
 * @code
 * vll order = topological_sort(nexts);
 * if(is_unique_topological_order(nexts, order)) { ... }
 * @endcode
 */
bool is_unique_topological_order(vvll &nexts, vll &order){
    if(order.size() != nexts.size()) return false;
    rep(i, (ll)order.size() - 1){
        bool found = false;
        for(ll next: nexts[order[i]]) if(next == order[i+1]) { found = true; break; }
        if(!found) return false;
    }
    return true;
}
