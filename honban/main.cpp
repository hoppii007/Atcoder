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

string to_upper(string s){
	string ret = "";
	rep(i,s.size()){
		if('a' <= s[i] && s[i] <= 'z') ret += string(1, s[i] -'a' + 'A');
		else ret += string(1, s[i]);
	}
	return ret;
}

//メンバ lower_bound を持つか判定（set / multiset は持つ、vector は持たない）
template<typename C, typename = void>
struct has_member_lower_bound : false_type {};
template<typename C>
struct has_member_lower_bound<C, void_t<decltype(declval<C&>().lower_bound(declval<const typename C::value_type&>()))>> : true_type {};

/**
 * @brief 昇順のコンテナから最も近い値のイテレータを返す
 *
 * @tparam C ソート済み vector、または set / multiset
 * @param c  対象コンテナ（空でないこと）
 * @param t  探索する値
 * @return   t に最も近い要素を指すイテレータ
 *
 * @pre vector を渡す場合は昇順ソート済みであること
 * @pre c は空でないこと
 * @pre c は非 const であること（const コンテナは const_iterator になるため）
 * @note 同距離の場合は小さい方を返す
 * @note set / multiset ではメンバの lower_bound に自動で切り替わる。
 *       これらに std::lower_bound を使うとイテレータ移動が O(N) になり O(log N) にならない
 * @complexity O(log N)
 *
 * @example
 * std::vector<ll> v = {1, 3, 5, 7};
 * auto it = near_value(v, 4); // *it == 3
 * std::multiset<ll> m = {1, 3, 5, 7};
 * auto mit = near_value(m, 4); // *mit == 3
 */
template<typename C>
typename C::iterator near_value(C &c, const typename C::value_type &t){
	//set / multiset はメンバ版、vector などは std::lower_bound
	typename C::iterator it;
	if constexpr (has_member_lower_bound<C>::value) it = c.lower_bound(t);
	else it = lower_bound(c.begin(), c.end(), t);
	// 以上のやつが一個もないとき（すべてtより小さいとき）
	if(it == c.end()) return prev(it);
	// 一個前もあればそれと今の比較
	if(it != c.begin()){
		auto pit = prev(it);
		//一個前の方がitよりtに近ければそれを返す
		if(abs(*pit-t) <= abs(*it-t)) return pit;
	}
	// 一個前がなければそこ
	return it;
}

int main(){
	cinll(n);
	multiset<ll> m;
	rep(i, n){
		cinll(t);
		m.insert(t);
	}
	ll now = 0;
	ll ans = 0;
	while(m.size() > 0){
		auto it = near_value(m, now);
		ans += abs(*it - now);
		now = *it;
		m.erase(it);
	}
	cout << ans << endl;


	return 0;
}