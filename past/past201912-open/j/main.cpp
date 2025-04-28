#include<bits/stdc++.h>
//入力系
#define cinll(...) ll __VA_ARGS__; input(__VA_ARGS__);
#define cinill(...) ll __VA_ARGS__; input_index(__VA_ARGS__);
#define cinint(...) int __VA_ARGS__; input(__VA_ARGS__);
#define ciniint(...) int __VA_ARGS__; input_index(__VA_ARGS__);
#define cinstr(...) string __VA_ARGS__; input(__VA_ARGS__);
#define cinchar(...) char __VA_ARGS__; input(__VA_ARGS__);
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
using namespace std;
//型系
using ll = long long;
using vll = vector<long long>;
using vvll = vector<vector<long long>>;
using vi = vector<int>;
using vvi = vector<vector<int>>;
using vb = vector<bool>;
using vvb = vector<vector<bool>>;
using vc = vector<char>;
using vvc = vector<vector<char>>;
using vs = vector<string>;
using pll = pair<long long,long long>;
using vpll = vector<pair<long long,long long>>;

const ll mod = 998244353LL;
const ll inf = 100100100100100100LL;
const double PI=3.1415926535897932384626433832795028841971;

//便利系
//vectorをjoinするやつ
template<typename T>
string join(const vector<T> &a,string c){
	string ret = "";
	if(a.size()==0) return ret;
	
    //もともとstringなら
	if constexpr (std::is_same_v<T, std::string> || std::is_same_v<T, char>) {
	    rep(i,a.size()-1) ret += a[i]+c;
	    return ret + a[a.size()-1];
    }
    //もともとstringじゃなければto_stringして結合
	else{
	    rep(i,a.size()-1) ret += to_string(a[i]) + c;
	    return ret + to_string(a[a.size()-1]);
	}
	
}
//空白区切り表示
template<class T, class... Ts>
void out(const T& a,const Ts&... b){
	cout<<a;
	(cout<< ... << (cout << ' ',b));
	cout<<endl;
}
template<class T>
void out(const vector<T> &a){
    out(join(a," "));
}
//入力
template<class... T>
void input(T&... a){
	(cin >> ... >> a);
}

template<class... T>
void input_index(T&... a){
    (cin >> ... >> a);
    ((a--), ...);
}

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

bool allSame(UnionFind &uf, vll vs){
	rep(i,(ll)vs.size()-1) if(!uf.same(vs[i], vs[i+1])) return false;
	return true;
}


//O((E+V)logV)でsからの最短経路がもとまる
//負の辺があるとダメ
//nextsには{cost,index}にしといてね
//始点が複数あって全部の中から最短が欲しい時は、ちょっと変更して最初から全部入れるようにしたほうがいい（dijkstraは何回も呼ばないほうがいい）
//辺のコストが1しかないならbfsでOK
//辺のコストが0か1しかないならbfs01でOK
vll dijkstra(vector<vpll> &nexts, vll starts){
	struct dijk_comp{
		bool operator()(const pll &p1,const pll &p2) const{
			return p1.first > p2.first;
		}
	};
	vll dijk(nexts.size(), inf);
	priority_queue<pll,vector<pll>,dijk_comp> q;
	for(ll s: starts){
		dijk[s] = 0;
		q.push({0,s});
	}
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

//縦0以上h未満、横0以上w未満の(i,j)の4近傍をとる
vpll kinbo4(ll h,ll w, ll i,ll j){
	vpll ret;
	if(i-1 >= 0) ret.push_back({i-1,j});
	if(j-1 >= 0) ret.push_back({i,j-1});
	if(i+1 < h) ret.push_back({i+1,j});
	if(j+1 < w) ret.push_back({i,j+1});
	return ret;
}

int main(void){
	cinll(h,w);
	cinvvll(a,h,w);

	std::function<pll(ll)> toInd = [&](ll n){
		return pll({n/w, n%w});
	};
	std:: function<ll(pll)> toLL = [&](pll p){
		return p.first*w + p.second;
	};

	vector<vpll> nexts(h*w);
	rep(i,h) rep(j,w) {
		vpll ks = kinbo4(h,w, i,j);
		for(pll k: ks){
			nexts[toLL({i,j})].push_back({a[k.first][k.second],toLL(k)});
			nexts[toLL(k)].push_back({a[i][j],toLL({i,j})});
		}
	}

	vll dijk1 = dijkstra(nexts, {toLL({h-1,0})});
	vll dijk2 = dijkstra(nexts, {toLL({h-1,w-1})});
	vll dijk3 = dijkstra(nexts, {toLL({0,w-1})});
	ll ans = inf;
	rep(i,h) rep(j,w){
		ans = min(ans, dijk1[toLL({i,j})]+dijk2[toLL({i,j})]+dijk3[toLL({i,j})]-2*a[i][j]);
	}
	cout<<ans<<endl;
	
	
    
	return 0;
}
