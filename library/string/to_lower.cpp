#pragma once
/**
 * @brief 英大文字を小文字に変換（1 文字 / 文字列）
 *
 *   to_lower(c) … 英大文字なら小文字に、それ以外はそのまま返す  O(1)
 *   to_lower(s) … 英大文字を小文字にした文字列（他の文字はそのまま）  O(|s|)
 *
 * @code
 * char c = to_lower('A');           // 'a'
 * string t = to_lower("AtCoder");  // "atcoder"
 * @endcode
 */
char to_lower(char c){
    if('A' <= c && c <= 'Z') return c-'A'+'a';
    else return c;
}

string to_lower(string s){
    for(auto it = s.begin(); it != s.end(); it++) *it = to_lower(*it);
    return s;
}
