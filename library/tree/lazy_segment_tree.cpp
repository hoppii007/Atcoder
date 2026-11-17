#pragma once
#include "library/Math/monoid.cpp"
/**
 * @brief 遅延セグメント木（区間作用・区間取得）
 *
 *   lazy_segtree<MS, MF, mapping> lst(v) … 配列 v から構築  O(N)
 *   lazy_segtree<MS, MF, mapping> lst(n) … 長さ n（全要素 MS::e()）で構築
 *   lst.change(l, r, f)                  … [l, r) の各 x を mapping(f, x) に  O(log N)
 *   lst.get(l, r)                        … [l, r) の MS::merge による集約（l == r なら MS::e()）  O(log N)
 *   lst.find_first_condition(l, r, cond) … [l, r) で cond(a[i]) が true の最小の i（無ければ -1）  O(log N)
 *
 * @tparam MS      要素のモノイド monoid<S, op, e>
 * @tparam MF      作用のモノイド monoid<F, composition, id>。composition(f_new, f_old) は「f_old の後に f_new」
 * @tparam mapping S mapping(F f, S x)：作用 f を要素 x に適用した値
 * @note 区間はすべて半開区間 [l, r)（0-indexed）
 * @note cond は「区間内のどれかの要素で true なら、その区間の集約値でも true」となること（例: min < 0）
 *
 * @code
 * // 区間加算・区間和（要素 = {和, 区間長}）
 * using S = pair<ll, ll>;
 * S op_s(S a, S b){ return {a.fst + b.fst, a.snd + b.snd}; }
 * S e_s(){ return {0, 0}; }
 * ll op_f(ll f_new, ll f_old){ return f_new + f_old; }
 * ll e_f(){ return 0; }
 * S mapping(ll f, S x){ return {x.fst + f * x.snd, x.snd}; }
 * using range_add_sum = lazy_segtree<monoid<S, op_s, e_s>, monoid<ll, op_f, e_f>, mapping>;
 *
 * range_add_sum lst(vector<S>(n, {0, 1}));
 * lst.change(l, r, x);       // [l, r) に x を加算
 * ll s = lst.get(l, r).fst;  // [l, r) の和
 *
 * // 区間代入にする場合（inf を「作用なし」とする）
 * ll op_f(ll f_new, ll f_old){ return f_new == inf ? f_old : f_new; }
 * ll e_f(){ return inf; }
 * S mapping(ll f, S x){ return f == inf ? x : S{f * x.snd, x.snd}; }
 * @endcode
 */
template<typename MS, typename MF, typename MS::value_type (*mapping)(typename MF::value_type, typename MS::value_type)>
class lazy_segtree{
    using S = typename MS::value_type;
    using F = typename MF::value_type;
    int n, size, log;
    vector<S> d;
    vector<F> lz;

    void update(int k){ d[k] = MS::merge(d[2*k], d[2*k+1]); }
    void all_apply(int k, const F& f){
        d[k] = mapping(f, d[k]);
        if(k < size) lz[k] = MF::merge(f, lz[k]);
    }
    void push(int k){
        all_apply(2*k, lz[k]);
        all_apply(2*k+1, lz[k]);
        lz[k] = MF::e();
    }
    // 半開区間 [l, r) を含むノードの遅延を、根から葉の方向へ流す
    void push_range(int l, int r){
        for(int i = log; i >= 1; i--){
            if(((l >> i) << i) != l) push(l >> i);
            if(((r >> i) << i) != r) push((r - 1) >> i);
        }
    }
    template<class Cond>
    int find_dfs(int k, int nl, int nr, int l, int r, Cond& cond){
        if(nr <= l || r <= nl || !cond(d[k])) return -1;
        if(k >= size) return k - size;
        push(k);
        int mid = (nl + nr) / 2;
        int res = find_dfs(2*k, nl, mid, l, r, cond);
        if(res != -1) return res;
        return find_dfs(2*k+1, mid, nr, l, r, cond);
    }

public:
    lazy_segtree(int n_) : lazy_segtree(vector<S>(n_, MS::e())) {}
    lazy_segtree(const vector<S>& v) : n(v.size()) {
        size = 1;
        log = 0;
        while(size < n){ size <<= 1; log++; }
        d.assign(2*size, MS::e());
        lz.assign(size, MF::e());
        for(int i = 0; i < n; i++) d[size + i] = v[i];
        for(int i = size - 1; i >= 1; i--) update(i);
    }

    void change(int l, int r, F f){
        if(l >= r) return;
        l += size;
        r += size;
        push_range(l, r);
        int l2 = l, r2 = r;
        while(l < r){
            if(l & 1) all_apply(l++, f);
            if(r & 1) all_apply(--r, f);
            l >>= 1;
            r >>= 1;
        }
        l = l2;
        r = r2;
        for(int i = 1; i <= log; i++){
            if(((l >> i) << i) != l) update(l >> i);
            if(((r >> i) << i) != r) update((r - 1) >> i);
        }
    }

    S get(int l, int r){
        if(l >= r) return MS::e();
        l += size;
        r += size;
        push_range(l, r);
        S sml = MS::e(), smr = MS::e();
        while(l < r){
            if(l & 1) sml = MS::merge(sml, d[l++]);
            if(r & 1) smr = MS::merge(d[--r], smr);
            l >>= 1;
            r >>= 1;
        }
        return MS::merge(sml, smr);
    }

    template<class Cond>
    int find_first_condition(int l, int r, Cond cond){
        return find_dfs(1, 0, size, l, r, cond);
    }
};
