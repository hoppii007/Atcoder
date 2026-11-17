#pragma once
/**
 * @brief モノイド（値の型・演算・単位元をまとめた型）
 *
 *   M::merge(a, b) … 演算
 *   M::e()         … 単位元
 *   M::value_type  … 値の型
 *
 * @tparam T  値の型（ll / modll / 構造体 など）
 * @tparam op 演算 T op(T, T)（結合則を満たすこと）
 * @tparam E  単位元を返す関数 T E()
 *
 * @code
 * // 区間和
 * ll op_sum(ll a, ll b){ return a + b; }
 * ll e_sum(){ return 0; }
 * using Sum = Monoid<ll, op_sum, e_sum>;
 *
 * // 区間最小
 * ll op_min(ll a, ll b){ return min(a, b); }
 * ll e_min(){ return inf; }
 * using Min = Monoid<ll, op_min, e_min>;
 *
 * SegmentTree<Sum> seg(a);
 * @endcode
 */
template<typename T, T (*op)(T, T), T (*E)()>
struct Monoid {
    using value_type = T;
    static T e(){ return E(); }
    static constexpr auto merge = op;
};
