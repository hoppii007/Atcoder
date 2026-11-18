#pragma once
/**
 * @brief 巡回セールスマン（bitDP）
 *
 *   tsp(dist, init) … dp[state][j] = 訪問集合が state で、最後に j にいるときの最小コスト（到達不能は inf）
 *                     O(2^N · N^2)、メモリ O(2^N · N)。N ≤ 18 程度まで
 *
 * @param dist dist[i][j] = 頂点 i から j への移動コスト（N×N、到達不能は inf）
 * @param init init[j] = 最初に j を訪れるときのコスト（= dp[1<<j][j]）。j から始められないなら inf
 * @note 始点を集合に含めたくない（S→お菓子 のように始点が数える対象でない）ときは、
 *       init[j] = 始点から j への距離 とする。始点も集合に含める普通の TSP なら
 *       init[s] = 0、他は inf とする。
 *       終点への戻り（巡回なら + dist[j][s]、ゴールがあるなら + ゴールまでの距離）は呼び出し側で足す。
 *       state は昇順に処理すれば部分集合が先に確定するので、ループで書ける（メモ化再帰は不要）。
 *
 * @code
 * // 例: abc301_e（S から出発し、お菓子をなるべく多く回って T 以内に G へ）
 * // d[i][j] = お菓子 i→j の距離、from_s[j] = S→お菓子 j、to_g[j] = お菓子 j→G
 * vvll dp = tsp(d, from_s);
 * ll ans = 0;
 * rep(state, 1LL << n) rep(j, n) if(dp[state][j] + to_g[j] <= t) chmax(ans, popcount(state));
 *
 * // 例: 頂点 0 から出発して全頂点を回り 0 に戻る最小コスト
 * vll init(n, inf); init[0] = 0;
 * vvll dp = tsp(d, init);
 * ll best = inf;
 * rep(j, n) chmin(best, dp[(1LL << n) - 1][j] + d[j][0]);
 * @endcode
 */
vvll tsp(const vvll &dist, const vll &init){
    ll n = init.size();
    vvll dp(1LL << n, vll(n, inf));
    rep(j, n) dp[1LL << j][j] = init[j];
    rep(state, 1LL << n) rep(j, n){
        // state に j が含まれない・到達不能なら、ここから配らない
        if(!(state & (1LL << j)) || dp[state][j] >= inf) continue;
        rep(k, n){
            if(state & (1LL << k)) continue;
            if(dist[j][k] >= inf) continue;
            ll nstate = state | (1LL << k);
            dp[nstate][k] = min(dp[nstate][k], dp[state][j] + dist[j][k]);
        }
    }
    return dp;
}
