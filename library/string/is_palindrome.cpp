/**
 * @brief 文字列が回文かどうか判定する
 * @param s 判定対象の文字列
 * @return s が回文なら true、そうでなければ false
 * @note 空文字列および長さ 1 の文字列は回文として true を返す
 * @note 計算量: O(|s|)
 */
bool is_palindrome(string s){
    rep(i,s.size()/2) if(s[i] != s[(ll)s.size()-1-i]) return false;
    return true;
}
