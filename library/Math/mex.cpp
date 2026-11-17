#pragma once
/**
 * @brief mex（0, 1, 2, ... のうち a に含まれない最小の値）
 *
 *   mex(a) … a の mex  O(max(a) + |a|)
 *
 * @note 負の要素は無視される。max(a) が大きいとメモリを食う
 *
 * @code
 * vll a = {0, 1, 3};
 * ll m = mex(a);  // 2
 * @endcode
 */
ll mex(vll& a){
    if(a.size() == 0) return 0;
    vb f(max(0LL, *max_element(all(a))+1),false);
    rep(i,a.size()) if(a[i] < f.size()) f[a[i]] = true;
    rep(i,f.size()) if(f[i] == false) return i;
    return (ll)f.size();
}
