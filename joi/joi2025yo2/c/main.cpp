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

ll max(vll a){
	ll ret = -inf;
	rep(i,a.size()) if(ret < a[i]) ret = a[i];
	return ret;
}

ll min(vll a){
	ll ret = inf;
	rep(i,a.size()) if(ret > a[i]) ret = a[i];
	return ret;
}

ll neary(vll &a, ll t){
	auto it = upper_bound(all(a), t);
	ll ret = inf;
	if(it != a.end()){
		if(abs(t-ret) > abs(t - *it)) ret = *it;
	}
	if(it != a.begin()){
		it--;
		if(abs(t-ret) > abs(t - *it)) ret = *it;
	}
	return ret;
}

int main(void){
    cinll(x,y,z,p);
	cinvll(a,x);
	cinvll(b,y);
	cinvll(c,z);
	sort(all(a));
	sort(all(b));
	sort(all(c));
	ll maa = max(a), mia = min(a), mac=max(c), mic=min(c);
	ll ans = -inf;
	if(mac == mic){
		rep(i,x){
			ll al = a[i]+mac;
			ll bob = neary(b, p-al);
			ans = max(ans, abs(p-al-bob));
		}
		cout<<ans<<endl;
		return 0;

	}
	rep(i,x){
		//二分探索のジャッジ用のやつ
		auto nibutan_judge = [&](ll mid){
			//b[mid]で、左側が採択されるならtrue
			ll s1 = abs(p-a[i]-b[mid]-mic);
			ll s2 = abs(p-a[i]-b[mid]-mac);
			return s1 >= s2;
		};
		ll ok = -1;
		ll ng = y;
		while(abs(ok-ng) > 1){
			ll mid = (ok+ng)/2;
			if(nibutan_judge(mid)) ok = mid;
			else ng = mid;
		}
		ll bob = inf;
		//okの左側か、ngの右側
		if(ok >= 0) bob = min(bob, abs(p-a[i]-b[ok]-mic));
		if(ng < y) bob = min(bob, abs(p-a[i]-b[ng]-mac));
		ans = max(ans,bob);
	}
	cout<<ans<<endl;
	return 0;
}
