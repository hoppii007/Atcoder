#pragma once
/**
 * @brief ソート済み配列で条件を満たす要素の個数を数える
 *
 *   cnt_lte(a, t)           … t 以下の個数        O(log N)
 *   cnt_lt(a, t)            … t 未満の個数        O(log N)
 *   cnt_gte(a, t)           … t 以上の個数        O(log N)
 *   cnt_gt(a, t)            … t より大きい個数    O(log N)
 *   cnt_gte_lte(a, t1, t2)  … t1 以上 t2 以下の個数  O(log N)
 *
 * @note a は昇順ソート済みであること
 *
 * @code
 * vll a = {1, 3, 3, 5, 8};
 * ll x = cnt_lte(a, 3);          // 3
 * ll y = cnt_gte_lte(a, 2, 5);   // 3
 * @endcode
 */
ll cnt_lte(vll &a, ll t){
    auto it = upper_bound(all(a), t);
    return it - a.begin();
}
ll cnt_lt(vll &a, ll t){
    auto it = lower_bound(all(a), t);
    return it - a.begin();
}
ll cnt_gte(vll &a, ll t){
    auto it = lower_bound(all(a), t);
    ll idx = it-a.begin();
    return a.size() - idx;
}
ll cnt_gt(vll &a, ll t){
    auto it = upper_bound(all(a), t);
    ll idx = it-a.begin();
    return a.size() - idx;
}
ll cnt_gte_lte(vll &a, ll t1, ll t2){
    ll idx1 = lower_bound(all(a), t1) - a.begin();
    ll idx2 = upper_bound(all(a), t2) - a.begin();
    return max(0LL, idx2-idx1);
}