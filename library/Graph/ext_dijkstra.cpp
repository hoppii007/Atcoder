#pragma once
/**
 * @brief 拡張ダイクストラ（頂点を構造体 Node で表す版）
 *
 *   Node{energy, l, i, j}           … 頂点（問題に合わせてメンバを書き換えて使う）
 *   ext_dijkstra(nexts, starts)     … starts からの最短距離 map<Node, ll>（到達不能は inf）  O((V+E) log V)
 *
 * @param nexts 隣接リスト nexts[u] = {{cost, v}, ...}（map<Node, vector<pair<ll,Node>>>）
 * @note 負の辺は不可。nexts のキーに無く到達もしない頂点は返り値に含まれない（dist.count で確認）
 *
 * @code
 * map<Node, vector<pair<ll,Node>>> nexts;
 * Node a{0, 0, 0, 0}, b{0, 0, 0, 1};
 * nexts[a].pb({1, b});
 * map<Node,ll> dist = ext_dijkstra(nexts, {a});
 * ll d = dist.count(b) ? dist[b] : inf;
 * @endcode
 */
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
            auto it = dijk.find(next.second);
            if(it == dijk.end() || it->second > ncost){
                dijk[next.second] = ncost;
                q.push({ncost, next.second});
            }
        }
    }
    return dijk;
}
