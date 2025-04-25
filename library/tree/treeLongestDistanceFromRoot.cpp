
//木に対して、頂点から最も遠い点（複数ある場合はどれか一つ）の{距離、番号}を返す
//longestDistanceFromRoot(nexts, s)のように使う
pll longestDistanceFromRoot(vvll &nexts, ll now, ll prev=-1){
    ll ret = 0;
    ll reti = now;
    for(ll next: nexts[now]) if(next != prev){
        pll nl = longestDistanceFromRoot(nexts, next, now);
        if(nl.first+1 >= ret){
            ret = nl.first+1;
            reti = nl.second;
        }
    }
    return {ret, reti};
}
