#pragma once
/**
 * @brief グリッドの範囲内の近傍マス
 *
 *   kinbo4(h, w, i, j) … (i, j) の上下左右のうち [0,h) x [0,w) に入るマス
 *   kinbo8(h, w, i, j) … (i, j) の周囲 8 マスのうち範囲内のもの（左上から行優先の順）
 *
 * @code
 * for(auto [ni, nj]: kinbo4(h, w, i, j)) cout << ni << " " << nj << endl;
 * @endcode
 */
vpll kinbo4(ll h,ll w, ll i,ll j){
    vpll ret;
    if(i-1 >= 0) ret.push_back({i-1,j});
    if(j-1 >= 0) ret.push_back({i,j-1});
    if(i+1 < h) ret.push_back({i+1,j});
    if(j+1 < w) ret.push_back({i,j+1});
    return ret;
}

vpll kinbo8(ll h,ll w, ll i,ll j){
    vpll ret;
    if(i-1 >= 0 && j-1 >= 0) ret.push_back({i-1, j-1});
    if(i-1 >= 0) ret.push_back({i-1, j});
    if(i-1 >= 0 && j+1 < w) ret.push_back({i-1, j+1});
    if(j-1 >= 0) ret.push_back({i, j-1});
    if(j+1 < w) ret.push_back({i, j+1});
    if(i+1 < h && j-1 >= 0) ret.push_back({i+1, j-1});
    if(i+1 < h) ret.push_back({i+1, j});
    if(i+1 < h && j+1 < w) ret.push_back({i+1, j+1});
    return ret;
}