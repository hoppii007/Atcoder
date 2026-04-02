
//約数の個数はだいたいO(N^1/3)らしい
//計算自体はO(N^1/2)
vll divisors(ll n){
    vll ret;
    for(ll i=1;i*i<=n;i++) if(n%i == 0){
        ret.push_back(i);
        if(i*i != n) ret.push_back(n/i);
    }
    sort(all(ret));
    return ret;
}
