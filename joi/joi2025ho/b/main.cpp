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

int main(void){
    cinll(n);
	cinvll(a,n);
	cinvll(b,n);
	rep(i,n) a.push_back(a[i]);
	rep(i,n) b.push_back(b[i]);
	ll ans = inf;
	vll p(2*n);
	vll rem(2*n,0);
	p[0] = a[0] + b[0];
	rep(i,1,2*n){
		p[i] = p[i-1] + b[i];
		rem[i] = p[i-1] - a[i];
	}

	// cout<<"p :";
	// rep(i,n) cout<<p[i]<<" ";
	// cout<<endl;
	// cout<<"rem :";
	// rep(i,n) cout<<rem[i]<<" ";
	// cout<<endl;

	multiset<ll> ms;

	rep(i,n) ms.insert(rem[i]);
	
	ll deltax = *ms.begin();
	if(deltax < 0) deltax = -deltax;
	else deltax = 0;

	ans = min(ans, a[0] + deltax);
	ll deltar = deltax;

	//cout<<"0-deltax:"<<deltax<<" deltar:"<<deltar<<endl;

	//1番目から始める場合
	//まずは0番目の要素を消す
	rep(i,1,n){
		ms.erase(ms.lower_bound(rem[i-1]));
		ms.insert(rem[n-1+i]);
		//初期値の変化量を計算
		deltax = 0;
		deltar = -(rem[i] + deltax);
		ll over = -(*ms.begin() + deltar);
		//cout<<"over:"<<over<<endl;
		if(over > 0) deltax += over;
		ans = min(ans, a[i] + deltax);
		//cout<<i<<"-deltax:"<<deltax<<" deltar"<<deltar<<endl;
	}
	cout<<ans<<endl;
	return 0;
}
