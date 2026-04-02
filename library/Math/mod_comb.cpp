
// 1 <= k <= n < p <= 10^7
// 逆元の求め方が天才 https://drken1215.hatenablog.com/entry/2018/06/08/210000
const ll MAX = 10000000;
ll fac[MAX+1], inv[MAX+1], ifac[MAX+1];
void mod_comb_init(){
    fac[0] = fac[1] = 1;
    ifac[0] = ifac[1] = 1;
    inv[1] = 1;
    rep(i,2,MAX+1){
        fac[i] = fac[i-1]*i % mod;
        inv[i] = (-inv[mod%i]*(mod/i))%mod;
        ifac[i] = ifac[i-1]*inv[i] % mod;;
        while(ifac[i] < 0) ifac[i] += mod;
    }
}

ll mod_comb(ll n, ll k){
    if(n < k) return 0;
    if(n < 0 || k < 0) return 0;
    return fac[n]*ifac[k]%mod*ifac[n-k]%mod;
}
