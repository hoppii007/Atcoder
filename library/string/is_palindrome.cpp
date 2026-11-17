#pragma once
/**
 * @brief 回文判定
 *
 *   is_palindrome(s) … s が回文なら true  O(|s|)
 *
 * @code
 * bool b = is_palindrome("abcba");  // true
 * @endcode
 */
bool is_palindrome(string s){
    rep(i,s.size()/2) if(s[i] != s[(ll)s.size()-1-i]) return false;
    return true;
}
