#pragma once
/**
 * @brief 列を先頭から長さ k ずつのまとまりに分割する（最後のまとまりは k 未満になりうる）
 *
 *   split_ranges(n, k) … [0, n) を長さ k ずつに区切った半開区間 [l, r) の列  O(n/k)
 *   split_k(a, k)      … a を長さ k ずつに分割した vector<vector<T>>          O(n)
 *
 * @note まとまり数は ceil(n/k)。n == 0 なら空。k ≥ 1 を前提とする。
 *       和だけ欲しいなら split_ranges と累積和（prefix_sum.cpp）を組み合わせるとコピーが要らない。
 *
 * @code
 * vll f = {7, 6, 6, 3, 1};
 * auto rs = split_ranges(f.size(), 2);  // {{0,2}, {2,4}, {4,5}}
 * for (auto [l, r] : rs) { ll total = accumulate(f.begin() + l, f.begin() + r, 0LL); }
 *
 * auto sp = split_k(f, 2);               // {{7,6}, {6,3}, {1}}
 * @endcode
 */
vpll split_ranges(ll n, ll k){
    vpll ret;
    for(ll l = 0; l < n; l += k){
        ret.pb({l, min(l + k, n)});
    }
    return ret;
}

template <typename T>
vector<vector<T>> split_k(const vector<T> &a, ll k){
    vector<vector<T>> ret;
    for(auto [l, r] : split_ranges(a.size(), k)){
        ret.emplace_back(a.begin() + l, a.begin() + r);
    }
    return ret;
}
