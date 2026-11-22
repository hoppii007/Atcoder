#pragma once
/**
 * @brief 英字の大文字・小文字変換（1 文字 / 文字列）
 *
 *   to_upper(c) … 英小文字なら大文字に、それ以外はそのまま返す  O(1)
 *   to_upper(s) … 英小文字を大文字にした文字列（他の文字はそのまま）  O(|s|)
 *   to_lower(c) … 英大文字なら小文字に、それ以外はそのまま返す  O(1)
 *   to_lower(s) … 英大文字を小文字にした文字列（他の文字はそのまま）  O(|s|)
 *
 * @code
 * char c = to_upper('a');           // 'A'
 * string t = to_upper("AtCoder");  // "ATCODER"
 * char d = to_lower('A');           // 'a'
 * string u = to_lower("AtCoder");  // "atcoder"
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

char to_lower(char c){
    if('A' <= c && c <= 'Z') return c-'A'+'a';
    else return c;
}

string to_lower(string s){
    for(auto it = s.begin(); it != s.end(); it++) *it = to_lower(*it);
    return s;
}
