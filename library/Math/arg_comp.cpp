#pragma once
/**
 * @brief 偏角ソート（偏角の比較関数）
 *
 *   arg_comp(p0, p1)    … arg(p0) < arg(p1) なら true  O(1)
 *   is_same_arg(p0, p1) … 偏角が等しい（同じ向き）なら true  O(1)
 *
 * @param p0, p1 ベクトル (x, y) = (fst, snd)
 * @note 偏角は +x 軸を 0 とし反時計回りに [0, 2π)。(0,0) は最初に来る
 * @note 外積 x0*y1 - y0*x1 が ll に収まること
 *
 * @code
 * vpll v = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}, {0, 0}};
 * sort(all(v), arg_comp);  // (0,0) (1,0) (0,1) (-1,0) (0,-1)
 * bool same = is_same_arg({1, 1}, {2, 2});  // true
 * @endcode
 */
bool arg_comp(pll p0, pll p1) {
    // (0,0) → 0、上半分（偏角が [0, π)）→ 1、下半分（偏角が [π, 2π)）→ 2
    auto half = [](pll p){
        if(p.fst == 0 && p.snd == 0) return 0;
        return (p.snd > 0 || (p.snd == 0 && p.fst > 0)) ? 1 : 2;
    };

    if(half(p0) != half(p1)) return half(p0) < half(p1);
    // 同じ側にあるときは外積で判定（p0 × p1 > 0 ⇔ p0 の偏角の方が小さい）
    // 偏角が等しい（または両方 (0,0)）ときは cross == 0 で false（strict weak ordering）
    ll cross = p0.fst*p1.snd-p0.snd*p1.fst;
    return cross > 0;
}

bool is_same_arg(pll p0, pll p1){
    return !arg_comp(p0, p1) && !arg_comp(p1, p0);
}
