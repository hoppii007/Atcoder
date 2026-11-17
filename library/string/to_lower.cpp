#pragma once
/**
 * @brief 文字列を小文字に変換
 *
 *   to_lower(s) … 英大文字を小文字にした文字列（他の文字はそのまま）  O(|s|)
 *
 * @code
 * string t = to_lower("AtCoder");  // "atcoder"
 * @endcode
 */
string to_lower(string s){
    string ret = "";
    rep(i,s.size()) {
        if('A' <= s[i] && s[i] <= 'Z') ret += string(1,s[i]-'A'+'a');
        else ret += string(1,s[i]);
    }
    return ret;
}
