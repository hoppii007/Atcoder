#pragma once
/**
 * @brief 1 文字を小文字に変換
 *
 *   to_lowerOne(c) … 英大文字なら小文字に、それ以外はそのまま返す  O(1)
 *
 * @code
 * char c = to_lowerOne('A');  // 'a'
 * @endcode
 */
char to_lowerOne(char c){
    if('A' <= c && c <= 'Z') return c-'A'+'a';
    else return c;
}
