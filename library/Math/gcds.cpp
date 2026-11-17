#pragma once
/**
 * @brief 配列全体の最大公約数
 *
 *   gcds(a) … gcd(a[0], a[1], ..., a[n-1])  O(n log max(a))
 *
 * @note 要素は正整数。a が空なら 0
 *
 * @code
 * ll g = gcds({12, 18, 30});  // 6
 * @endcode
 */
ll gcds(vll a){
    if(a.empty()) return 0;
    ll ret = a[0];
    rep(i,a.size()){
        ret = gcd(ret,a[i]);
    }
    return ret;
}
