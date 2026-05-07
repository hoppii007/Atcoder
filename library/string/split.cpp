/**
 * @brief 文字列を指定した区切り文字で分割する
 * @param s 分割対象の文字列
 * @param c 区切り文字
 * @return 分割された文字列の配列。連続する区切り文字や先頭が区切り文字の場合は空文字列が要素として含まれる。末尾が区切り文字の場合は空文字列を追加しない
 * @note 計算量: O(|s|)
 * @example split("a,b,,c", ',') -> {"a", "b", "", "c"}
 */
vs split(string s, char c){
    vs ret;
    s += string(1,c);  // 末尾要素を確実に push するための番兵
    string pre = "";
    rep(i, s.size()){
        if(s[i] == c){
            ret.pb(pre);
            pre = "";
        }
        else{
            pre += string(1, s[i]);
        }
    }
    return ret;
}
