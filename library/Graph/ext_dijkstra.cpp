
//O((E+V)logV)でsからの最短経路がもとまる

//負の辺があるとダメ
//nextsには{cost,index}にしといてね
//始点が複数あって全部の中から最短が欲しい時は、ちょっと変更して最初から全部入れるようにしたほうがいい（dijkstraは何回も呼ばないほうがいい）
//辺のコストが1しかないならbfsでOK
//辺のコストが0か1しかないならbfs01でOK
struct Node {
    ll energy;
    ll l;
    ll i,j;
    bool operator < (const Node &other) const {
        return tie(energy,l,i,j) < tie(other.energy,other.l,other.i,other.j);
    }
};
map<Node,ll> ext_dijkstra(map<Node, vector<pair<ll,Node>>> &nexts, vector<Node> starts){
    struct dijk_comp{
        bool operator()(const pair<ll,Node> &p1,const pair<ll,Node> &p2) const{
            return p1.first > p2.first;
        }
    };
    map<Node,ll> dijk;
    for(auto node: nexts) dijk[node.fst] = inf;

    priority_queue<pair<ll,Node>,vector<pair<ll,Node>>,dijk_comp> q;
    for(Node s: starts){
        dijk[s] = 0;
        q.push({0,s});
    }
    while(!q.empty()){
        pair<ll,Node> now = q.top();
        q.pop();
        //すでに更新されていれば無視
        if(now.first > dijk[now.second]) continue;
        for(pair<ll,Node> next: nexts[now.second]){
            ll ncost = next.first + now.first;
            if(dijk[next.second] > ncost){
                dijk[next.second] = ncost;
                q.push({ncost, next.second});
            }
        }
    }
    return dijk;
}
