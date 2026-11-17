#pragma once
/**
 * @brief 01-BFS（辺コストが 0 か 1 の最短距離）
 *
 *   bfs01(nexts, starts) … starts からの最短距離（到達不能は inf）  O(V+E)
 *
 * @param nexts 隣接リスト nexts[u] = {cost, v}（cost は 0 か 1）
 * @note 多始点は starts にまとめて 1 回で呼ぶ
 *
 * @code
 * vvpll nexts(n);
 * nexts[u].pb({c, v});
 * vll dist = bfs01(nexts, {0});
 * @endcode
 */
vll bfs01(vector<vpll> &nexts, vll starts){
    vll bfs01s(nexts.size(), inf);
    deque<pll> q;
    for(ll s: starts){
        bfs01s[s] = 0;
        q.push_front({0,s});
    }
    while(!q.empty()){
        pll now = q.front();
        q.pop_front();
        for(pll next: nexts[now.second]){
            ll ncost = bfs01s[now.second]+next.first;
            if(bfs01s[next.second] <= ncost) continue;
            bfs01s[next.second] = ncost;
            if(next.first == 0) q.push_front({ncost, next.second});
            else q.push_back({ncost, next.second});
        }
    }
    return bfs01s;
}
