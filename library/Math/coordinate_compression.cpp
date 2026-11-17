#pragma once
/**
 * @brief 座標圧縮（比較関数を指定可能）
 *
 *   coordinate_compression(a)        … 元の値 → 圧縮後の値（0-indexed）の map を返す（< で比較）  O(N log N)
 *   coordinate_compression(a, comp)  … comp で比較する版。構造体やペアの一部だけで順位を付けたいときに使う
 *
 * @note 大小関係を保つ。comp で同値（!comp(x,y) && !comp(y,x)）なものは同じ番号になる
 * @note 返り値の map も comp で比較するので、同値な別の要素で引いても同じ番号が返る
 * @note a はソート済みでなくてよい（中でソートする）
 *
 * @code
 * vll a = {100, 5, 100, 30};
 * auto za = coordinate_compression(a);
 * ll x = za[100];  // 2  (5→0, 30→1, 100→2)
 *
 * // 構造体を一部のキーだけで順位付け（BFS の層ごとの辞書順ランクなど）
 * struct E { pll p; ll v; };
 * auto comp = [](const E &x, const E &y){ return x.p < y.p; };
 * auto zb = coordinate_compression(es, comp);
 * ll r = zb[es[i]];  // p が等しい要素は同じランク
 * @endcode
 */
template <typename T, typename Comp = less<T>>
map<T,ll,Comp> coordinate_compression(vector<T> a, Comp comp = Comp()){
    sort(all(a), comp);
    a.erase(unique(all(a), [&](const T &x, const T &y){ return !comp(x, y) && !comp(y, x); }), a.end());
    map<T,ll,Comp> za(comp);
    rep(zaatui, a.size()) za.emplace(a[zaatui], zaatui);
    return za;
}
