
//O(E+V)でsからの最短経路がもとまる
//変のコストが0か1しかない場合は使える
//nextsには{cost,index}にしといてね
//始点が複数あって全部の中から最短が欲しい時は、ちょっと変更して最初から全部入れるようにしたほうがいい（何回も呼ばないほうがいい）
vll bfs01(vector<vpll> &nexts, vll starts){
    vll bfs01s(nexts.size(), inf);
    deque<pll> q;
    for(ll s: starts){
        bfs01s[s] = 0;
        q.push_front({0,s});
    }
    while(!q.empty()){
        pll now = q.front();
        q.pop_front();
        for(pll next: nexts[now.second]){
            ll ncost = bfs01s[now.second]+next.first;
            if(bfs01s[next.second] <= ncost) continue;
            bfs01s[next.second] = ncost;
            if(next.first == 0) q.push_front({ncost, next.second});
            else q.push_back({ncost, next.second});
        }
    }
    return bfs01s;
}
