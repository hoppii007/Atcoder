#pragma once
/**
 * @brief n 進数表記（下の桁から順に並べる）
 *
 *   to_base_n(a, n)  … ret[i] = a の n^i の位の数字  O(log_n a)
 *
 * @note a = 0 のときは空配列を返す。a ≥ 0, n ≥ 2 であること
 *
 * @code
 * vll d = to_base_n(6, 2);   // {0, 1, 1}（6 = 0·2^0 + 1·2^1 + 1·2^2）
 * @endcode
 */
vll to_base_n(ll a, ll n){
    vll ret;
    while(a > 0){
        ret.push_back(a%n);
        a /= n;
    }
    return ret;
}
