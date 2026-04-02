
//aは昇順ソート済み
//以下の個数
ll cnt_lte(vll &a, ll t){
    auto it = upper_bound(all(a), t);
    return it - a.begin();
}
//未満の個数
ll cnt_lt(vll &a, ll t){
    auto it = lower_bound(all(a), t);
    return it - a.begin();
}
//以上の個数
ll cnt_gte(vll &a, ll t){
    auto it = lower_bound(all(a), t);
    ll idx = it-a.begin();
    return a.size() - idx;
}
//より大きい個数
ll cnt_gt(vll &a, ll t){
    auto it = upper_bound(all(a), t);
    ll idx = it-a.begin();
    return a.size() - idx;
}
//以上以下
ll cnt_gte_lte(vll &a, ll t1, ll t2){
    ll idx1 = lower_bound(all(a), t1) - a.begin();
    ll idx2 = upper_bound(all(a), t2) - a.begin();
    return max(0LL, idx2-idx1);
}