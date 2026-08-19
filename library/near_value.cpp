
//メンバ lower_bound を持つか判定（set / multiset は持つ、vector は持たない）
template<typename C, typename = void>
struct has_member_lower_bound : false_type {};
template<typename C>
struct has_member_lower_bound<C, void_t<decltype(declval<C&>().lower_bound(declval<const typename C::value_type&>()))>> : true_type {};

/**
 * @brief 昇順のコンテナから最も近い値のイテレータを返す
 *
 * @tparam C ソート済み vector、または set / multiset
 * @param c  対象コンテナ（空でないこと）
 * @param t  探索する値
 * @return   t に最も近い要素を指すイテレータ
 *
 * @pre vector を渡す場合は昇順ソート済みであること
 * @pre c は空でないこと
 * @pre c は非 const であること（const コンテナは const_iterator になるため）
 * @note 同距離の場合は小さい方を返す
 * @note set / multiset ではメンバの lower_bound に自動で切り替わる。
 *       これらに std::lower_bound を使うとイテレータ移動が O(N) になり O(log N) にならない
 * @complexity O(log N)
 *
 * @example
 * std::vector<ll> v = {1, 3, 5, 7};
 * auto it = near_value(v, 4); // *it == 3
 * std::multiset<ll> m = {1, 3, 5, 7};
 * auto mit = near_value(m, 4); // *mit == 3
 */
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
