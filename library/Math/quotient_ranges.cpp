#pragma once
/**
 * @brief 商列挙（floor(n / i) の値ごとに i の区間をまとめる）
 *
 *   quotient_ranges(n) … i = 1..n を ⌊n / i⌋ の値で区間に分割して返す  O(√n)
 *     各要素 {q, l, r} は「i ∈ [l, r) で ⌊n / i⌋ = q」を表す（半開区間）
 *     i の昇順（q は降順）に並ぶ。要素数は 2√n 程度
 *
 * @note n >= 1。⌊n / i⌋ の異なる値は高々 2√n 個しかないことを利用する。
 *       q = ⌊n / i⌋ となる最大の i は ⌊n / q⌋ なので、r = ⌊n / q⌋ + 1
 *
 * @code
 * // Σ_{i=1}^{n} ⌊n / i⌋ を O(√n) で
 * ll sum = 0;
 * for(auto [q, l, r] : quotient_ranges(n)) sum += q * (r - l);
 *
 * // n = 10 → {10,1,2} {5,2,3} {3,3,4} {2,4,6} {1,6,11}
 * @endcode
 */
struct quotient_range {
    ll q, l, r;
};

vector<quotient_range> quotient_ranges(ll n){
    vector<quotient_range> ret;
    for(ll l=1;l<=n;){
        ll q = n / l;
        ll r = n / q + 1;
        ret.push_back({q, l, r});
        l = r;
    }
    return ret;
}
