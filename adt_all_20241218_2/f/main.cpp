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

//四つの頂点の情報をもらい、正方形かどうかを判別する
//overlapIsOutがtrueなら、ぴったりなってる時ダメにする
bool isSquare(pll a,pll b,pll c,pll d, bool overlapIsOut = true){
	vpll ps = {a,b,c,d};
	sort(all(ps));
	swap(ps[2], ps[3]);
	//長さチェック
	ll dx = ps[3].first-ps[0].first;
	ll dy = ps[3].second-ps[0].second;
	ll len = dx*dx + dy*dy;
	if(overlapIsOut && len == 0) return false; 
	rep(i,ps.size()){
		dx = ps[(i+1)%4].first - ps[i].first;
		dy = ps[(i+1)%4].second - ps[i].second;
		if(dx*dx + dy*dy != len) return false;
	}
	//角度チェック
	rep(i,ps.size()){
		ll dx1 = ps[(i+1)%4].first - ps[i].first;
		ll dy1 = ps[(i+1)%4].second - ps[i].second;
		ll dx2 = ps[(i+2)%4].first - ps[(i+1)%4].first;
		ll dy2 = ps[(i+2)%4].second - ps[(i+1)%4].second;
		if(dy1 * dy2 != -dx1 * dx2) return false;
	}
	return true;
}

bool isAllHash(string s, vpll ps){
	for(pll p: ps) if(s[p.first][p.second] != '#') return false;
	return true;
}

//座標圧縮した後の座標が返ってくる
map<ll,ll> zaatu(vll &a){
	set<ll> s;
	rep(i,a.size()) s.insert(a[i]);
	map<ll,ll> ret;
	ll i = 0;
	for(ll t: s) ret[t] = i++;
	return ret;
}

int main(void){
    cinvs(s,9);
	
	//bit全探索
	//要素数n）
	rep(biti,1LL<<n){
		
		rep(bitj,n){
			//1のところ
			if(biti&(1LL<<bitj)){
	
			}
			//0のところ
			else{
	
			}
		}
	}
	return 0;
}
