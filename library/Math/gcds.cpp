/**
 * @brief 配列 a の全要素の最大公約数を求める
 * @param a 正整数の配列（要素はすべて 0 より大きいこと）
 * @return gcd(a[0], a[1], ..., a[n-1])
 * @note 計算量: O(n log max(a))
 */
ll gcds(vll a){
    ll ret = a[0];
    rep(i,a.size()){
        ret = gcd(ret,a[i]);
    }
    return ret;
}
