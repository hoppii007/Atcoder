
// n進数表記された配列を10進数に戻す
// [0,1,1], 2が渡されたとしたら、0*2^0 + 1*2^1 + 1*2^2
ll from_base_n(vll a, ll n){
    ll ret = 0;
    ll t = 1;
    rep(i, a.size()) {
        ret += t*a[i];
        t *= n;
    }
    return ret;
}
