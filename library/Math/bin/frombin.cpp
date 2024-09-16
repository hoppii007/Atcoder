
//右が小さいことにする
//つまり、001 -> 1となる
//逆にしたい場合はreverseしてから渡してね
ll frombin(string s){
    //s = "1"+s+"1";
    ll n = s.size();
    ll ret = 0;
    ll now = 1;
    reverse(all(s));
    rep(i,n){
        if(s[i] == '1') ret+=now;
        now*=2;
    }
    return ret;
}