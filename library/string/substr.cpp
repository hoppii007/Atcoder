
// 部分文字列を切り出す
// 半開区間[i,j)でとる
// 配列外になるところは無視
string substr(string s, ll i, ll j){
    string t = "";
    if(j <= i) return t;
    rep(ii, max(0LL,i), min((ll)s.size(),j)) t += string(1,s[ii]);
    return t;
}
