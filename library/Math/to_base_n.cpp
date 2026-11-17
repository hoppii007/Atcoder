#pragma once
/**
 * @brief n 進数変換（下の桁から順に並ぶ）
 *
 *   toNsinsu(x, n) … x の n 進表記の各桁。ret[i] が n^i の位
 *
 * @note x >= 0, n >= 2。x = 0 なら {0}
 *
 * @code
 * vll d = toNsinsu(6, 2);  // {0, 1, 1}
 * @endcode
 */
vll toNsinsu(ll x, ll n) {
	if (x == 0) return {0};
	ll cnt = 0;
	ll xx = x;
	while (xx > 0) {
		xx /= n;
		cnt++;
	}
	vll ret(cnt, 0);
	rep(i, cnt) {
		ret[i] = x%n;
		x /= n;	}
	return ret;
}