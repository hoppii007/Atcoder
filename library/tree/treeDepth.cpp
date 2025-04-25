
//木の中で、rootからの距離を入れる
//O(N)
vll treeDepth(ll root, vvll &nexts){
    vll ret(nexts.size(), inf);
    std::function<void(ll,ll)> subDfs = [&](ll now, ll d){
        ret[now] = d;
        for(ll next: nexts[now]) if(ret[next] == inf) subDfs(next, d+1);
    };
    subDfs(root, 0);
    return ret;
}
