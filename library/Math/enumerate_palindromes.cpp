#pragma once
/**
 * @brief 回文数の列挙
 *
 *   enumerate_palindromes(n) … 0 以上 n 以下の回文数を昇順で返す  O(√n log n)
 *
 * @note n <= 10^12
 *
 * @code
 * vll p = enumerate_palindromes(30);  // 0 1 ... 9 11 22
 * @endcode
 */
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
        string str_l = to_string(i);
        string str_r = to_string(i);
        reverse(all(str_r));
        ll tmp = stoll(str_l + str_r);
        if(tmp > n) break;
        se.insert(stoll(str_l + str_r));
        rep(j,10) {
            tmp = stoll(str_l + to_string(j) + str_r);
            if(tmp > n) break;
            se.insert(tmp);
        }
    }

    vll ret;
    for(ll t: se) ret.push_back(t);
    return ret;
}
