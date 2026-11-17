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
