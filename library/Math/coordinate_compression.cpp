#pragma once
/**
 * @brief 座標圧縮
 *
 *   coordinate_compression(a) … 元の値 → 圧縮後の値（0-indexed）の map を返す  O(N log N)
 *
 * @note 大小関係を保つ。同じ値は同じ番号になる
 *
 * @code
 * vll a = {100, 5, 100, 30};
 * map<ll,ll> za = coordinate_compression(a);
 * ll x = za[100];  // 2  (5→0, 30→1, 100→2)
 * @endcode
 */
map<ll,ll> coordinate_compression(vll a){
    sort(all(a));
    a.erase(unique(all(a)), a.end());
    map<ll,ll> za;
    rep(zaatui, a.size()) za[a[zaatui]] = zaatui;
    return za;
}