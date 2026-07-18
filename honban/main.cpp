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
const ll inf = 1300100100100100100LL;
// const ll inf = 1001001001LL;
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

/**
 * @brief a と b の最大公約数を求める
 * @param a 非負整数
 * @param b 非負整数
 * @return gcd(a, b)
 * @note 計算量: O(log min(a, b))
 */
ll gcd(ll a,ll b){
	if(b == 0) return a;
	if(a<b) return gcd(b,a);
	ll r;
	while((r=a%b)){
		a = b;
		b = r;
	}
	return b;
}

/**
 * @brief 配列 a の全要素の最大公約数を求める
 * @param a 正整数の配列（要素はすべて 0 より大きいこと）
 * @return gcd(a[0], a[1], ..., a[n-1])
 * @note 計算量: O(n log max(a))
 */
ll gcds(vll a){
	ll ret = a[0];
	rep(i,a.size()){
		ret = gcd(ret,a[i]);
	}
	return ret;
}

/**
 * @brief 直線 ax + by + c = 0 を表す構造体
 * @note 同じ直線が同じ (a, b, c) で表現されるよう、以下の正規化を行う:
 *       1. gcd(|a|, |b|, |c|) で割って既約にする
 *       2. a, b, c のうち最初に現れる非零の値が正になるよう符号を揃える
 *       これにより operator< で直線の集合管理（map/set のキー）が可能。
 */
struct L {
	ll a, b, c;
	L(){}
	/**
	 * @brief 係数 (ta, tb, tc) から直線を構築する（正規化込み）
	 * @param ta x の係数
	 * @param tb y の係数
	 * @param tc 定数項
	 * @note a と b の少なくとも一方が非零であることを前提とする
	 */
	L(ll ta, ll tb, ll tc): a(ta), b(tb), c(tc) {
		ll g = gcds({abs(a),abs(b),abs(c)});
		a /= g;
		b /= g;
		c /= g;

		// 最初に出てくる 0 じゃないやつが - なら -1 をかけて符号を統一
		for(ll t: {a,b,c}) if(t != 0){
			if(t < 0){
				a *= -1;
				b *= -1;
				c *= -1;
			}
			break;
		}
	}
	bool operator<(const L& l) const {
		return tie(a,b,c) < tie(l.a, l. b, l.c);
	}
	/**
	 * @brief 2 直線が一致するか
	 * @note 正規化済みのため、(a, b, c) の一致と直線としての一致が同値になる
	 */
	bool operator==(const L& l) const {
		return tie(a,b,c) == tie(l.a, l.b, l.c);
	}
	bool operator!=(const L& l) const {
		return !(*this == l);
	}
};

/**
 * @brief 点 p1 と点 p2 を通る直線を返す
 * @param p1 通る点その1
 * @param p2 通る点その2（p1 と異なること）
 * @return p1, p2 を通る直線 L
 * @note 方向ベクトル (dx, dy) に対し法線ベクトル (dy, -dx) を係数として採用。
 *       定数項は p1 が直線上にある条件から決定。
 */
L make_line(pll p1, pll p2){
	ll dx = p2.fst - p1.fst;
	ll dy = p2.snd - p1.snd;
	return L(dy, -dx, p1.snd*dx - p1.fst*dy);
}

/**
 * @brief 点 p1 と点 p2 の垂直二等分線を返す
 * @param p1 端点その1
 * @param p2 端点その2（p1 と異なること）
 * @return p1, p2 から等距離にある点の集合をなす直線 L
 * @note |P-p1|^2 = |P-p2|^2 を展開して得られる
 *       2(x2-x1)x + 2(y2-y1)y + (x1^2+y1^2-x2^2-y2^2) = 0 を用いる。
 *       係数に座標の 2 乗が現れるため、座標が大きいとオーバーフローに注意。
 */
L perpendicular_bisector(pll p1, pll p2){
	ll a = 2 * (p2.fst - p1.fst);
	ll b = 2 * (p2.snd - p1.snd);
	ll c = p1.fst*p1.fst + p1.snd*p1.snd - p2.fst*p2.fst - p2.snd*p2.snd;
	return L(a, b, c);
}

/**
 * @brief 2 直線が平行かを判定する
 * @param l1 直線その1
 * @param l2 直線その2
 * @return 平行なら true
 * @note 法線ベクトル (a, b) 同士の外積が 0 かで判定する。
 *       一致する 2 直線も平行として true を返すため、
 *       「平行だが異なる」を見たいときは l1 != l2 と併用する。
 */
bool is_parallel(const L& l1, const L& l2){
	return l1.a * l2.b - l2.a * l1.b == 0;
}

/**
 * @brief 2 直線が垂直かを判定する
 * @param l1 直線その1
 * @param l2 直線その2
 * @return 垂直なら true
 * @note 法線ベクトル (a, b) 同士の内積が 0 かで判定する
 */
bool is_perpendicular(const L& l1, const L& l2){
	return l1.a * l2.a + l1.b * l2.b == 0;
}

/**
 * @brief 点 p を通り、直線 l に垂直な直線を返す
 * @param p 通る点
 * @param l 基準となる直線
 * @return p を通り l に垂直な直線 L
 * @note l の法線 (a, b) を法線としない向き、すなわち法線 (-b, a) を採用する。
 *       定数項は p が直線上にある条件から決定。
 */
L perpendicular_line(pll p, const L& l){
	return L(-l.b, l.a, l.b*p.fst - l.a*p.snd);
}

int solve(){
	cinll(px, py, qx, qy, rx, ry, sx, sy);
	L l1 = perpendicular_bisector({px,py}, {qx, qy});
	L l2 = perpendicular_bisector({rx,ry}, {sx, sy});

	if(is_parallel(l1, l2)){
		if(l1 == l2) cout << "Yes" << endl;
		else cout << "No" << endl;
	}
	else cout << "Yes" << endl;

	return 0;
}

int main(){
	cinll(t);
	while(t--) solve();
	return 0;
}