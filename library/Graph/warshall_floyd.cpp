
//nexts : {cost, id}
//N * Nのdpを返す
vvll warshall_floyd(vector<vpll> &nexts){
    vvll dp(nexts.size(), vll(nexts.size(),inf));
    rep(i,nexts.size()) for(pll p: nexts[i]) dp[i][p.second] = p.first;
    rep(k, nexts.size()) rep(i, nexts.size()) rep(j, nexts.size()) dp[i][j] = min(dp[i][j], dp[i][k] + dp[k][j]);
    return dp;
}
