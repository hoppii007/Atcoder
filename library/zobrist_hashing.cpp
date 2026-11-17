#pragma once
#include "library/Math/random.cpp"
/**
 * @brief Zobrist Hashing（集合のハッシュ値を XOR で差分更新）
 *
 *   zobrist_hashing_table(s)  … s の各要素値 → ランダム値 の map  O(N log N)
 *
 * 集合のハッシュ = 含まれる要素のランダム値の XOR。追加・削除は同じ値を XOR するだけ。
 *
 * @note 多重度は区別しない（集合として比較）。衝突確率は 1 組あたり約 2^-59
 *
 * @code
 * vll s = {3, 1, 4};
 * map<ll,ll> z = zobrist_hashing_table(s);
 * ll h = 0;
 * h ^= z[3]; h ^= z[1];   // {1, 3} のハッシュ
 * h ^= z[3];              // 3 を削除 → {1} のハッシュ
 * @endcode
 */
map<ll,ll> zobrist_hashing_table(vll& s){
    map<ll,ll> ret;
    rep(i, s.size()) ret[s[i]] = randll(0,(1LL << 59));
    return ret;
}
