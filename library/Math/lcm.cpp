#pragma once
/**
 * @brief 最小公倍数
 *
 *   lcm(a, b) … a と b の最小公倍数  O(log min(a, b))
 *
 * @note 結果が ll を超えるとオーバーフローする
 *
 * @code
 * ll l = lcm(4, 6);  // 12
 * @endcode
 */
ll lcm(ll a,ll b){
    return a / gcd(a,b) * b;
}

