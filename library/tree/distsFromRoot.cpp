
//木に対してやるやつ
//頂点sからの全部の距離
//distsFromRoot(nexts, dists, s)と呼び出す
//distsは最初全て-1にしとく
vll distsFromRoot(vvll &nexts, ll s){
    vll dists(nexts.size(), -1);
    auto distsFromRootDfs = [&](vvll &ns, ll now, ll d=0){
        for(ll next: ns[now]) if(dists[next] == -1) {
            dists[next] = d+1;
            distsFromRootDfs(ns, next, d+1);
        }
    };
    dists[s] = 0;
    distsFromRootDfs(nexts, s);
    return dists;
}
