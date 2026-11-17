#pragma once
/**
 * @brief DFS（到達可能な頂点に印を付ける）
 *
 *   dfs(nexts, visit, root) … root から到達できる頂点の visit を true にする  O(V+E)
 *
 * @note visit は全 false で作り、visit[root] = true にしてから呼ぶ
 *
 * @code
 * vvll nexts(n);
 * vb visit(n, false);
 * visit[0] = true;
 * dfs(nexts, visit, 0);
 * @endcode
 */
void dfs(vvll &nexts, vb &visit, ll now, ll prev = -1){
    for(ll next: nexts[now]) {
        if(next == prev) continue;
        if(visit[next]) continue;
        visit[next] = true;
        dfs(nexts, visit, next, now);
    }
}
