#pragma once
/**
 * @brief 昇順コンテナの中で t に最も近い要素を探す
 *
 *   near_value(c, t)  … t に最も近い要素のイテレータ（同距離なら小さい方）  O(log N)
 *
 * @param c ソート済み vector、または set / multiset（set 系はメンバ lower_bound を自動で使う）
 * @note c は空でなく、非 const であること
 *
 * @code
 * vll v = {1, 3, 5, 7};
 * auto it = near_value(v, 4);    // *it == 3
 * multiset<ll> m = {1, 3, 5, 7};
 * auto mit = near_value(m, 6);   // *mit == 5
 * @endcode
 */
template<typename C, typename = void>
struct has_member_lower_bound : false_type {};
template<typename C>
struct has_member_lower_bound<C, void_t<decltype(declval<C&>().lower_bound(declval<const typename C::value_type&>()))>> : true_type {};

template<typename C>
typename C::iterator near_value(C &c, const typename C::value_type &t){
    //set / multiset はメンバ版、vector などは std::lower_bound
    typename C::iterator it;
    if constexpr (has_member_lower_bound<C>::value) it = c.lower_bound(t);
    else it = lower_bound(c.begin(), c.end(), t);
    // 以上のやつが一個もないとき（すべてtより小さいとき）
    if(it == c.end()) return prev(it);
    // 一個前もあればそれと今の比較
    if(it != c.begin()){
        auto pit = prev(it);
        //一個前の方がitよりtに近ければそれを返す
        if(abs(*pit-t) <= abs(*it-t)) return pit;
    }
    // 一個前がなければそこ
    return it;
}
