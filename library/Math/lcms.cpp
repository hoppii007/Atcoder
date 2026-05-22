/**
 * @brief 配列 v の全要素の最小公倍数を求める
 * @param v 正整数の配列
 * @return lcm(v[0], v[1], ..., v[n-1])
 * @note 計算量: O(n log max(v))
 * @note 途中の lcm が ll の範囲を超えるとオーバーフローする
 */
ll lcms(vll v){
    ll ret = v[0];
    rep(i,v.size()){
        ret = lcm(ret,v[i]);
    }
    return ret;
}
