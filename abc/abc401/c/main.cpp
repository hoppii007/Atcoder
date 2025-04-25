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

const ll mod = 1000000000LL;
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

//基本的にはmoが素数前提
struct Modll{
public:
	ll value;
	Modll(){
		this->value = 0;
	}
	Modll(ll v){
		this->value = v%mod;
		while(this->value < 0) this->value += mod;
	}
	friend ostream& operator << (ostream& os, const Modll& p){
		os << p.value;
		return os;
	}
	Modll operator =(const ll &a){
		value = a;
		return *this;
	}
	friend Modll operator -(const Modll &a, const Modll &b){ return Modll(a.value-b.value); }
	friend Modll operator -(const Modll &a, const ll &b){ return Modll(a.value-(b%mod)); }
	friend Modll operator -(const ll &a, const Modll &b){ return Modll((a%mod)-b.value); }
	friend Modll operator +(const Modll &a, const Modll &b){ return Modll(a.value+b.value); }
	friend Modll operator +(const Modll &a, const ll &b){ return Modll(a.value+(b%mod)); }
	friend Modll operator +(const ll &a, const Modll &b){ return Modll((a%mod)+b.value); }
	friend Modll operator *(const Modll &a, const Modll &b){ return Modll(a.value*b.value); }
	friend Modll operator *(const Modll &a, const ll &b){ return Modll(a.value*(b%mod)); }
	friend Modll operator *(const ll &a, const Modll &b){ return Modll((a%mod)*b.value); }
	friend Modll operator ^(const Modll &a, const ll &b){
		if(b == 0) return Modll(1);
		if(b%2 == 0) return (a*a)^(b/2);
		else return a*(a^(b-1));
	}
	friend Modll operator /(const Modll &a, const Modll &b){ return a*(b^(mod-2)); }
	friend Modll operator /(const Modll &a, const ll &b){ return a*(Modll(b)^(mod-2)); }
	friend Modll operator /(const ll &a, const Modll &b){ return a*(b^(mod-2)); }
	friend void operator +=(Modll &a, const Modll &b){ a = a+b; }
	friend void operator +=(Modll &a, const ll &b){ a = a+b; }
	friend void operator +=(ll &a, const Modll &b){ a = (a+b).value; }
	friend void operator -=(Modll &a, const Modll &b){ a = a-b; }
	friend void operator -=(Modll &a, const ll &b){ a = a-b; }
	friend void operator -=(ll &a, const Modll &b){ a = (a-b).value; }
	friend void operator *=(Modll &a, const Modll &b){ a = a*b; }
	friend void operator *=(Modll &a, const ll &b){ a = a*b; }
	friend void operator *=(ll &a, const Modll &b){ a = (a*b).value; }
	friend void operator /=(Modll &a, const Modll &b){ a = a/b; }
	friend void operator /=(Modll &a, const ll &b){ a = a/b; }
	friend void operator /=(ll &a, const Modll &b){ a = (a/b).value; }
};

int main(void){
    cinll(n,k);
	vector<Modll> dp(n+1,1);
	Modll sum = 0;
	rep(i,k){
		sum += 1;
		dp[i] = 1;
		if(i == n){
			cout<<1<<endl;
			return 0;
		}
	}
	rep(i,k,n+1){
		dp[i] = sum;
		sum -= dp[i-k];
		sum += dp[i];
		if(i == n){
			cout<<dp[i]<<endl;
			return 0;
		}
	}
	return 0;
}
