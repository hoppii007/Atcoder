#pragma once
/**
 * @brief 入次数・出次数の配列
 *
 *   in_degrees(nexts)  … ret[i] = 頂点 i に入る辺の本数  O(V+E)
 *   out_degrees(nexts) … ret[i] = 頂点 i から出る辺の本数  O(V)
 *
 * @code
 * vvll nexts(n);
 * vll in = in_degrees(nexts), out = out_degrees(nexts);
 * @endcode
 */
vll in_degrees(vvll& nexts){
    ll n = nexts.size();
    vll ret(n, 0);
    rep(i, n) rep(j, nexts[i].size()) ret[nexts[i][j]]++;
    return ret;
}

vll out_degrees(vvll& nexts){
    ll n = nexts.size();
    vll ret(n, 0);
    rep(i, n) ret[i] = nexts[i].size();
    return ret;
}