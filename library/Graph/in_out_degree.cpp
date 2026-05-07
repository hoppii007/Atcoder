/**
 * @brief 入次数配列
 * @param nexts 隣接リスト（nexts[i] は頂点 i から伸びる辺の行き先一覧）
 * @return 各頂点の入次数を格納した配列（ret[i] = 頂点 i に入る辺の本数）
 * @note 計算量: O(V + E)
 */
vll in_degrees(vvll& nexts){
    ll n = nexts.size();
    vll ret(n, 0);
    rep(i, n) rep(j, nexts[i].size()) ret[nexts[i][j]]++;
    return ret;
}

/**
 * @brief 出次数配列
 * @param nexts 隣接リスト（nexts[i] は頂点 i から伸びる辺の行き先一覧）
 * @return 各頂点の出次数を格納した配列（ret[i] = 頂点 i から出る辺の本数）
 * @note 計算量: O(V)
 */
vll out_degrees(vvll& nexts){
    ll n = nexts.size();
    vll ret(n, 0);
    rep(i, n) ret[i] = nexts[i].size();
    return ret;
}