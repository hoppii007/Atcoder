
//単一始点から、全頂点までの最短距離がわかる
//nextsは{cost, idx}
//O(|V|*|E|)
//負の閉路があるか？の判定も可能
//まだ試してないから怖いっす...
vll bellman_ford(vvpll &nexts, vll starts){
    vll ret(nexts.size(), inf);
    for(ll s: starts) ret[s] = 0;
    ll cnt = 0;
    while(cnt < nexts.size()) {
        bool isEnd = true;
        rep(i,nexts.size()){
            for(pll next: nexts[i]){
                if(ret[next.snd] > ret[i]+next.fst){
                    isEnd = false;
                    ret[next.snd] = ret[i]+next.fst;
                }
            }
        }
    }
    //ここでcnt == nexts.size()となっていれば負の閉路が存在する
    return ret;
}
