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
#define cinvpll(a,n) vpll a(n); rep(i,n) cin>>a[i].fst>>a[i].snd;
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
//vector系
#define pb push_back
//書くのが長いやつ
#define fst first
#define snd second
using namespace std;
//型系
using ll = long long;
using vll = vector<ll>;
using vvll = vector<vll>;
using vi = vector<int>;
using vvi = vector<vi>;
using vb = vector<bool>;
using vvb = vector<vb>;
using vd = vector<double>;
using vvd = vector<vd>;
using vc = vector<char>;
using vvc = vector<vc>;
using vs = vector<string>;
using pll = pair<long long,long long>;
using pi = pair<int,int>;
using pd = pair<double,double>;
using sll = set<long long>;
using vsll = vector<sll>;
using vpll = vector<pll>;
using vpi = vector<pi>;
using vpd = vector<pd>;
using vvpll  = vector<vpll>;

const ll mod = 998244353LL;
// const ll mod = 1000000007LL;
// const ll mod = 10007;
const ll inf = 1300100100100100100LL;
// const ll inf = 1001001001LL;
const double PI=3.1415926535897932384626433832795028841971;

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
//便利関数
template<typename T> bool chmax(T &a, T b){ if(a < b) {a = b; return true;} return false; }
template<typename T> bool chmin(T &a, T b){ if(a > b) {a = b; return true;} return false; }

//配列表示用
template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) { 
    rep(i,vec.size()) os << vec[i] << (i==(ll)vec.size()-1?"":" ");
    return os;
}
// pair 表示用
template <typename T, typename S>
std::ostream& operator<<(std::ostream& os, const std::pair<T,S>& p) { 
    os << p.fst << " " << p.snd;
    return os;
}


/*
**どの問題?**

**difficulty**

**solved**
(自力AC / 解説AC / 未AC)

**felt**
（簡単 / 中 / 難しい）

**考察ノート**

**難所**

## 問題全体のメモ
*/


// --------------------------- ここまでテンプレ ----------------------------

/*
**どの問題?**
ABC218E

**difficulty**
1004

**solved**
(自力AC / 解説AC / 未AC)

**felt**
（簡単 / 中 / 難しい）

**考察ノート**
最小全域木みたいにできそう
最小全域木は、使う辺のコストの総和を最小化するアルゴリズム
今回の場合、辺を取り除くのにコストがかかって、それを最大化したいから、やっぱり使う辺のコストの総和を最小化したい。
負の辺が使われてるとまずいけど、全部の辺から最小コストを引いた後（つまり、最小コストがマイナスの場合果たすことになる。最小コストを0基準にしてコストを振り直す感じ）
木を構成すると辺の本数はN-1本となるから、コストの総和 + 最小コスト*(N-1)でよさそう

**難所**

## 問題全体のメモ
*/

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
// O(E log E)
//無向グラフだったとしても有向グラフとして作るから、同じu,vを結ぶ編は一つしか出てこない
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
			ret[edges[i].v].push_back({edges[i].c, edges[i].u});
		}
	}
	//最小コストだけがほしいならここでminCostを返す
	return minCost;
}

int main(){
	cinll(n, m);
	vll a(m), b(m), c(m);
	rep(i, m) cin >> a[i] >> b[i] >> c[i];
	vvpll nexts(n);
	rep(i, m){
		chmax(c[i], 0LL);
		nexts[a[i]-1].pb({c[i], b[i]-1});
		nexts[b[i]-1].pb({c[i], a[i]-1});
	}
	cout << accumulate(all(c), 0LL) - (kruskal(nexts)) << endl;
	return 0;
}