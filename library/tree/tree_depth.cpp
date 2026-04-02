
//木の中で、rootからの距離を入れる
//O(N)
vll tree_depth(ll root, vvll &nexts){
    vll ret(nexts.size(), inf);
    std::function<void(ll,ll)> sub_dfs = [&](ll now, ll d){
        ret[now] = d;
        for(ll next: nexts[now]) if(ret[next] == inf) sub_dfs(next, d+1);
    };
    sub_dfs(root, 0);
    return ret;
}
