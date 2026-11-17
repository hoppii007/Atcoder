#pragma once
/**
 * @brief 範囲外でも安全な部分文字列
 *
 *   substr(s, i, j) … s の半開区間 [i, j) の部分文字列  O(j - i)
 *
 * @note はみ出した部分は無視する（i < 0 や j > |s| も可）。j <= i なら ""
 * @note 第 3 引数は長さではなく終端位置（std::string::substr と違う）
 *
 * @code
 * string t = substr("abcde", -2, 3);  // "abc"
 * @endcode
 */
string substr(string s, ll i, ll j){
    string t = "";
    if(j <= i) return t;
    rep(ii, max(0LL,i), min((ll)s.size(),j)) t += string(1,s[ii]);
    return t;
}
