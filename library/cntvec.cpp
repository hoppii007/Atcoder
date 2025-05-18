
//aは昇順ソート済み
//以下の個数
ll cntLte(vll &a, ll t){
    auto it = upper_bound(all(a), t);
    return it - a.begin();
}
//未満の個数
ll cntLt(vll &a, ll t){
    auto it = lower_bound(all(a), t);
    return it - a.begin();
}
//以上の個数
ll cntGte(vll &a, ll t){
    auto it = lower_bound(all(a), t);
    ll idx = it-a.begin();
    return a.size() - idx;
}
//より大きい個数
ll cntGt(vll &a, ll t){
    auto it = upper_bound(all(a), t);
    ll idx = it-a.begin();
    return a.size() - idx;
}
//以上以下
ll cntGteLte(vll &a, ll t1, ll t2){
    ll idx1 = lower_bound(all(a), t1) - a.begin();
    ll idx2 = upper_bound(all(a), t2) - a.begin();
    return max(0LL, idx2-idx1);
}