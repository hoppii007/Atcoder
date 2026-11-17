#pragma once
#include "library/Math/mod_ll.cpp"
/**
 * @brief 二項係数 nCk（mod、階乗の前計算）
 *
 *   mc.init(N)          … N までの階乗・逆階乗を前計算  O(N)
 *   mc.comb(n, k)       … nCk（k < 0, n < k なら 0）  O(1)
 *   mc.f[i], mc.invf[i] … i! / (i!)^{-1}
 *
 * @note n <= N であること。modll::mod は素数
 *
 * @code
 * ModComb mc;
 * mc.init(200000);
 * modll x = mc.comb(5, 2);  // 10
 * @endcode
 */
struct ModComb {
    vector<modll> f;
    vector<modll> invf;

    void init(ll n){
        f.assign(n+1, 1);
        rep(i, 1, n+1) f[i] = i*f[i-1];
        invf.assign(n+1, 1/f[n]);
        mrep(i, n-1) invf[i] = (i+1)*invf[i+1];
    }

    modll comb(ll n, ll k){
        if(n < k) return 0;
        if(n < 0 || k < 0) return 0;
        return f[n] * invf[k] * invf[n-k];
    }
};