

/**
 * @brief a と b の最小公倍数を求める
 * @param a 正整数
 * @param b 正整数
 * @return lcm(a, b)
 * @note 計算量: O(log min(a, b))
 * @note (a / gcd(a, b)) * b を計算するため、結果が ll の範囲を超えるとオーバーフローする
 */
ll lcm(ll a,ll b){
    return a / gcd(a,b) * b;
}

