#pragma once
/**
 * @brief 繰り返し二乗法による a^b mod
 *
 *   mpow(a, b) … a^b mod (グローバルの mod)  O(log b)
 *
 * @note b >= 0。a は負や mod 以上でもよい
 *
 * @code
 * ll x = mpow(2, 10);  // 1024
 * @endcode
 */
ll mpow(ll a, ll b){
    a %= mod;
    if(a < 0) a += mod;
    if(b == 0) return 1;
    if(b == 1) return a % mod;
    if(b % 2 == 0) return mpow((a*a) % mod, b/2);
    else return (a % mod) * mpow((a*a) % mod, (b-1)/2) % mod;
}
