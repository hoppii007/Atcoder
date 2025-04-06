
//O((E+V)logV)でsからの最短経路がもとまる
//負の辺があるとダメ
//nextsには{cost,index}にしといてね
vll dijkstra(vector<vpll> &nexts, ll s){
    struct dijk_comp{
        bool operator()(const pll &p1,const pll &p2) const{
            return p1.first > p2.first;
        }
    };
    vll dijk(nexts.size(), inf);
    priority_queue<pll,vector<pll>,dijk_comp> q;
    dijk[s] = 0;
    q.push({0,s});
    while(!q.empty()){
        pll now = q.top();
        q.pop();
        //すでに更新されていれば無視
        if(now.first > dijk[now.second]) continue;
        for(pll next: nexts[now.second]){
            ll ncost = next.first + now.first;
            if(dijk[next.second] > ncost){
                dijk[next.second] = ncost;
                q.push({ncost, next.second});
            }
        }
    }
    return dijk;
}
