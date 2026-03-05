
// path: s -> p1 -> p2 -> ... -> gにたいして、その合計コストを計算する
// O(V + E)
ll path_to_costs(vvpll& nexts, vll& path){
    ll ret = 0;
    rep(i, (ll)path.size()-1) {
        for(pll p: nexts[path[i]]) if(p.snd == path[i+1]) ret += p.fst;
    }
    return ret;
}
