#pragma once
/**
 * @brief 3点の共線判定
 *
 *   is_same_line(p1, p2, p3) … 3点 (x, y) = (fst, snd) が同一直線上なら true  O(1)
 *
 * @note 整数のまま判定（誤差なし）。垂直な直線も可。重なる点を含む場合も true
 * @note 座標差が ll に収まること（積は __int128 で計算）
 *
 * @code
 * bool b1 = is_same_line({0, 0}, {1, 2}, {3, 6});  // true
 * bool b2 = is_same_line({0, 0}, {1, 2}, {3, 5});  // false
 * @endcode
 */
bool is_same_line(pll p1, pll p2, pll p3){
    // (p2 - p1) × (p3 - p1) == 0 ⇔ 共線
    __int128 cross = (__int128)(p2.fst-p1.fst)*(p3.snd-p1.snd) - (__int128)(p2.snd-p1.snd)*(p3.fst-p1.fst);
    return cross == 0;
}