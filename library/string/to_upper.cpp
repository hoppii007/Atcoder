#pragma once
/**
 * @brief 英小文字を大文字に変換（1 文字 / 文字列）
 *
 *   to_upper(c) … 英小文字なら大文字に、それ以外はそのまま返す  O(1)
 *   to_upper(s) … 英小文字を大文字にした文字列（他の文字はそのまま）  O(|s|)
 *
 * @code
 * char c = to_upper('a');           // 'A'
 * string t = to_upper("AtCoder");  // "ATCODER"
 * @endcode
 */
char to_upper(char c){
    if('a' <= c && c <= 'z') return c-'a'+'A';
    else return c;
}

string to_upper(string s){
    for(auto it = s.begin(); it != s.end(); it++) *it = to_upper(*it);
    return s;
}
