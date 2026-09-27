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


//区間更新、区間クエリはこいつにおまかせ！
///////注意/////////
//・T,opT,idTがモノイドになっている
//・O,opO,idOがモノイドになっている
//・f(o,(opT(t1,t2))) = opT(f(o,t1),f(o,t2))
//・f(opO(o1,o2),t) = f(o1,f(op2,t))
//・f(idO,t) = t
////////////////////
//コンストラクタ　関数ポインタの部分は普通に関数名書くだけでOK
//LazySegmentTree<T,O>(T配列、T演算、T単位元、O演算、O単位元、更新作用(OxT -> T))
//LazySegmentTree<T,O>(T配列の数、T演算、T単位元、O演算、O単位元、更新作用(OxT -> T))
//関数
//change(int,int,O) : 閉区間を作用素oで更新
//get(int,int)閉区間
// 注意事項
// ・opT(o1, o2)はo2が先に追加されて、左に積まれていく
template<typename T,typename O>
class LazySegmentTree{
public:
	//要素のモノイド
	T idT;
	T (*opT)(T,T);
	vector<T> arrayT;
	//作用素のモノイド（この値について作用が一つ求まる）
	O idO;
	O (*opO)(O,O);
	vector<O> arrayO;
	//作用素から作用を作り、それをtに適用する
	T (*f)(O,T);
	
public:
	int l_child(int i){ return i*2+1; }
	int r_child(int i){ return i*2+2; }
	int get_parent(int i){ return (i%2 == 0) ? (i/2-1):(i/2); }
	LazySegmentTree(vector<T> &v, T (*opt)(T,T),T idt, O (*opo)(O,O), O ido, T (*tf)(O,T)){
		int sz = v.size();
		int n = 1;
		while(n < sz) n *= 2;
		this->idT = idt;
		this->opT = opt;
		this->idO = ido;
		this->opO = opo;
		this->arrayT = vector<T>(2*n-1,this->idT);
		this->arrayO = vector<O>(2*n-1,this->idO);
		this->f = tf;
		for(int i=0;i<sz;i++) this->arrayT[this->arrayT.size()/2+i] = v[i];
		for(int i=this->arrayT.size()/2-1;i>=0;i--){
			this->arrayT[i] = opT(this->arrayT[l_child(i)],this->arrayT[r_child(i)]);
		}
	}
	LazySegmentTree(int a, T (*opt)(T,T),T idt, O (*opo)(O,O), O ido, T (*tf)(O,T)){
		int n = 1;
		while(n < a) n*= 2;
		this->idT = idt;
		this->opT = opt;
		this->idO = ido;
		this->opO = opo;
		this->arrayT = vector<T>(2*n-1,this->idT);
		this->arrayO = vector<O>(2*n-1,this->idO);
		this->f = tf;
	}
	LazySegmentTree(){}
	void o_update(int k,O o){
		this->arrayT[k] = f(o,this->arrayT[k]);
		this->arrayO[k] = opO(o,this->arrayO[k]);
		//葉なら更新履歴をリセットする
		if(l_child(k) >= this->arrayT.size()) this->arrayO[k] = idO;
	}
	//[l,r] : 更新したい閉区間
	//o : 更新に使いたい作用素（[l,r]のすべてのtをf(o,t)にしたい）
	//k : 自分のインデックス
	//[nowL,nowR] : 自分がカバーしている閉区間
	void change(int l,int r,O o,int k=0,int nowL=-1,int nowR=-1){
		//親は最初から最後までカバー
		if(k == 0){
			nowL = 0;
			nowR = this->arrayT.size()/2;
		}
		//自分のカバー範囲と変更範囲が全く変わらないときは何もしない
		if(r < nowL || nowR < l){ return; }
		//自分のカバー範囲内全部を更新するとき
		if(l <= nowL && nowR <= r){
			//完全にかぶる場合、値と変更履歴を更新しておく
			o_update(k,o);
			return;
		}
		//自分の中のカバー範囲の1部が被ってるとき
		//自分の更新履歴を下に引き継ぐ
		int lc = l_child(k);
		int rc = r_child(k);
		//葉じゃないときは今溜まってる更新履歴を下に伝播
		if(lc < this->arrayT.size()){
			o_update(lc,this->arrayO[k]);
			o_update(rc,this->arrayO[k]);
			this->arrayO[k] = idO;
		}
		change(l,r,o,lc,nowL,(nowL+nowR)/2);
		change(l,r,o,rc,(nowL+nowR)/2+1,nowR);
		//自分の値を更新
		this->arrayT[k] = opT(arrayT[lc],arrayT[rc]);
		return;
	}
	//o : 親までの更新履歴
	T get(int l,int r,int k=0,int nowL=-1,int nowR=-1){
		//親は最初から最後までカバー
		if(k == 0){
			nowL = 0;
			nowR = this->arrayT.size()/2;
		}
		//カバー範囲が全く被らないときは単位元を返す
		if(r < nowL || nowR < l) return this->idT;
		//完全にかぶっているとき（葉の時は必ずここに来る）
		if(l <= nowL && nowR <= r){
			//葉なら更新は終わってるはずだから返すだけ
			if(l_child(k) >= this->arrayT.size()){
				return this->arrayT[k];
			}
			//葉じゃなくても更新が終わってるなら返す
			if(this->arrayO[k] == idO){
				return this->arrayT[k];
			}
			//葉じゃなくて更新も終わってないなら更新して一個下にだけ流して返す
			o_update(l_child(k),this->arrayO[k]);
			o_update(r_child(k),this->arrayO[k]);
			this->arrayO[k] = idO;
			return this->arrayT[k];
		}
		//更新履歴を下に流す
		int lc = l_child(k);
		int rc = r_child(k);
		//葉じゃないときは更新履歴を下に伝播
		if(lc < this->arrayT.size()){
			o_update(lc,this->arrayO[k]);
			o_update(rc,this->arrayO[k]);
			this->arrayO[k] = idO;
		}
		T vl = get(l,r,lc,nowL,(nowL+nowR)/2);
		T vr = get(l,r,rc,(nowL+nowR)/2+1,nowR);
		this->arrayO[k] = idO;
		this->arrayT[k] = opT(arrayT[lc],arrayT[rc]);
		return opT(vl,vr);
	}
	// [l,r] 区間で、最も左にある condition = trueの部分を返す
	// conditionは（集計結果に）単調性があること（false,false,...,false,true,true,...,true）
	// 例えば、minを記録しておいてマイナスがあるかどうかのチェックみたいな
	// 見つからなければ -1 を返す。
	template<class Cond>
	int find_first_condition(int l,int r,Cond condition_f, int k=0,int nowL=-1,int nowR=-1){
		if(k == 0){
			nowL = 0;
			nowR = (int)this->arrayT.size()/2;
		}
		if(r < nowL || nowR < l) return -1; // 範囲外
		if(condition_f(this->arrayT[k]) == false) return -1; // このノード内にtrueがない
		if(nowL == nowR){
			// 葉で、かつ m < 0 ならここ
			return nowL;
		}
		int lc = l_child(k);
		int rc = r_child(k);
		// 子に遅延伝播してから探索（必要）
		if(lc < (int)this->arrayT.size()){
			o_update(lc,this->arrayO[k]);
			o_update(rc,this->arrayO[k]);
			this->arrayO[k] = idO;
		}
		int mid = (nowL+nowR)/2;
		int res = find_first_condition(l,r,condition_f,lc,nowL,mid);
		if(res != -1) return res;
		return find_first_condition(l,r,condition_f,rc,mid+1,nowR);
	}
	void showTree(){
		int n = 1;
		int j=0;
		while(n+j < this->arrayT.size()){
			while(j < n){
				cout<<"("<<this->arrayT[n+j-1].first<<","<<this->arrayT[n+j-1].second<<")"<<"/"<<this->arrayO[n+j-1]<<" ";
				j++;
			}
			cout<<endl;
			n *= 2;
			j = 0;
		}
	}
};




struct QQ{
	ll a, b, c, d;
};

int main(){
	cinll(n, m, q);
	cinvpll(lr, n);
	vpll need_points;

	vector<QQ> querys(q);
	rep(i, q){
		cinll(a,b,c,d);
		querys[i] = {a,b,c,d};
		need_points.pb({a,c});
		need_points.pb({a,d+1});
		need_points.pb({b+1,c});
		need_points.pb({b+1,d+1});
	}
	sort(all(need_points));

	// 区間加算、区間和取得
	// ・TとOの積が*で定義されてないとダメ
	// ・vector<T>のvを用意しておく
	using T = ll;
	using O = ll;
	const T id_t = 0;
	const O id_o = 0;
	vector<pair<T,ll>> before(m+10);
	rep(i, m+10) before[i] = pll{0, 1};
	LazySegmentTree<pair<T,ll>, O> lst(
		before,
		[](pair<T, ll> a, pair<T, ll> b){ return pair<T,ll>{a.fst+b.fst, a.snd+b.snd}; },
		{id_t, 0},
		[](O o_new, O o_old){ return o_new + o_old; },
		id_o,
		[](O o, pair<T,ll> t){ return pair<T,ll>{t.fst+o*t.snd, t.snd}; }
	);
	

	// 次使う先頭
	ll idx = 0; 
	map<pll,ll> value;
	rep(i, need_points.size()){
		pll p = need_points[i];
		while(idx < n && idx < p.fst-1){
			lst.change(lr[idx].fst, lr[idx].snd, 1);
			idx++;
		}
		value[p] = lst.get(0, p.snd-1).fst;
	}

	rep(i, q){
		ll a = querys[i].a;
		ll b = querys[i].b;
		ll c = querys[i].c;
		ll d = querys[i].d;

		cout << value[{b+1, d+1}]-value[{a,d+1}]-value[{b+1,c}]+value[{a,c}] << endl;
	}

	//cout << "ngo" << endl;

	return 0;
}