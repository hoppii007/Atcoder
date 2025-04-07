
//O(log n)で、a^b <= nとなる最大のbを求める（logの切り捨て的な）
ll mylog(ll n, ll a){
    ll ret = 0;
    ll now = 1;
    while(now <= n){
        now *= a;
        ret++;
    }
    return ret-1;
}
