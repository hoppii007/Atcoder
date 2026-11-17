#pragma once
/**
 * @brief ワーシャルフロイド（全点対最短距離）
 *
 *   warshall_floyd(nexts) … dp[i][j] = i から j への最短距離（到達不能は inf）  O(V^3)
 *
 * @param nexts 隣接リスト nexts[u] = {cost, v}
 * @note dp[i][i] は自己ループかサイクルが無い限り inf のまま（0 にはならない）
 *
 * @code
 * vvpll nexts(n);
 * vvll dp = warshall_floyd(nexts);
 * @endcode
 */
vvll warshall_floyd(vector<vpll> &nexts){
    vvll dp(nexts.size(), vll(nexts.size(),inf));
    rep(i,nexts.size()) for(pll p: nexts[i]) dp[i][p.second] = p.first;
    rep(k, nexts.size()) rep(i, nexts.size()) rep(j, nexts.size()) dp[i][j] = min(dp[i][j], dp[i][k] + dp[k][j]);
    return dp;
}
