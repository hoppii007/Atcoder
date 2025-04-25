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

	vector<set<ll>> breaks;
	
	UnionFind(ll n) : par(n),kosu(n),Count(n) { //最初は全てが根であるとして初期化
		rep(i,n){
			par[i] = i;
			kosu[i] = 1;
		}
		breaks = vector<set<ll>>(n);
	}

	ll root(ll x) { // データxが属する木の根を再帰で得る：root(x) = {xの木の根}
		if (par[x] == x) return x;
		return par[x] = root(par[x]);
	}

	void unite(ll x, ll y) { // xとyの木を併合
		ll rx = root(x); //xの根をrx
		ll ry = root(y); //yの根をry
		if (rx == ry) return; //xとyの根が同じ(=同じ木にある)時はそのまま
		
		if(breaks[rx].size() < breaks[ry].size()){
			for(ll b: breaks[rx].size()) breaks[ry].insert(b);
			par[rx] = ry; //xとyの根
			kosu[ry] += kosu[rx];
			breaks[rx] = set<ll>();
		}
		else{
			for(ll b: breaks[ry].size()) breaks[rx].insert(b);
			par[ry] = rx; //xとyの根
			kosu[rx] += kosu[ry];
			breaks[ry] = set<ll>();
		}
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

vpll canMoveKinbo(vvc &s, ll i, ll j){
	vpll ret;
	if(i-1 >= 0 && s[i-1][j] == '.') ret.push_back({i-1,j});
	if(j-1 >= 0 && s[i][j-1] == '.') ret.push_back({i,j-1});
	if(i+1 < s.size() && s[i+1][j] == '.') ret.push_back({i+1,j});
	if(j+1 < s[0].size() && s[i][j+1] == '.') ret.push_back({i,j+1});
	return ret;
}

//trueだったら第1引数が手前に来る
//p2 < p1として作ると降順になる
//p1 < p2として作ると昇順になる

///////////////////////////////////////
//priority_queueは後ろからとることに注意//
///////////////////////////////////////

//mainに以下をコピー（型を入力）
//pushとtopとpopを使うからね

struct comp{
	bool operator()(const pll &p1,const pll &p2) const{
		return p1.first > p2.first;
	}
};

//1回前下痢したら行けるところ

vpll inKinbos(vvc &s, ll i, ll j){
	vpll ret;
	if(i-1 >= 0 ) ret.push_back({i-1,j});
	if(j-1 >= 0) ret.push_back({i,j-1});
	if(i+1 < s.size()) ret.push_back({i+1,j});
	if(j+1 < s[0].size()) ret.push_back({i,j+1});
	return ret;
}

int main(void){

	return 0;
}
