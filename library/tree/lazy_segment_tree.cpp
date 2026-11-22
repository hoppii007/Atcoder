#pragma once
#include "library/Math/monoid.cpp"
/**
 * @brief 遅延セグメント木（区間作用・区間取得）
 *
 *   lazy_segtree<MS, MF, mapping> lst(v) … 配列 v から構築  O(N)
 *   lazy_segtree<MS, MF, mapping> lst(n) … 長さ n（全要素 MS::e()）で構築
 *   lst.change(l, r, f)                  … [l, r) の各 x を mapping(f, x) に  O(log N)
 *   lst.get(l, r)                        … [l, r) の MS::merge による集約（l == r なら MS::e()）  O(log N)
 *   lst.find_first_condition(l, r, cond) … [l, r) で cond(a[i]) が true の最小の i（無ければ n）  O(log N)
 *
 * @tparam MS      要素のモノイド monoid<S, op, e>
 * @tparam MF      作用のモノイド monoid<F, composition, id>。composition(f_new, f_old) は「f_old の後に f_new」
 * @tparam mapping S mapping(F f, S x)：作用 f を要素 x に適用した値
 * @note 区間はすべて半開区間 [l, r)（0-indexed）
 *
 * find_first_condition(l, r, cond) について:
 *   [l, r) で cond(a[i]) が true になる最小の i を返す（無ければ n）。
 *   cond(a[i]) が i について単調である必要はない（true が飛び飛びでもよい）。
 *   木を左の子 → 右の子の順にたどり、ノードの集約値で cond が false ならその区間を丸ごと読み飛ばす。
 *   - 必要条件（正しさ）: 「区間内に cond が true の葉がある ⇒ その区間の集約値でも cond が true」
 *       これが崩れると true の葉を含む区間を読み飛ばし、答えを見落とす。
 *       反例: 区間和で cond = (v > 0)、a = [-5, 3]。根の和 -2 で打ち切られ、1 を見落とす。
 *   - 逆向き「集約値で true ⇒ true の葉がある」は不要。崩れても空振りが増えるだけで答えは正しい
 *       （ただし空振りが多いと O(N) に近づく）。両向き成り立てば O(log N)。
 *   - 成り立つ例: max で v >= k、min で v <= k（両向き成立）、非負配列の和で v > 0。
 *   - cond(a[i]) が i について単調なら、葉を get(i, i + 1) で二分探索してもよい（O(log^2 N)、集約値の条件は不要）。
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
        int res = find_dfs(1, 0, size, l, r, cond);
        return res == -1 ? n : res;
    }
};

/**
 * @brief 遅延セグメント木の定番 6 種（ll の配列としてだけ扱う）
 *
 *   range_add_sum    … 区間加算・区間和
 *   range_change_sum … 区間代入・区間和
 *   range_add_max    … 区間加算・区間最大
 *   range_change_max … 区間代入・区間最大
 *   range_add_min    … 区間加算・区間最小
 *   range_change_min … 区間代入・区間最小
 *
 * 共通の操作（T は上の 6 つのどれか）:
 *   T lst(n)                             … 長さ n・全 0 で構築  O(N)
 *   T lst(a)                             … vll a から構築  O(N)
 *   lst.change(l, r, x)                  … [l, r) に x を加算（add 系）／ x を代入（change 系）  O(log N)
 *   lst.get(i)                           … a[i]  O(log N)
 *   lst.get(l, r)                        … [l, r) の和／最大／最小  O(log N)
 *   lst.find_first_condition(l, r, cond) … [l, r) で cond(a[i]) が true の最小の i（無ければ n）  O(log N)
 *
 * @note 区間はすべて半開区間 [l, r)（0-indexed）
 * @note get(l, r) で l == r のときは単位元（和: 0、最大: -inf、最小: inf）
 * @note find_first_condition の cond(ll) は区間の集約値（和／最大／最小）にも適用される。
 *       必要条件は「区間内に true の要素がある ⇒ 集約値でも true」（詳細は lazy_segtree の説明）
 *       （例: max 系で v >= k、min 系で v <= k、和で非負配列に v > 0）
 * @note 和の型は内部で {和, 区間長} を持つが、外からは見えない
 *
 * @code
 * range_add_sum lst(a);
 * lst.change(l, r, x);        // [l, r) に x を加算
 * ll s = lst.get(l, r);       // [l, r) の和
 * ll v = lst.get(i);          // a[i]
 *
 * range_change_min mn(n);     // 全 0
 * mn.change(l, r, x);         // [l, r) を x に
 * ll i = mn.find_first_condition(0, n, [&](ll v){ return v <= k; });
 * @endcode
 */
namespace lazy_segtree_presets{
    const ll no_change = LLONG_MIN;  // change 系の「作用なし」

    // 和: {和, 区間長}
    using sum_s = pair<ll, ll>;
    sum_s op_sum(sum_s a, sum_s b){ return {a.fst + b.fst, a.snd + b.snd}; }
    sum_s e_sum(){ return {0, 0}; }
    sum_s make_sum(ll x){ return {x, 1}; }
    ll value_sum(sum_s s){ return s.fst; }

    // 最大・最小
    ll op_max(ll a, ll b){ return max(a, b); }
    ll e_max(){ return -inf; }
    ll op_min(ll a, ll b){ return min(a, b); }
    ll e_min(){ return inf; }
    ll make_id(ll x){ return x; }
    ll value_id(ll x){ return x; }

    // 作用: 加算
    ll comp_add(ll f_new, ll f_old){ return f_new + f_old; }
    ll id_add(){ return 0; }
    // 作用: 代入
    ll comp_change(ll f_new, ll f_old){ return f_new == no_change ? f_old : f_new; }
    ll id_change(){ return no_change; }

    sum_s map_add_sum(ll f, sum_s x){ return {x.fst + f * x.snd, x.snd}; }
    sum_s map_change_sum(ll f, sum_s x){ return f == no_change ? x : sum_s{f * x.snd, x.snd}; }
    ll map_add(ll f, ll x){ return x + f; }
    ll map_change(ll f, ll x){ return f == no_change ? x : f; }

    /**
     * @brief lazy_segtree を ll の配列として包む
     * @tparam make  ll から要素 S を作る
     * @tparam value 要素 S から ll を取り出す
     */
    template<typename S, S (*op)(S, S), S (*e)(),
             ll (*comp)(ll, ll), ll (*id)(),
             S (*mapping)(ll, S), S (*make)(ll), ll (*value)(S)>
    class wrapper{
        lazy_segtree<monoid<S, op, e>, monoid<ll, comp, id>, mapping> seg;

        static vector<S> leaves(const vll& a){
            vector<S> v(a.size());
            for(size_t i = 0; i < a.size(); i++) v[i] = make(a[i]);
            return v;
        }

    public:
        wrapper(int n) : seg(leaves(vll(n, 0))) {}
        wrapper(const vll& a) : seg(leaves(a)) {}

        void change(int l, int r, ll x){ seg.change(l, r, x); }
        ll get(int i){ return value(seg.get(i, i + 1)); }
        ll get(int l, int r){ return value(seg.get(l, r)); }

        template<class Cond>
        int find_first_condition(int l, int r, Cond cond){
            return seg.find_first_condition(l, r, [&](const S& s){ return cond(value(s)); });
        }
    };
}

using range_add_sum    = lazy_segtree_presets::wrapper<lazy_segtree_presets::sum_s, lazy_segtree_presets::op_sum, lazy_segtree_presets::e_sum,
                                                       lazy_segtree_presets::comp_add, lazy_segtree_presets::id_add,
                                                       lazy_segtree_presets::map_add_sum, lazy_segtree_presets::make_sum, lazy_segtree_presets::value_sum>;
using range_change_sum = lazy_segtree_presets::wrapper<lazy_segtree_presets::sum_s, lazy_segtree_presets::op_sum, lazy_segtree_presets::e_sum,
                                                       lazy_segtree_presets::comp_change, lazy_segtree_presets::id_change,
                                                       lazy_segtree_presets::map_change_sum, lazy_segtree_presets::make_sum, lazy_segtree_presets::value_sum>;
using range_add_max    = lazy_segtree_presets::wrapper<ll, lazy_segtree_presets::op_max, lazy_segtree_presets::e_max,
                                                       lazy_segtree_presets::comp_add, lazy_segtree_presets::id_add,
                                                       lazy_segtree_presets::map_add, lazy_segtree_presets::make_id, lazy_segtree_presets::value_id>;
using range_change_max = lazy_segtree_presets::wrapper<ll, lazy_segtree_presets::op_max, lazy_segtree_presets::e_max,
                                                       lazy_segtree_presets::comp_change, lazy_segtree_presets::id_change,
                                                       lazy_segtree_presets::map_change, lazy_segtree_presets::make_id, lazy_segtree_presets::value_id>;
using range_add_min    = lazy_segtree_presets::wrapper<ll, lazy_segtree_presets::op_min, lazy_segtree_presets::e_min,
                                                       lazy_segtree_presets::comp_add, lazy_segtree_presets::id_add,
                                                       lazy_segtree_presets::map_add, lazy_segtree_presets::make_id, lazy_segtree_presets::value_id>;
using range_change_min = lazy_segtree_presets::wrapper<ll, lazy_segtree_presets::op_min, lazy_segtree_presets::e_min,
                                                       lazy_segtree_presets::comp_change, lazy_segtree_presets::id_change,
                                                       lazy_segtree_presets::map_change, lazy_segtree_presets::make_id, lazy_segtree_presets::value_id>;
