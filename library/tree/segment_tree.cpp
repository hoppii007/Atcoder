#pragma once
#include "library/Math/monoid.cpp"
/**
 * @brief セグメント木（点更新・区間取得）
 *
 *   SegmentTree<M> seg(v) … 配列 v から構築  O(N)
 *   SegmentTree<M> seg(n) … 長さ n（全要素 M::e()）で構築  O(N)
 *   seg.change(i, x)      … a[i] = x  O(log N)
 *   seg.get(i)            … a[i]  O(1)
 *   seg.get(l, r)         … [l, r) の M::merge による集約（l == r なら M::e()）  O(log N)
 *
 * @tparam M モノイド Monoid<T, op, e>
 * @note 区間は半開区間 [l, r)（0-indexed）
 *
 * @code
 * ll op_sum(ll a, ll b){ return a + b; }
 * ll e_sum(){ return 0; }
 * using Sum = Monoid<ll, op_sum, e_sum>;
 *
 * vll a = {1, 2, 3, 4, 5};
 * SegmentTree<Sum> seg(a);
 * seg.change(2, 10);          // a[2] を 10 に
 * ll s = seg.get(1, 4);       // a[1] + a[2] + a[3] = 16
 * @endcode
 */
template<typename M>
class SegmentTree{
    using T = typename M::value_type;
    int n, size;
    vector<T> d;

    void update(int k){ d[k] = M::merge(d[2*k], d[2*k+1]); }

public:
    SegmentTree() : n(0), size(1), d(2, M::e()) {}
    SegmentTree(int n_) : SegmentTree(vector<T>(n_, M::e())) {}
    SegmentTree(const vector<T>& v) : n(v.size()) {
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
};
