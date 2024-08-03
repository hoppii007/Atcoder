
//dp[i][i] = 0
//重みkの辺があるa -> bについては dp[a][b] = k;
rep(k,n) rep(i,n) rep(j,n){
	dp[i][j] = min(dp[i][j],dp[i][k]+dp[k][j]);
}