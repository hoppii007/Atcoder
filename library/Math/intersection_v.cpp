#pragma once
/**
 * @brief 複数の直方体の共通部分の体積
 *
 *   Pos{x, y, z}        … 3次元の点
 *   intersection_v(vs) … vs の全直方体の共通部分の体積（共通部分なしなら 0）  O(|vs|)
 *
 * @param vs 各直方体の {小さい角, 大きい角} の配列（値渡し）
 * @note 辺の長さは「大きい角 − 小さい角」。1 個だけ渡せばその体積
 * @note 長方形の面積がほしいときは z を {0, 1} にする
 *
 * @code
 * // 参考: https://atcoder.jp/contests/abc343/tasks/abc343_e
 * vector<pair<Pos,Pos>> vs = {{{0,0,0},{7,7,7}}, {{3,3,3},{10,10,10}}};
 * ll v = intersection_v(vs);  // 4*4*4 = 64
 * @endcode
 */
struct Pos {
    ll x,y,z;
};
ll intersection_v(vector<pair<Pos,Pos>> vs){
    //でかい方の最小を取る
    Pos mins = vs[0].snd;
    rep(i,1,vs.size()){
        chmin(mins.x, vs[i].snd.x);
        chmin(mins.y, vs[i].snd.y);
        chmin(mins.z, vs[i].snd.z);
    }
    //小さい方の最大を取る
    Pos maxs = vs[0].fst;
    rep(i,1,vs.size()){
        chmax(maxs.x, vs[i].fst.x);
        chmax(maxs.y, vs[i].fst.y);
        chmax(maxs.z, vs[i].fst.z);
    }
    ll intersectionX = max(0LL, (mins.x-maxs.x));
    ll intersectionY = max(0LL, (mins.y-maxs.y));
    ll intersectionZ = max(0LL, (mins.z-maxs.z));
    return intersectionX * intersectionY * intersectionZ;
}
