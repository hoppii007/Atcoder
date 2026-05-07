/**
 * @brief 文字列の部分文字列を半開区間 [i, j) で切り出す
 * @param s 元の文字列
 * @param i 切り出し開始位置（含む）。負の値も受け付ける
 * @param j 切り出し終了位置（含まない）。s.size() を超える値も受け付ける
 * @return [i, j) の範囲のうち、s の有効なインデックス [0, s.size()) と重なる部分の文字列。j <= i のときは空文字列
 * @note 標準の string::substr と異なり、範囲が文字列外にはみ出してもエラーにならず、はみ出した部分は無視される
 * @note 計算量: O(j - i)
 * @example substr("abcde", -2, 3) -> "abc"
 */
string substr(string s, ll i, ll j){
    string t = "";
    if(j <= i) return t;
    rep(ii, max(0LL,i), min((ll)s.size(),j)) t += string(1,s[ii]);
    return t;
}
