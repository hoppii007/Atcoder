#pragma once
/**
 * @brief 1 文字を大文字に変換
 *
 *   to_upperOne(c) … 英小文字なら大文字に、それ以外はそのまま返す  O(1)
 *
 * @code
 * char c = to_upperOne('a');  // 'A'
 * @endcode
 */
char to_upperOne(char c){
    if('a' <= c && c <= 'z') return c-'a'+'A';
    else return c;
}

