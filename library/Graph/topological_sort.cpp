
//DAG（るーぷをふくんじゃだめ）に対してトポロジカルソートし、根っこが一番左にくるようにする
//ループとかあったらバグるからマジやめてね
vll topological_sort(vvll &nexts){
    vvb canUse(nexts.size());
    rep(i,nexts.size()){
        rep(j,nexts[i].size()) canUse[i].push_back(true);
    }

    vll in(nexts.size());
    rep(i,nexts.size()) for(ll next: nexts[i]) in[next]++;

    //入次数が0の集合
    queue<ll> s;
    rep(i,nexts.size()) if(in[i] == 0) s.push(i);

    vll ret;

    while(!s.empty()){
        ll now = s.front();
        s.pop();
        ret.push_back(now);

        for(ll next: nexts[now]) {
            in[next]--;
            if(in[next] == 0) s.push(next);
        }
    }

    return ret;
}
