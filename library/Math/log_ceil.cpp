#pragma once
/**
 * @brief 整数版 log の切り上げ
 *
 *   log_ceil(n, a) … a^b >= n となる最小の b  O(log n)
 *
 * @note a >= 2。n <= 1 なら 0
 *
 * @code
 * ll b = log_ceil(9, 2);  // 4（2^4 = 16 >= 9）
 * @endcode
 */
ll log_ceil(ll n, ll a){
	ll ret = 0;
	ll now = 1;
	while(now < n){
		ret++;
		if(now > (n - 1) / a) break;  // now * a >= n（掛けるとオーバーフローし得る）
		now *= a;
	}
	return ret;
}