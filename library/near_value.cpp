
/**
 * @brief 昇順ソート済み範囲から最も近い値のイテレータを返す
 *
 * @tparam It ランダムアクセスイテレータ
 * @param be 範囲の先頭イテレータ
 * @param en 範囲の末尾イテレータ（末尾の次）
 * @param t  探索する値
 * @return   t に最も近い要素を指すイテレータ
 *
 * @pre [be, en) は昇順ソート済みであること
 * @note 同距離の場合は小さい方を返す
 * @complexity O(log N)
 *
 * @example
 * std::vector<int> v = {1, 3, 5, 7};
 * auto it = near_value(v.begin(), v.end(), 4); // *it == 3
 */
template<typename It>
It near_value(It be, It en, typename iterator_traits<It>::value_type t){
    auto it = lower_bound(be, en, t);
    // 以上のやつが一個もないとき（すべてtより小さいとき）
    if(it == en) return prev(it);
    // 一個前もあればそれと今の比較
    if(it != be){
        auto pit = prev(it);
        //一個前の方がitよりtに近ければそれを返す
        if(abs(*pit-t) <= abs(*it-t)) return pit;
        else return it;
    }
    // 一個前がなければそこ
    return it;
}
