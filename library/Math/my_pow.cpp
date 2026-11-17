#pragma once
/**
 * @brief 繰り返し二乗法による a^b（mod なし）
 *
 *   mypow(a, b) … a^b  O(log b)
 *
 * @note b >= 0。途中の a*a も含め ll を超えるとオーバーフローする
 *
 * @code
 * ll x = mypow(3, 4);  // 81
 * @endcode
 */
ll mypow(ll a,ll b){
    if(b == 0) return 1;
    if(b == 1) return a;
    if(b%2 == 0) return mypow(a*a,b/2);
    else return a*mypow(a*a,(b-1)/2);
}
