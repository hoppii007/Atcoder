
//座標圧縮した後の座標が返ってくる
//元の数字 -> 圧縮後の座標が取れるmap
map<ll,ll> zaatu(vll &a){
    set<ll> s;
    rep(i,a.size()) s.insert(a[i]);
    map<ll,ll> ret;
    ll i = 0;
    for(ll t: s) ret[t] = i++;
    return ret;
}
