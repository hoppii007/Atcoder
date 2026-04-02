
//複数の長方形・長方体の共通部分の面積を出す
//デフォルトは値渡しになってるから注意！！！！！！！！！
//vs = pll{{1,2,3},{4,5,6}},...のように、左下、右上のN次元の座標が並んでいる
//一つだけ渡せばその一つの面積をだしてくれる
//ここが参考になるhttps://atcoder.jp/contests/abc343/tasks/abc343_e
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
