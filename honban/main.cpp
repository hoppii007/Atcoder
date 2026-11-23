#include "template.hpp"  // 先頭に置くこと（プリコンパイル済みヘッダが効く条件）
#include "library/Math/mod_ll.cpp"

int main(){
    cinll(n, m, k);
    // dp[i][j] := i番目まで考えて、合計がjになる通りの数
    vector<vector<modll>> dp(n, vector<modll>(k+1, 0));
    rep(i, 1, min(k, m)+1) dp[0][i] = 1;

    rep(i, 1, n) rep(j, 1, k+1) {
        // dp[i][j]を設定していく
        // i番目を1にするか、2にするか、...min(j-1, m)にするかのどれか
        rep(jj, 1, min(j, m+1)) dp[i][j] += dp[i-1][j-jj];
    }
    modll ans = 0;
    rep(i, k+1) ans += dp[n-1][i];
    cout << ans << endl;
    return 0;
}