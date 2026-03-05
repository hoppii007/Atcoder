
bool have_cycle(vvll &nexts){
    vb visit(nexts.size(), false);
    vb finished(nexts.size(), false);

    function<bool(ll)> dfs = [&](ll now){
        visit[now] = true;
        for(ll nxt: nexts[now]){
            if(visit[nxt] == false) {
                if(dfs(nxt)) return true;
            }
            else if(finished[nxt] == false) return true;
        }
        finished[now] = true;
        return false;
    };

    rep(i, nexts.size()) if(visit[i] == false) if(dfs(i)) return true;
    return false;
}