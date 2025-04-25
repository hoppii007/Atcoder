#include<bits/stdc++.h>
//入力系
#define cinll(a) ll a; cin>>a; 
#define cinll2(a,b) ll a,b; cin>>a>>b;
#define cinll3(a,b,c) ll a,b,c; cin>>a>>b>>c;
#define cinint(a) int a; cin>>a;
#define cinint2(a,b) int a,b; cin>>a>>b;
#define cinint3(a,b,c) int a,b,c; cin>>a>>b>>c;
#define cinstring(s) string s; cin>>s;
#define cinstring2(s,t) string s,t; cin>>s>>t;
#define cinstring3(s,t,u) string s,t,u; cin>>s>>t>>u;
#define cinchar(c) char c; cin>>c;
#define cinvll(a,n) vll a(n); rep(i,n) cin>>a[i];
#define cinvs(a,n) vs a(n); rep(i,n) cin>>a[i];
//繰り返し系
#define rep(i,n) for(int i=0;i<n;i++)
#define rep1(i,n) for(int i=1;i<n;i++)
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

//便利系
//vectorをjoinするやつ
template<typename T>
string v_join(vector<T> &a,string c){
	string ret = "";
	rep(i,a.size()-1) ret += (to_string(a[i]) + c);
	return ret + to_string(a[a.size()-1]);
}

//modは問題に応じて変える
//a^-1で積の逆元を計算できる
//ModInt op llでも計算できる
class ModInt{
    ll mod = 998244353LL;
public:
    ll v;
    ModInt(ll tv): v(tv%mod) {while(this->v < 0) this->v += mod;}
    ModInt(): v(0) {}
    ModInt operator+(const ModInt &a){ return ModInt(this->v + a.v); }
    ModInt operator*(const ModInt &a){ return ModInt(this->v * a.v); }
    ModInt operator-(const ModInt &a){ return ModInt(this->v - a.v); }
    ModInt operator+(const ll &a){ return ModInt(this->v) + ModInt(a); }
    ModInt operator*(const ll &a){ return ModInt(this->v) * ModInt(a); }
    ModInt operator-(const ll &a){ return ModInt(this->v) - ModInt(a); }
    ModInt operator^(ll a){
        if(a == -1) a = mod-2;
        ModInt ret(1);
        ll rootv = this->v;
        while(true){
            if(a == 0) return ret;
            if(a%2 == 0){
                rootv = (rootv * rootv) % mod;
                a /= 2;
            }
            else{
                ret = ret * rootv;
                a--;
            }
        }
    }
    bool operator ==(const ModInt &a){ return this->v == a.v; }
    bool operator ==(const ll a){ return this->v == ModInt(a).v; }
	ModInt &operator=(const ModInt &a){
		this->v = a.v;
		return (*this);
	}
	ModInt &operator=(const ll &a){
		this->v = a%mod;
		while(this->v < 0) this->v += mod;
		return (*this);
	}
    
};


int main(void){
	//dp[i] := マスiを最後に塗る場合の通りの数
	cinll(n);
	cinvll(tmp,n);
	vector<ModInt> a(n);
	rep(i,n) a[i] = tmp[i];
	static ModInt dp[200001];
	//a[i]の最大値が小さいときは
	rep(i,n) dp[i] = 0;
	dp[0] = 1;
	//rep(i,n) for(int j=a[i].v;j<n;j+=a[i].v) dp[i+j] = dp[i+j] + dp[i];
	ModInt ans = 0;

	static ModInt dp2[200001][100];
	rep(i,n) rep(j,100) dp2[i][j] = 0;

	rep(i,n){
		rep(j,100) dp[i] = dp[i] + dp2[i][j];
		rep(j,100){
			if(i+j >= n) continue;
			dp2[i+j][j] = dp2[i+j][j] + dp2[i][j];
		}
		if(a[i].v >= 100){
			for(int j=a[i].v;i+j<n;j+=a[i].v) dp[i+j] = dp[i+j] + dp[i];
		}
		else{
			if(i + a[i].v >= n) continue;
			dp2[i+a[i].v][a[i].v] = dp2[i+a[i].v][a[i].v]+dp[i];

		}
	}
	
	rep(i,n) ans = ans + dp[i];
	cout<<ans.v<<endl;
	return 0;
}
