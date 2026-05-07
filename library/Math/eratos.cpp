
/**
 * @brief エラトステネスの篩
 * @param n 上限値（n 以下の素数を列挙する）
 * @return n 以下の素数を昇順に格納した vector
 * @note 計算量: O(n log log n)
 */
vll eratos(ll n){
    vb isPrime(n+1,true);
    isPrime[0] = false;
    isPrime[1] = false;
    for(ll i = 2;i*i<=n;++i){
        if(isPrime[i]){
            for(ll j = i+i;j<=n;j+=i){
                isPrime[j] = false;
            }
        }
    }
    vll ret;
    rep(i,n+1){
        if(isPrime[i]) ret.push_back(i);
    }
    return ret;
}
