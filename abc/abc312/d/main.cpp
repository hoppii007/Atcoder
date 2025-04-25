#include<bits/stdc++.h>
//入力系
#define cinll(...) ll __VA_ARGS__; input(__VA_ARGS__);
#define cinint(...) int __VA_ARGS__; input(__VA_ARGS__);
#define cinstr(...) string __VA_ARGS__; input(__VA_ARGS__);
#define cinchar(...) char __VA_ARGS__; input(__VA_ARGS__);
#define cinvll(a,n) vll a(n); rep(i,n) cin>>a[i];
#define cinvvll(a,n,m) vvll a(n,vll(m)); rep(i,n) rep(j,m) cin>>a[i][j];
#define cinvs(a,n) vs a(n); rep(i,n) cin>>a[i];
//繰り返し系
#define rep1(n) for(int i=0;i<n;i++)
#define rep2(i,n) for(int i=0;i<n;i++)
#define rep3(i,a,n) for(int i=a;i<n;i++)
#define rep4(i,a,n,b) for(int i=a;i<n;i+=b)
#define overload4(a,b,c,d,e,...) e
#define rep(...) overload4(__VA_ARGS__,rep4,rep3,rep2,rep1)(__VA_ARGS__)
#define mrep1(n) for(int i=n;i>=0;i--)
#define mrep2(i,n) for(int i=n;i>=0;i--)
#define mrep3(i,n,a) for(int i=n;i>=a;i--)
#define mrep4(i,n,a,b) for(int i=n;i>=a;i-=b)
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
string v_join(vector<T> &a,string c){
	string ret = "";
	rep(i,a.size()-1) ret += (to_string(a[i]) + c);
	return ret + to_string(a[a.size()-1]);
}
//空白区切り表示
template<class T, class... Ts>
void out(const T& a,const Ts&... b){
	cout<<a;
	(cout<< ... << (cout << ' ',b));
	cout<<endl;
}
//入力
template<class... T>
void input(T&... a){
	(cin >> ... >> a);
}

//基本的にはmoが素数前提
class Modll{
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
	cinstr(s);
	vector<vector<Modll>> dp(s.size(), vector<Modll>(s.size()+1,0));
	if(s[0] == ')'){
		cout<<"0"<<endl;
		return 0;
	}
	ll n = s.size();

	//行けるパターンしか残ってない
	dp[0][1] = 1;
	rep(i,n-1) rep(j,n+1){
		if(s[i+1] == '?'){
			if(j-1 >= 0) dp[i+1][j-1] += dp[i][j];
			if(j+1 <= n) dp[i+1][j+1] += dp[i][j];
		}
		else if(s[i+1] == ')'){
			if(j-1 >= 0) dp[i+1][j-1] += dp[i][j];
		}
		else{
			if(j+1 <= n) dp[i+1][j+1] += dp[i][j];
		}
	}
	cout<<dp[n-1][0]<<endl;
	return 0;
}
