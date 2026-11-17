#pragma once
/**
 * @brief エラトステネスの篩
 *
 *   eratos(n) … n 以下の素数を昇順で返す  O(n log log n)
 *
 * @code
 * vll ps = eratos(20);  // 2 3 5 7 11 13 17 19
 * @endcode
 */
vll eratos(ll n){
    if(n < 2) return {};
    vb is_prime(n+1,true);
    is_prime[0] = false;
    is_prime[1] = false;
    for(ll i = 2;i*i<=n;++i){
        if(is_prime[i]){
            for(ll j = i+i;j<=n;j+=i){
                is_prime[j] = false;
            }
        }
    }
    vll ret;
    rep(i,n+1){
        if(is_prime[i]) ret.push_back(i);
    }
    return ret;
}
