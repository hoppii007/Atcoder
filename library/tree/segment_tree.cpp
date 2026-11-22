#pragma once
#include "library/Math/monoid.cpp"
/**
 * @brief セグメント木（点更新・区間取得）
 *
 *   segtree<M> seg(v) … 配列 v から構築  O(N)
 *   segtree<M> seg(n) … 長さ n（全要素 M::e()）で構築  O(N)
 *   seg.change(i, x)      … a[i] = x  O(log N)
 *   seg.get(i)            … a[i]  O(1)
 *   seg.get(l, r)         … [l, r) の M::merge による集約（l == r なら M::e()）  O(log N)
 *   cout << seg           … 葉 a[0..n) を空白区切りで出力（デバッグ用）  O(N)
 *
 * @tparam M モノイド monoid<T, op, e>
 * @note 区間は半開区間 [l, r)（0-indexed）
 *
 * @code
 * ll op_sum(ll a, ll b){ return a + b; }
 * ll e_sum(){ return 0; }
 * using sum_monoid = monoid<ll, op_sum, e_sum>;
 *
 * vll a = {1, 2, 3, 4, 5};
 * segtree<sum_monoid> seg(a);
 * seg.change(2, 10);          // a[2] を 10 に
 * ll s = seg.get(1, 4);       // a[1] + a[2] + a[3] = 16
 * @endcode
 */
template<typename M>
class segtree{
    using T = typename M::value_type;
    int n, size;
    vector<T> d;

    void update(int k){ d[k] = M::merge(d[2*k], d[2*k+1]); }

public:
    segtree() : n(0), size(1), d(2, M::e()) {}
    segtree(int n_) : segtree(vector<T>(n_, M::e())) {}
    segtree(const vector<T>& v) : n(v.size()) {
        size = 1;
        while(size < n) size <<= 1;
        d.assign(2*size, M::e());
        for(int i = 0; i < n; i++) d[size + i] = v[i];
        for(int i = size - 1; i >= 1; i--) update(i);
    }

    void change(int i, T x){
        i += size;
        d[i] = x;
        for(i >>= 1; i >= 1; i >>= 1) update(i);
    }

    T get(int i){ return d[size + i]; }

    T get(int l, int r){
        T sml = M::e(), smr = M::e();
        l += size;
        r += size;
        while(l < r){
            if(l & 1) sml = M::merge(sml, d[l++]);
            if(r & 1) smr = M::merge(d[--r], smr);
            l >>= 1;
            r >>= 1;
        }
        return M::merge(sml, smr);
    }

    /// @brief 葉 a[0..n) を空白区切りで出力する（デバッグ用。cout << seg）
    friend ostream& operator<<(ostream& os, const segtree& seg){
        for(int i = 0; i < seg.n; i++){
            if(i) os << ' ';
            os << seg.d[seg.size + i];
        }
        return os;
    }
};

/**
 * @brief セグメント木の定番 3 種（点代入・区間取得、ll の配列）
 *
 *   point_change_sum … 点代入・区間和
 *   point_change_max … 点代入・区間最大
 *   point_change_min … 点代入・区間最小
 *
 * 共通の操作（T は上の 3 つのどれか。中身は segtree<monoid<ll, ...>> そのもの）:
 *   T seg(a)          … vll a から構築  O(N)
 *   T seg(n)          … 長さ n で構築。初期値は単位元（和: 0、最大: -inf、最小: inf）  O(N)
 *   seg.change(i, x)  … a[i] = x  O(log N)
 *   seg.get(i)        … a[i]  O(1)
 *   seg.get(l, r)     … [l, r) の和／最大／最小（l == r なら単位元）  O(log N)
 *   cout << seg       … 葉 a[0..n) を空白区切りで出力（デバッグ用）
 *
 * @note 区間は半開区間 [l, r)（0-indexed）
 * @note 点加算は seg.change(i, seg.get(i) + x)
 *
 * @code
 * point_change_sum seg(a);
 * seg.change(2, 10);          // a[2] を 10 に
 * ll s = seg.get(1, 4);       // a[1] + a[2] + a[3]
 *
 * point_change_min mn(n);     // 全 inf
 * mn.change(i, x);
 * ll m = mn.get(l, r);
 * @endcode
 */
namespace segtree_presets{
    ll op_sum(ll a, ll b){ return a + b; }
    ll e_sum(){ return 0; }
    ll op_max(ll a, ll b){ return max(a, b); }
    ll e_max(){ return -inf; }
    ll op_min(ll a, ll b){ return min(a, b); }
    ll e_min(){ return inf; }
}

using point_change_sum = segtree<monoid<ll, segtree_presets::op_sum, segtree_presets::e_sum>>;
using point_change_max = segtree<monoid<ll, segtree_presets::op_max, segtree_presets::e_max>>;
using point_change_min = segtree<monoid<ll, segtree_presets::op_min, segtree_presets::e_min>>;
