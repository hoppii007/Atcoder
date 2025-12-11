
//頂点番号 -> {行きがけ距離,帰りがけ+1}を記録する
//自分を含む部分木集合が[fst,snd)になる
//木であることは必要
vpll euler_tour(vvll &nexts, ll root){
    vpll ret(nexts.size(), {-1,-1});
    ll order = 0;
    std::function<void(ll)> dfs = [&](ll now){
        ret[now].fst = order++;
        for(ll next: nexts[now]) if(ret[next].fst == -1){
            dfs(next);
        }
        ret[now].snd = order;
    };
    dfs(root);
    return ret;
}
