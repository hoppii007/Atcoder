
struct UnionFind {
public:
    vll par; // par[i]:iの親の番号　(例) par[3] = 2 : 3の親が2
    vll kosu;
    ll Count; //全体として何個の集合があるか
    
    UnionFind(ll n) : par(n),kosu(n),Count(n) { //最初は全てが根であるとして初期化
        rep(i,n){
            par[i] = i;
            kosu[i] = 1;
        }
    }

    ll root(ll x) { // データxが属する木の根を再帰で得る：root(x) = {xの木の根}
        if (par[x] == x) return x;
        return par[x] = root(par[x]);
    }

    void unite(ll x, ll y) { // xとyの木を併合
        ll rx = root(x); //xの根をrx
        ll ry = root(y); //yの根をry
        if (rx == ry) return; //xとyの根が同じ(=同じ木にある)時はそのまま
        par[rx] = ry; //xとyの根
        kosu[ry] += kosu[rx];
        Count--;
    }

    bool same(ll x, ll y) { // 2つのデータx, yが属する木が同じならtrueを返す
        ll rx = root(x);
        ll ry = root(y);
        return rx == ry;
    }
    
    ll size(ll x){
        return kosu[root(x)];
    }
    ll size(){
        return Count;
    }
};

//0 ~ N-1までのグラフ（連結）の中で、最小全域木を作る
//costの型$1（基本的にはllかdouble）
//nextsは{cost,index}
//実際の最小コストだけが欲しいときは、戻り値の型をllにして、minCostを返せばいい
vector<vector<pair<$1,ll>>> kruskal(vector<vector<pair<$1,ll>>> &nexts){
    UnionFind uf(nexts.size());
    struct KruskalEdge{
        ll u,v;
        $1 c;
    };
    vector<KruskalEdge> edges;
    rep(i, nexts.size()) for(pair<$1,ll> e: nexts[i]){
        edges.push_back({i, e.second, e.first});
    }
    //trueだったら第1引数が手前に来る
    //p1 < p2として作ると昇順になる
    //p2 < p1として作ると降順になる
    
    //sort(all(v), compf);みたいに使ってね
    std::function<bool(KruskalEdge&, KruskalEdge&)> compf = [&](const KruskalEdge &p1,const KruskalEdge &p2) {
        return p1.c < p2.c;
    };
    
    sort(all(edges), compf);
    $1 minCost = 0;
    vector<vector<pair<$1,ll>>> ret(nexts.size());
    rep(i,edges.size()){
        if(!uf.same(edges[i].u, edges[i].v)){
            uf.unite(edges[i].u, edges[i].v);
            minCost += edges[i].c;
            ret[edges[i].u].push_back({edges[i].c, edges[i].v});
        }
    }
    //最小コストだけがほしいならここでminCostを返す
    return ret;
}
