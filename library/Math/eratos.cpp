
//O(nloglogn)でn以下の素数リストを返す（昇順）
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
