#pragma once
/**
 * @brief 素因数分解（試し割り）
 *
 *   prime_factorize(n) … {素因数: 指数} の map  O(√n)
 *
 * @note n >= 1。n = 1 なら空の map
 *
 * @code
 * for(auto [p, e] : prime_factorize(12)) ...  // (2, 2), (3, 1)
 * @endcode
 */
map<ll,ll> prime_factorize(ll n){
    ll a = 2;
    map<ll,ll> ret;
    if(n == 1) return ret;
    while(a*a <= n){
        if(n%a == 0){
            n /= a;
            ret[a]++;
        }
        else{
            a++;
        }
    }
    ret[n]++;
    return ret;
}
