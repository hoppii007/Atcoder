#pragma once
#include "library/Math/lcm.cpp"
/**
 * @brief 配列全体の最小公倍数
 *
 *   lcms(v) … lcm(v[0], ..., v[n-1])  O(n log max(v))
 *
 * @note v は空でないこと。途中の lcm が ll を超えるとオーバーフローする
 *
 * @code
 * ll l = lcms({4, 6, 10});  // 60
 * @endcode
 */
ll lcms(vll v){
    ll ret = v[0];
    rep(i,v.size()){
        ret = lcm(ret,v[i]);
    }
    return ret;
}
