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
//const ll inf = 1300100100100100100LL;
const ll inf = 1001001001LL;
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


/*
**どの問題?**

**difficulty**

**solved**
(自力AC / 解説AC / 未AC)

**felt**
（簡単 / 中 / 難しい）

**考察ノート**

**難所**

## 問題全体のメモ
*/


// --------------------------- ここまでテンプレ ----------------------------

/*
**どの問題?**
ABC244E

**difficulty**
1125

**solved**
(自力AC / 解説AC / 未AC)

**felt**
（簡単 / 中 / 難しい）

**考察ノート**
簡単なケースを考えてみる
「SからK回移動してTにたどり着くルートを数える」と言う問題なら、グラフDPで行けそう？
dp[i][j] := 頂点iから、j回移動で頂点Tに行くルートの本数としてメモ化再帰でいけそう
dp[i][j][k] := 頂点iから、j回移動で、頂点Tに、Xを偶数 or 奇数（k=0 or 1）回通っていけるルートの本数
としてメモ化再帰でよさそうな気もする


**難所**

## 問題全体のメモ
*/

/**
 * @brief mod付き整数型（long long ベース）
 * @details 四則演算・累乗・比較を自動で mod を取りながら行う構造体。
 *          割り算（逆元）はフェルマーの小定理を利用するため、mod が素数であることが前提。
 *          mod の変更は modll::mod = 値 で行う。デフォルトは 998244353。
 *
 * @note 使用例:
 *   modll a(10), b(3);
 *   cout << a + b << endl;   // 13
 *   cout << a / b << endl;   // 10 * 3^(mod-2) mod p
 *   modll::mod = 1000000007; // mod を変更
 */
struct modll{
public:
	static ll mod; ///< 法（デフォルト: 998244353）。素数を設定すること。
	ll value;      ///< mod を取った後の値（0 以上 mod 未満）

	/// @brief デフォルトコンストラクタ。値を 0 で初期化。
	modll(){
		this->value = 0;
	}

	/// @brief 値を指定して構築。負の値も正しく [0, mod) に正規化される。
	modll(ll v){
		this->value = v%modll::mod;
		while(this->value < 0) this->value += modll::mod;
	}

	/// @brief 出力ストリームへの書き出し
	friend ostream& operator << (ostream& os, const modll& p){
		os << p.value;
		return os;
	}

	/// @brief ll からの代入
	modll operator =(const ll &a){
		value = a;
		return *this;
	}

	// --- 算術演算（+, -, *, /, ^） ---

	friend modll operator -(const modll &a, const modll &b){ return modll(a.value-b.value); }
	friend modll operator -(const modll &a, const ll &b){ return modll(a.value-(b%modll::mod)); }
	friend modll operator -(const ll &a, const modll &b){ return modll((a%modll::mod)-b.value); }
	friend modll operator +(const modll &a, const modll &b){ return modll(a.value+b.value); }
	friend modll operator +(const modll &a, const ll &b){ return modll(a.value+(b%modll::mod)); }
	friend modll operator +(const ll &a, const modll &b){ return modll((a%modll::mod)+b.value); }
	friend modll operator *(const modll &a, const modll &b){ return modll(a.value*b.value); }
	friend modll operator *(const modll &a, const ll &b){ return modll(a.value*(b%modll::mod)); }
	friend modll operator *(const ll &a, const modll &b){ return modll((a%modll::mod)*b.value); }

	/// @brief 繰り返し二乗法による累乗。a^b mod p を O(log b) で計算。
	friend modll operator ^(const modll &a, const ll &b){
		if(b == 0) return modll(1);
		if(b%2 == 0) return (a*a)^(b/2);
		else return a*(a^(b-1));
	}

	/// @brief 除算。フェルマーの小定理により b^(mod-2) を逆元として計算。mod が素数であること。
	friend modll operator /(const modll &a, const modll &b){ return a*(b^(modll::mod-2)); }
	friend modll operator /(const modll &a, const ll &b){ return a*(modll(b)^(modll::mod-2)); }
	friend modll operator /(const ll &a, const modll &b){ return a*(b^(modll::mod-2)); }

	// --- 比較演算 ---

	friend bool operator <(const modll &a, const modll &b){ return a.value < b.value; }
	friend bool operator <(const modll &a, const ll &b){ return a.value < b; }
	friend bool operator <(const ll &a, const modll &b){ return a < b.value; }
	friend bool operator <=(const modll &a, const modll &b){ return a.value <= b.value; }
	friend bool operator <=(const modll &a, const ll &b){ return a.value <= b; }
	friend bool operator <=(const ll &a, const modll &b){ return a <= b.value; }
	friend bool operator >(const modll &a, const modll &b){ return a.value > b.value; }
	friend bool operator >(const modll &a, const ll &b){ return a.value > b; }
	friend bool operator >(const ll &a, const modll &b){ return a > b.value; }
	friend bool operator >=(const modll &a, const modll &b){ return a.value >= b.value; }
	friend bool operator >=(const modll &a, const ll &b){ return a.value >= b; }
	friend bool operator >=(const ll &a, const modll &b){ return a >= b.value; }
	friend bool operator ==(const modll &a, const modll &b){ return a.value == b.value; }
	friend bool operator ==(const modll &a, const ll &b){ return a.value == b; }
	friend bool operator ==(const ll &a, const modll &b){ return a == b.value; }
	friend bool operator !=(const modll &a, const modll &b){ return a.value != b.value; }
	friend bool operator !=(const modll &a, const ll &b){ return a.value != b; }
	friend bool operator !=(const ll &a, const modll &b){ return a != b.value; }

	// --- 複合代入演算 ---

	friend void operator +=(modll &a, const modll &b){ a = a+b; }
	friend void operator +=(modll &a, const ll &b){ a = a+b; }
	friend void operator +=(ll &a, const modll &b){ a = (a+b).value; }
	friend void operator -=(modll &a, const modll &b){ a = a-b; }
	friend void operator -=(modll &a, const ll &b){ a = a-b; }
	friend void operator -=(ll &a, const modll &b){ a = (a-b).value; }
	friend void operator *=(modll &a, const modll &b){ a = a*b; }
	friend void operator *=(modll &a, const ll &b){ a = a*b; }
	friend void operator *=(ll &a, const modll &b){ a = (a*b).value; }
	friend void operator /=(modll &a, const modll &b){ a = a/b; }
	friend void operator /=(modll &a, const ll &b){ a = a/b; }
	friend void operator /=(ll &a, const modll &b){ a = (a/b).value; }

	// --- インクリメント・デクリメント ---

	friend void operator ++(modll &a, int){ a += 1; } ///< 後置インクリメント
	friend void operator ++(modll &a){ a += 1; }      ///< 前置インクリメント
	friend void operator --(modll &a, int){ a -= 1; }  ///< 後置デクリメント
	friend void operator --(modll &a){ a -= 1; }       ///< 前置デクリメント
};
ll modll::mod = 998244353LL;

modll dp[2000][2001][2];
bool is_set_dp[2000][2001][2];
ll S,T,X;

modll dfs(vvll& nexts, ll i, ll j, ll k){
	if(j == 0){
		if(i == T && k == 0) return 1;
		else return 0;
	}
	if(is_set_dp[i][j][k]) return dp[i][j][k];
	is_set_dp[i][j][k] = true;
	dp[i][j][k] = 0;
	for(ll nxt: nexts[i]){
		if(i == X) dp[i][j][k] += dfs(nexts, nxt, j-1, (k+1)%2);
		else dp[i][j][k] += dfs(nexts, nxt, j-1, k);
	}
	return dp[i][j][k];
}

int main(){
	cinll(n,m,k);
	cin >> S >> T >> X;
	S--; T--; X--;
	vvll nexts(n);
	rep(i, 2000) rep(j, 2001) rep(k, 2) {
		is_set_dp[i][j][k] = false;
		dp[i][j][k] = 0;
	}
	rep(i, m){
		cinll(u, v);
		u--; v--;
		nexts[u].pb(v);
		nexts[v].pb(u);
	}
	cout << dfs(nexts, S, k, 0) << endl;
	return 0;
}