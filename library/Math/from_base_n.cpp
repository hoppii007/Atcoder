/**
 * @brief n進数表記された配列を10進数に変換する
 * @param n 基数（2以上の整数）
 * @param a n進数の各桁を格納した配列。a[i] は n^i の位（下位桁が先頭）
 * @return 10進数に変換した値
 * @note 計算量: O(a.size())
 * @note 例: a = [0,1,1], n = 2 のとき、0*2^0 + 1*2^1 + 1*2^2 = 6
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