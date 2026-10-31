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

// 頂点0から、bfsでDAGを作る
vvll make_dag(vvpll& nexts){
	//型を入力
	queue<ll> q;

	vll d(nexts.size(), inf);
	
	//ここでスタート地点を入れてく
	q.push({0});
	d[0] = 0;
	
	while(!q.empty()){
		ll now = q.front();
		q.pop();
	
		//ここで次の奴らを追加していく
		for(auto [c, nxt]: nexts[now]) if(d[nxt] == inf){
			d[nxt] = d[now] + 1;
			q.push(nxt);
		}
	}

	vvll ret(nexts.size());
	rep(i, nexts.size()){
		for(auto [c, nxt]: nexts[i]) if(d[i]+1 == d[nxt]) ret[i].pb(nxt);
	}
	return ret;
}

// O(N log N) で、順序関係(<)が定義されたT配列に対して、
// r[i] := T[i]のランク(0-indexed)となるrを返す
// ランクは昇順
template<typename T>
vll ranking(vector<T> a){
	vector<pair<T,int>> b(a.size());
	rep(i, a.size()) b[i] = {a[i], i};
	sort(all(b));

	vll ret(a.size());
	ll r = 0;
	ret[b[0].snd] = r;
	rep(i, 1, a.size()){
		if(b[i-1].fst < b[i].fst) r++;
		ret[b[i].snd] = r;
	}

	return ret;
}

int main(){
	cinll(n, m);
	vvpll nexts(n);
	rep(i, m){
		cinll(a, b, c);
		a--; b--;
		nexts[a].pb({c, b});
	}

	vvll dag = make_dag(nexts);

	// 前の層の{頂点, ランク}
	vpll prevs = {{0,0}};

	set<ll> visit;
	visit.insert(0);
	
	vll ans(n);
	ans[0] = 0;

	while(visit.size() < n){
		map<ll,pll> vnxt;
		set<ll> nvisit;
		// 前の層から伸びるすべての辺に対して行う
		for(auto [p, r]: prevs){
			for(auto [c, nxt]: nexts[p]) if(visit.count(nxt) == 0){
				nvisit.insert(nxt);
				if(vnxt.count(nxt) == 0) {
					vnxt[nxt] = {r, c};
					ans[nxt] = (ans[p]*10 + c)%mod;
				}
				else {
					if(vnxt[nxt] > pll{r, c}){
						vnxt[nxt] = {r, c};
						ans[nxt] = (ans[p]*10+c)%mod;
					}
				}
			}
		}
		vpll pnxt;
		for(ll nxt: nvisit) pnxt.pb(vnxt[nxt]);
		vll rank = ranking(pnxt);
		ll idx = 0;
		vpll nexts;
		for(ll nv: nvisit){
			nexts.pb({nv, rank[idx]});
			idx++;
		}
		for(ll v: nvisit){
			visit.insert(v);
		}
		prevs = nexts;
	}
	cout << ans << endl;

	return 0;
}