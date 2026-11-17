#pragma once
/**
 * @brief 文字列を大文字に変換
 *
 *   to_upper(s) … 英小文字を大文字にした文字列（他の文字はそのまま）  O(|s|)
 *
 * @code
 * string t = to_upper("AtCoder");  // "ATCODER"
 * @endcode
 */
string to_upper(string s){
    string ret = "";
    rep(i,s.size()){
        if('a' <= s[i] && s[i] <= 'z') ret += string(1, s[i] -'a' + 'A');
        else ret += string(1, s[i]);
    }
    return ret;
}
