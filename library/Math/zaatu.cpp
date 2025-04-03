
//座標圧縮した後の座標が返ってくる
map<ll,ll> zaatu(vll &a){
    set<ll> s;
    rep(i,a.size()) s.insert(a[i]);
    map<ll,ll> ret;
    ll i = 0;
    for(ll t: s) ret[t] = i++;
    return ret;
}
