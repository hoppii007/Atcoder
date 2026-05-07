
/**
 * @brief 素因数分解
 * @param n 素因数分解する正の整数（n >= 1）
 * @return 素因数とその指数の map（例: 12 → {{2,2},{3,1}}）
 * @note 計算量: O(√n)
 * @note n = 1 の場合は空の map を返す
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
