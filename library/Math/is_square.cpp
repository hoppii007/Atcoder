#pragma once
/**
 * @brief 4点が正方形をなすかの判定
 *
 *   is_square(a, b, c, d)        … 4頂点（順不同）が正方形なら true  O(1)
 *   is_square(a, b, c, d, false) … 4点が同一点（辺の長さ 0）でも true にする
 *
 * @note 傾いた正方形も可。座標差の2乗が ll に収まること
 *
 * @code
 * bool b = is_square({0, 1}, {1, 0}, {2, 1}, {1, 2});  // true
 * @endcode
 */
bool is_square(pll a,pll b,pll c,pll d, bool overlap_is_out = true){
    vpll ps = {a,b,c,d};
    sort(all(ps));
    swap(ps[2], ps[3]);
    //長さチェック
    ll dx = ps[3].first-ps[0].first;
    ll dy = ps[3].second-ps[0].second;
    ll len = dx*dx + dy*dy;
    if(overlap_is_out && len == 0) return false;
    rep(i,ps.size()){
        dx = ps[(i+1)%4].first - ps[i].first;
        dy = ps[(i+1)%4].second - ps[i].second;
        if(dx*dx + dy*dy != len) return false;
    }
    //角度チェック
    rep(i,ps.size()){
        ll dx1 = ps[(i+1)%4].first - ps[i].first;
        ll dy1 = ps[(i+1)%4].second - ps[i].second;
        ll dx2 = ps[(i+2)%4].first - ps[(i+1)%4].first;
        ll dy2 = ps[(i+2)%4].second - ps[(i+1)%4].second;
        if(dy1 * dy2 != -dx1 * dx2) return false;
    }
    return true;
}
