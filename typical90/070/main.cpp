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

ll solve(vpll &xys, pll p){
	ll ret = 0;
	rep(i,xys.size()){
		ret += abs(xys[i].first-p.first) + abs(xys[i].second-p.second);
	}
	return ret;
}

pll ave(vpll &xys){
	pll ret = {0,0};
	ll maxx=-inf, maxy=-inf, minx=inf, miny = inf;
	rep(i,xys.size()){
		maxx = max(maxx, xys[i].first);
		minx = min(minx, xys[i].first);
		maxy = max(maxy, xys[i].second);
		miny = min(miny, xys[i].second);
	}
	return {(maxx-minx)/2, (maxy-miny)/2};
}

//考察
//xよりx+1のスコアの方が小さくなるような最大のxが分かれば、x+1のスコアが最大
//そのためには、点をxに置いた時のスコアを高速に計算できればいい。

ll score(vll &xs, ll x, vll &xsr){
	ll ret = 0;
	//x以下の奴らに対して、x-xs[i]の合計を取る
	auto it = upper_bound(all(xs), x);
	ll idx = it-xs.begin();
	ret += x*idx - xsr[idx];
	//xより大きい奴らに対して、xs[j]-xの合計を取る
	//最後から、idx版目を含むところまでの合計が欲しい
	ret += xsr[xs.size()]-xsr[idx] - x*((ll)xs.size()-idx);
	return ret;
}

int main(void){
    cinll(n);
	vll xs(n), ys(n);
	rep(i,n) cin>>xs[i]>>ys[i];
	sort(all(xs));
	sort(all(ys));
	vll xsr(n+1,0), ysr(n+1,0);
	rep(i,n) xsr[i+1] = xs[i]+xsr[i];
	rep(i,n) ysr[i+1] = ys[i]+ysr[i];

	//score(x) > score(x+1)となるところの最大を探す
	//二分探索のジャッジ用のやつ
	auto nibutan_judge = [&](ll mid){
		return score(xs, mid, xsr) > score(xs, mid+1, xsr);
	};
	auto nibutan_judge2 = [&](ll mid){
		return score(ys, mid, ysr) > score(ys, mid+1, ysr);
	};
	ll ok = -inf;
	ll ng = inf;
	while(abs(ok-ng) > 1){
		ll mid = (ok+ng)/2;
		if(nibutan_judge(mid)) ok = mid;
		else ng = mid;
	}
	ll xok = ok+1;

	ok = -inf;
	ng = inf;
	while(abs(ok-ng) > 1){
		ll mid = (ok+ng)/2;
		if(nibutan_judge2(mid)) ok = mid;
		else ng = mid;
	}
	ll yok = ok+1;
	cout<<score(xs, xok, xsr) + score(ys, yok, ysr)<<endl;

	return 0;
}
