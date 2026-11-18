#pragma once
/**
 * @brief 文字を巡回的に n 個ずらす
 *
 *   shift_char(c, n, base, m) … base から始まる m 文字の巡回（base, base+1, ..., base+m-1）の中で
 *                               c を n 個先にずらした文字を返す  O(1)
 *
 * @param c    ずらす文字（base <= c < base+m であること）
 * @param n    ずらす量。負なら手前にずれる。10^18 のような巨大な値でもよい
 * @param base 巡回の先頭の文字（既定 'a'）
 * @param m    巡回の長さ（既定 26）
 *
 * @code
 * char a = shift_char('y', 3);             // 'b'（シーザー暗号）
 * char b = shift_char('A', -1, 'A', 3);    // 'C'（A,B,C の巡回）
 * char c = shift_char('B', (ll)1e18, 'A', 3);
 * @endcode
 */
char shift_char(char c, ll n, char base = 'a', ll m = 26){
    ll x = ((c - base) + n % m + m) % m;
    return (char)(base + x);
}
