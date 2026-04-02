
//素因数分解
//O(√n)
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
