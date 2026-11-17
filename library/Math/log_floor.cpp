#pragma once
/**
 * @brief 整数版 log の切り捨て
 *
 *   log_floor(n, a) … a^b <= n となる最大の b  O(log n)
 *
 * @note a >= 2, n >= 1
 *
 * @code
 * ll b = log_floor(9, 2);  // 3（2^3 = 8 <= 9）
 * @endcode
 */
ll log_floor(ll n, ll a){
    ll ret = 0;
    ll now = 1;
    while(now <= n / a){
        now *= a;
        ret++;
    }
    return ret;
}
