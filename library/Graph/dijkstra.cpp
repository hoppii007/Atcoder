#pragma once
/**
 * @brief ダイクストラ（非負辺の単一始点最短距離）
 *
 *   dijkstra(nexts, starts) … starts からの最短距離（到達不能は inf）  O((V+E) log V)
 *
 * @param nexts 隣接リスト nexts[u] = {cost, v}
 * @note 負の辺は不可。多始点は starts にまとめて 1 回で呼ぶ
 *
 * @code
 * vvpll nexts(n);
 * nexts[u].pb({c, v});
 * vll dist = dijkstra(nexts, {0});
 * @endcode
 */
vll dijkstra(vector<vpll> &nexts, vll starts){
    struct dijk_comp{
        bool operator()(const pll &p1,const pll &p2) const{
            return p1.first > p2.first;
        }
    };
    vll dijk(nexts.size(), inf);
    priority_queue<pll,vector<pll>,dijk_comp> q;
    for(ll s: starts){
        dijk[s] = 0;
        q.push({0,s});
    }
    while(!q.empty()){
        pll now = q.top();
        q.pop();
        //すでに更新されていれば無視
        if(now.first > dijk[now.second]) continue;
        for(pll next: nexts[now.second]){
            ll ncost = next.first + now.first;
            if(dijk[next.second] > ncost){
                dijk[next.second] = ncost;
                q.push({ncost, next.second});
            }
        }
    }
    return dijk;
}
