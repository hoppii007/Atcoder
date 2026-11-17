#pragma once
/**
 * @brief 1次元累積和の配列を返す  O(N)
 *
 *   s = prefix_sum(a) … 長さ N+1。s[0] = 0, s[i+1] = s[i] + a[i]
 *   [l, r) の和       … s[r] - s[l]
 *
 * @param a 元配列（長さ N）
 * @return 累積和配列 s（長さ N+1）
 * @note 区間は半開区間 [l, r)（0-indexed）
 *
 * @code
 * vll a = {1, 2, 3, 4, 5};
 * vll s = prefix_sum(a);   // {0, 1, 3, 6, 10, 15}
 * ll x = s[4] - s[1];      // a[1] + a[2] + a[3] = 9
 * @endcode
 */
template<typename T>
vector<T> prefix_sum(const vector<T>& a){
    vector<T> s(a.size() + 1, T(0));
    for(size_t i = 0; i < a.size(); i++) s[i+1] = s[i] + a[i];
    return s;
}
