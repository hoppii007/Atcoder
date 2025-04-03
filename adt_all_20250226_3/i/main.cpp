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

//座標圧縮した後の座標が返ってくる
map<ll,ll> zaatu(vll &a){
	set<ll> s;
	rep(i,a.size()) s.insert(a[i]);
	map<ll,ll> ret;
	ll i = 0;
	for(ll t: s) ret[t] = i++;
	return ret;
}

//コンストラクタ　関数ポインタの部分は普通に関数名書くだけでOK
//SegmentTree<T>(vector<T>&,T (*f)(T,T),T ide)
//SegmentTree<T>(int n,T (*f)(T,T),T ide)
//関数
//Change(int,T)
//get(int,int)閉区間
template<typename T>
class SegmentTree{
	int n;
	T ident;
	vector<T> array;
	T (*op)(T,T);
public:
	SegmentTree(vector<T> &v,T (*f)(T,T),T ide){
		int sz = v.size();
		this->n = 1;
		while(n < sz) n *= 2;
		this->array = vector<T>(2*this->n-1,ide);
		this->ident = ide;
		this->op = f;
		for(int i=0;i<sz;i++) change(i,v[i]);
	}
	SegmentTree(int a,T (*f)(T,T),T ide){
		this->n = 1;
		while(n < a) n*= 2;
		this->array = vector<T>(2*this->n-1,ide);
		this->ident = ide;
		this->op = f;
	}
	SegmentTree(){}
	int l_child(int i){ return i*2+1; }
	int r_child(int i){ return i*2+2; }
	int get_parent(int i){ return (i%2 == 0) ? (i/2-1):(i/2); }
	void change(int i,T v){
		int ind = this->array.size()/2+i;
		this->array[ind] = v;
		int parent = get_parent(ind);
		while(parent != -1){
			this->array[parent] = op(this->array[l_child(parent)],this->array[r_child(parent)]);
			parent = get_parent(parent); 
		}
	}
	T get(int l,int r,int k=0,int nowL=-1,int nowR=-1){
		if(k == 0){
			nowL = 0;
			nowR = this->array.size()/2;
		}
		if(r < nowL || nowR < l) return this->ident;
		if(l <= nowL && nowR <= r) return this->array[k];
		T vl = get(l,r,l_child(k),nowL,(nowL+nowR)/2);
		T vr = get(l,r,r_child(k),(nowL+nowR)/2+1,nowR);
		return op(vl,vr);
	}
};

ll add(ll a, ll b){ return a+b; }

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
    cinll(n);
	cinvll(a,n);
	map<ll,ll> za = zaatu(a);
	SegmentTree<ll> stc(za.size(), add, 0);
	SegmentTree<ll> sts(za.size(), add, 0);
	
	Modll sum = 0;
	
	rep(i,n){
		
		sum += a[i];
		
		//今追加したa[i]以下のやつの個数*a[i]
		ll t = stc.get(0,za[a[i]])*a[i];
		//a[i]より大きいやつの今までの合計
		ll tt = sts.get(za[a[i]], (ll)za.size()-1) - sts.get(za[a[i]], za[a[i]]);
		sum += (t+tt);
		sum += (t+tt);

		Modll bunbo = i+1;

		cout<<sum/(bunbo*bunbo)<<endl;


		stc.change(za[a[i]], stc.get(za[a[i]],za[a[i]])+1);
		sts.change(za[a[i]], sts.get(za[a[i]],za[a[i]])+a[i]);

	}
	return 0;
}
