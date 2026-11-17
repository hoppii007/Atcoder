#pragma once
/**
 * @brief オーバーフローしない a と b*c の大小比較（__int128 で計算）
 *
 *   lte(a, b, c) … a <= b*c
 *   lt(a, b, c)  … a <  b*c
 *   gte(a, b, c) … a >= b*c
 *   gt(a, b, c)  … a >  b*c
 *
 *
 * @code
 * if(lte(x, n, m)) ...  // x <= n*m（n*m が 10^36 でも OK）
 * @endcode
 */
bool lte(ll a, ll b, ll c){
    return (__int128)a <= (__int128)b * c;
}
bool lt(ll a, ll b, ll c){
    return (__int128)a < (__int128)b * c;
}
bool gte(ll a, ll b, ll c){
    return (__int128)a >= (__int128)b * c;
}
bool gt(ll a, ll b, ll c){
    return (__int128)a > (__int128)b * c;
}
