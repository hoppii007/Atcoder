#pragma once
/**
 * @brief 0-1 ナップサック（重さ添字版 / 価値添字版）
 *
 *   knapsack_max_value(w, v, cap) … dp[i][j] = 先頭 i 個（品 0..i-1）まで考えて、重さの合計を j 以下にするときの価値の最大
 *                                   i = 0..N, j = 0..cap。O(N · cap)。cap が小さい（〜10^7 / N）ときに使う
 *   knapsack_min_weight(w, v)     … dp[i][j] = 先頭 i 個（品 0..i-1）まで考えて、価値の合計を j 以上にするときの重さの最小
 *                                   i = 0..N, j = 0..Σv。O(N · Σv)。重さが巨大で価値の総和が小さいときに使う
 *
 * @param w w[i] = 品 i の重さ（≥ 0）
 * @param v v[i] = 品 i の価値（≥ 0）
 * @param cap 容量（≥ 0）
 * @note 重さ・価値のうち、総和（値域）が小さい方を添字にする。
 *       行 i は「先頭 i 個まで」なので N+1 行。全部考えた結果は dp[N]（dp[n-1] ではない）。
 *       初期値は「1 個も考えていない」行 dp[0] だけで決まる:
 *         knapsack_max_value  … dp[0][j] = 0（何も入れなければ価値 0、どの容量でも可能）
 *         knapsack_min_weight … dp[0][0] = 0、dp[0][j] = inf（j ≥ 1。何も入れずに価値 j 以上は不可能）
 *       「以上」は遷移で dp[i][max(0, j − v_i)] を見ることで表す（ちょうど版との違いはここだけ）。
 *       どちらの dp も各行で j について単調非減少。
 *       「容量 H で取れる価値の最大」は、dp[N][j] ≤ H を満たす最大の j。
 *       メモリは (N+1)·(値域+1) 個の ll。最後の行しか要らないなら 1 次元化（j 降順）で O(値域) にできる。
 *
 * @code
 * // 例: abc317_d（負けている区 i を取るコスト w_i = ceil((Y_i − X_i)/2)、議席 v_i = Z_i）
 * // need = 追加で必要な議席数
 * vvll dp = knapsack_min_weight(w, v);
 * cout << (need <= 0 ? 0 : dp[w.size()][need]) << endl;
 *
 * // 例: 容量 cap で詰め込める価値の最大
 * vvll dp = knapsack_max_value(w, v, cap);
 * cout << dp[w.size()][cap] << endl;
 * @endcode
 */
vvll knapsack_max_value(const vll &w, const vll &v, ll cap){
    ll n = w.size();
    vvll dp(n+1, vll(cap+1, 0));
    rep(i, n) rep(j, cap+1){
        dp[i+1][j] = dp[i][j];  // 品 i を入れない
        if(j >= w[i]) chmax(dp[i+1][j], dp[i][j-w[i]] + v[i]);  // 品 i を入れる
    }
    return dp;
}

vvll knapsack_min_weight(const vll &w, const vll &v){
    ll n = w.size();
    ll sum_v = 0;
    rep(i, n) sum_v += v[i];
    vvll dp(n+1, vll(sum_v+1, inf));
    dp[0][0] = 0;
    rep(i, n) rep(j, sum_v+1){
        dp[i+1][j] = dp[i][j];  // 品 i を入れない
        ll from = max(0LL, j-v[i]);  // 品 i を入れる（価値 v_i 以上分はまとめて 0 から）
        if(dp[i][from] < inf) chmin(dp[i+1][j], dp[i][from] + w[i]);
    }
    return dp;
}
