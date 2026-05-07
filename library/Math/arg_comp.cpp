
/**
 * @brief 偏角ソート用比較関数
 * @details 2 次元ベクトル p0, p1 の偏角を比較し、arg(p0) < arg(p1) なら true を返す。
 *          偏角は正の x 軸を 0 とし、反時計回りに [0, 2π) の範囲で測る。
 *          上半分（y > 0、または y == 0 かつ x > 0）と下半分（それ以外）に分け、
 *          同じ側にある場合は外積の符号で大小を判定する。
 *          偏角が等しいときは false を返す（strict weak ordering）。
 *          ゼロベクトル (0,0) は下半分として扱われる。
 * @param p0 比較対象のベクトル 1（first = x, second = y）
 * @param p1 比較対象のベクトル 2（first = x, second = y）
 * @return arg(p0) < arg(p1) なら true、そうでなければ false
 * @note 計算量: O(1)。座標値は ll の範囲で外積計算がオーバーフローしないことを呼び出し側で保証する。
 */
bool arg_comp(pll p0, pll p1) {
    // 上半分（偏角が [0, π) の範囲）にあるかを判定
    function<bool(pll)> is_upper = [](pll p){ return (p.snd > 0 || (p.snd == 0 && p.fst > 0)); };

    if(is_upper(p0) && !is_upper(p1)) return true;
    if(!is_upper(p0) && is_upper(p1)) return false;
    // 同じ側にあるときは外積で判定（p0 × p1 > 0 ⇔ p0 の偏角の方が小さい）
    ll cross = p0.fst*p1.snd-p0.snd*p1.fst;
    // 偏角が等しいときは false（strict weak ordering）
    if(cross == 0) return false;
    else return cross > 0;
}

/**
 * @brief 2 つのベクトルの偏角が等しいかを判定
 * @details arg_comp による strict weak ordering の同値関係として判定する。
 *          すなわち arg_comp(p0, p1) も arg_comp(p1, p0) も false なら同じ偏角とみなす。
 *          実質的には p0, p1 が同じ向き（正のスカラー倍の関係）であることに対応する。
 *          ゼロベクトル同士は同じ（下半分扱い）と判定される。
 * @param p0 比較対象のベクトル 1（first = x, second = y）
 * @param p1 比較対象のベクトル 2（first = x, second = y）
 * @return arg(p0) == arg(p1) なら true、そうでなければ false
 * @note 計算量: O(1)。
 */
bool is_same_arg(pll p0, pll p1){
    return !arg_comp(p0, p1) && !arg_comp(p1, p0);
}
