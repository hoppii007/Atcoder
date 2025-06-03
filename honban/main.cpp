#include<bits/stdc++.h>
//入力系
#define cinll(...) ll __VA_ARGS__; input(__VA_ARGS__);
#define cinint(...) int __VA_ARGS__; input(__VA_ARGS__);
#define cinstr(...) string __VA_ARGS__; input(__VA_ARGS__);
#define cinchar(...) char __VA_ARGS__; input(__VA_ARGS__);
#define cindouble(...) double __VA_ARGS__; input(__VA_ARGS__);
#define cinvll(a,n) vll a(n); rep(i,n) cin>>a[i];
#define cinvvll(a,n,m) vvll a(n,vll(m)); rep(i,n) rep(j,m) cin>>a[i][j];
#define cinvs(a,n) vs a(n); rep(i,n) cin>>a[i];
//繰り返し系
#define rep1(n) for(ll i=0;i<n;i++)
#define rep2(i,n) for(ll i=0;i<n;i++)
#define rep3(i,a,n) for(ll i=a;i<n;i++)
#define rep4(i,a,n,b) for(ll i=a;i<n;i+=b)
#define overload4(a,b,c,d,e,...) e
#define rep(...) overload4(__VA_ARGS__,rep4,rep3,rep2,rep1)(__VA_ARGS__)
#define mrep1(n) for(ll i=n;i>=0;i--)
#define mrep2(i,n) for(ll i=n;i>=0;i--)
#define mrep3(i,n,a) for(ll i=n;i>=a;i--)
#define mrep4(i,n,a,b) for(ll i=n;i>=a;i-=b)
#define mrep(...) overload4(__VA_ARGS__,mrep4,mrep3,mrep2,mrep1)(__VA_ARGS__)
//iterator系
#define all(a) a.begin(),a.end()
#define rall(a) a.rbegin(),a.rend()
//書くのが長いやつ
#define fst first
#define snd second
using namespace std;
//型系
using ll = long long;
using vll = vector<long long>;
using vvll = vector<vector<long long>>;
using vi = vector<int>;
using vvi = vector<vector<int>>;
using vb = vector<bool>;
using vvb = vector<vector<bool>>;
using vd = vector<double>;
using vvd = vector<vector<double>>;
using vc = vector<char>;
using vvc = vector<vector<char>>;
using vs = vector<string>;
using pll = pair<long long,long long>;
using pd = pair<double,double>;
using sll = set<long long>;
using vsll = vector<set<long long>>;
using vpll = vector<pair<long long,long long>>;
using vpd = vector<pair<double, double>>;
using vvpll  = vector<vector<pair<long long, long long>>>;
#define vmll vector<mll>
#define vvmll vector<vector<mll>>

//const ll mod = 998244353LL;
const ll mod = 1000000007LL;
const ll inf = 1301001001001001000LL;
const double PI=3.1415926535897932384626433832795028841971;

//表示
#define overload1(a,b,NAME,...) NAME
#define coutYesReturn() do {coutYes(); return 0; } while(0)
#define coutYesReturnIf(a) do { if(a){ coutYesReturn(); }} while(0)
#define coutNoReturn() do {coutNo(); return 0;} while(0)
#define coutNoReturnIf(a) do {if(a){ coutNoReturn(); }} while(0)
#define coutReturnIf(a,s) do{if(a){cout<<s<<endl; return 0;}}while(0)
template<typename... T>
void coutll(T... a){ ((cout << a <<" "),...) << endl; }
void coutvll(vll &a){ rep(i,a.size()) cout<<a[i]<<" "; cout<<endl; }
void coutvll(string name, vll &a){ cout<<name<<":"; coutvll(a); }
void coutvlln(vll &a){ rep(i,a.size()) cout<<a[i]<<endl; }
void coutYes(){ cout<<"Yes"<<endl; }
void coutNo(){ cout<<"No"<<endl; }
void coutYesNo(bool a){ cout<<(a?"Yes":"No")<<endl; }
void coutIf(bool a, string s, string t){ cout<<(a?s:t)<<endl; }
//入力
template<class... T>
void input(T&... a){
    (cin >> ... >> a);
}
//複数ソート
template<class... T>
void sorts(vector<T>&... a){
    (sort(all(a)),...);
}
template<typename T>
T chmax(T &a, T b){ if(a < b) a = b; return a; }
template<typename T>
T chmin(T &a, T b){ if(a > b) a = b; return a; }

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
//costの型ll（基本的にはllかdouble）
//nextsは{cost,index}
//実際の最小コストだけが欲しいときは、戻り値の型をllにして、minCostを返せばいい
ll kruskal(vector<vector<pair<ll,ll>>> &nexts){
    UnionFind uf(nexts.size());
    struct KruskalEdge{
        ll u,v;
        ll c;
    };
    vector<KruskalEdge> edges;
    rep(i, nexts.size()) for(pair<ll,ll> e: nexts[i]){
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
    ll minCost = 0;
    vector<vector<pair<ll,ll>>> ret(nexts.size());
    rep(i,edges.size()){
        if(!uf.same(edges[i].u, edges[i].v)){
            uf.unite(edges[i].u, edges[i].v);
            minCost += edges[i].c;
            ret[edges[i].u].push_back({edges[i].c, edges[i].v});
        }
    }
    //最小コストだけがほしいならここでminCostを返す
    return minCost;
}

struct S {
    ll i, x, y;
};

ll dist(S a, S b){
    return min(abs(a.x-b.x), abs(a.y-b.y));
}

int main(){
    cinll(n);
    UnionFind uf(n);
    vector<S> ps1(n);
    rep(i,n){
        cin>>ps1[i].x>>ps1[i].y;
        ps1[i].i = i;
    }
    vector<S> ps2 = ps1;
    //trueだったら第1引数が手前に来る
    //p1 < p2として作ると昇順になる
    //p2 < p1として作ると降順になる
    
    //sort(all(v), compf);みたいに使ってね
    std::function<bool(S&,S&)> compf = [&](const S &p1,const S &p2) {
        return pll{p1.x,p1.y} < pll{p2.x,p2.y};
    };
    //sort(all(v), compf);みたいに使ってね
    std::function<bool(S&,S&)> compf2 = [&](const S &p1,const S &p2) {
        return pll{p1.y, p1.x} < pll{p2.y, p2.x};
    };
    sort(all(ps1), compf);
    sort(all(ps2), compf2);

    vvpll nexts(n);
    rep(i,n-1){
        nexts[ps1[i].i].push_back({dist(ps1[i], ps1[i+1]), ps1[i+1].i});
        nexts[ps1[i+1].i].push_back({dist(ps1[i], ps1[i+1]), ps1[i].i});

        nexts[ps2[i].i].push_back({dist(ps2[i], ps2[i+1]), ps2[i+1].i});
        nexts[ps2[i+1].i].push_back({dist(ps2[i], ps2[i+1]), ps2[i].i});
    }
    
    cout<<kruskal(nexts)<<endl;



    
    return 0;
}