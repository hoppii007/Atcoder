#pragma once
/**
 * @brief 立っているビットの数
 *
 *   popcount(n) … n を2進数で表したときの 1 の個数  O(log n)
 *
 * @note n < 0 のときは 0 を返す
 *
 * @code
 * ll c = popcount(13);  // 13 = 1101 → 3
 * @endcode
 */
ll popcount(ll n){
    ll ret = 0;
    while(n > 0){
        ret += n%2;
        n >>= 1LL;
    }
    return ret;
}
