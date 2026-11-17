#pragma once
/**
 * @brief グリッド範囲内判定
 *
 *   is_in(h, w, i, j) … 0 <= i < h かつ 0 <= j < w なら true
 *
 * @code
 * if(is_in(h, w, i, j)) cout << "in" << endl;
 * @endcode
 */
bool is_in(ll h, ll w, ll i, ll j){
	if(i < 0 || h <= i) return false;
	if(j < 0 || w <= j) return false;
	return true;
}