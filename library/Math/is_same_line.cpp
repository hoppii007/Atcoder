/**
 * @brief 3点が同一直線上にあるかを判定する
 * @details 各点を (x, y) = (fst, snd) として扱う。
 *          まず y 座標が一致するケース（水平線・水平線上にない外れ点）を先に処理し、
 *          残りは「点間の傾き」を分数 (dy, dx) のまま外積で比較する。
 *          除算しないため誤差なし、垂直線（dx = 0）も同様に扱える。
 *          乗算のオーバーフロー対策として __int128 にキャストして比較する。
 * @param p1 1つ目の点 (x, y)
 * @param p2 2つ目の点 (x, y)
 * @param p3 3つ目の点 (x, y)
 * @return 3点が同一直線上にあれば true、そうでなければ false
 * @note 計算量: O(1)
 * @note 座標値の絶対値は ll の範囲内であることを想定（差分の積を __int128 で受ける）
 */
bool is_same_line(pll p1, pll p2, pll p3){
    // 3点とも y が一致 → 水平な同一直線上
    if(p1.snd == p2.snd && p2.snd == p3.snd) return true;

    // 2点だけ y が一致 → 残りの1点はその水平線上にないので非共線
    if(p1.snd == p2.snd && p2.snd != p3.snd) return false;
    if(p1.snd == p3.snd && p3.snd != p2.snd) return false;
    if(p2.snd == p3.snd && p3.snd != p1.snd) return false;

    // ここに来る時点で 3点の y は全て異なる
    // 「一番下 → 真ん中 → 一番上」の順に並べてから傾きを比較する想定の前処理
    vpll v = {p1, p2, p3};
    sort(all(v), [](const pll &p,const  pll &q){ return p.snd < q.snd; });

    // p1 → p2 の傾き（分子: dy, 分母: dx）
    pll p12 = {p2.snd-p1.snd, p2.fst-p1.fst};
    // p2 → p3 の傾き（分子: dy, 分母: dx）
    pll p23 = {p3.snd-p2.snd, p3.fst-p2.fst};

    // dy12 / dx12 == dy23 / dx23  ⇔  dy12 * dx23 == dx12 * dy23
    if((__int128)(p12.fst*p23.snd) == (__int128)(p12.snd*p23.fst)) return true;
    return false;
}