#pragma once
/**
 * @brief 2進数文字列 → 整数
 *
 *   frombin(s) … '0'/'1' の文字列 s を整数に変換して返す  O(|s|)
 *
 * @note 右端が最下位ビット（"001" → 1）。左端を最下位にしたいなら reverse してから渡す
 *
 * @code
 * ll x = frombin("110");  // 6
 * @endcode
 */
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