#pragma once
/**
 * @brief 約数列挙
 *
 *   divisors(n) … n の約数を昇順で返す  O(√n)
 *
 * @note n >= 1。約数の個数はおおよそ O(n^(1/3))
 *
 * @code
 * vll d = divisors(12);  // 1 2 3 4 6 12
 * @endcode
 */
vll divisors(ll n){
    vll ret;
    for(ll i=1;i*i<=n;i++) if(n%i == 0){
        ret.push_back(i);
        if(i*i != n) ret.push_back(n/i);
    }
    sort(all(ret));
    return ret;
}