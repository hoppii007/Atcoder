
ll popcount(ll n){
    ll ret = 0;
    while(n > 0){
        ret += n%2;
        n >>= 1LL;
    }
    return ret;
}
