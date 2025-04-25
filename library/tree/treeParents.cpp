
//木の中で、rootを指定して、各頂点のparentsを設定する
//親の親は自分自身にしておく
//O(N)
vll treeParents(ll root, vvll &nexts){
    vll ret(nexts.size(), inf);
    std::function<void(ll,ll)> subDfs = [&](ll now, ll parent){
        ret[now] = parent;
        for(ll next: nexts[now]) if(ret[next] == inf) subDfs(next, now);
    };
    //親の親を-1にしたりしたいときはここの第二引数を変更
    //infにするときは注意
    subDfs(root, -1);
    return ret;
}
