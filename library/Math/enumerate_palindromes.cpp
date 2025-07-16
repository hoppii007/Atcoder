
//n以下の回文列挙（O(√N)）
//n <= 10^12
vll enumerate_palindromes(ll n){
    set<ll> se;
    string s = to_string(n);
    function<ll(ll,ll)> sub_pow = [&](ll a, ll b){
        if(b == 0) return 1LL;
        if(b == 1) return a;
        if(b%2 == 0) return sub_pow(a*a,b/2);
        else return a*sub_pow(a*a,(b-1)/2);
    };
    ll ten = sub_pow(10,s.size()/2);
    rep(i,10) if(i <= n) se.insert(i);
    rep(i,1,ten){
        string strL = to_string(i);
        string strR = to_string(i);
        reverse(all(strR));
        ll tmp = stoll(strL + strR);
        if(tmp > n) break;
        se.insert(stoll(strL + strR));
        rep(j,10) {
            tmp = stoll(strL + to_string(j) + strR);
            if(tmp > n) break;
            se.insert(tmp);
        }
    }

    vll ret;
    for(ll t: se) ret.push_back(t);
    return ret;
}
