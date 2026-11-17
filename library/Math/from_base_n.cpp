#pragma once
/**
 * @brief n 進数の桁配列 → 整数
 *
 *   from_base_n(n, a) … 桁配列 a を n 進数として読んだ値  O(|a|)
 *
 * @param a 各桁。a[i] が n^i の位（先頭が最下位桁）
 *
 * @code
 * vll a = {0, 1, 1};
 * ll x = from_base_n(2, a);  // 0*1 + 1*2 + 1*4 = 6
 * @endcode
 */
ll from_base_n(ll n, vll& a){
    ll ret = 0;
    ll t = 1;
    rep(i, a.size()) {
        ret += t*a[i];
        t *= n;
    }
    return ret;
}