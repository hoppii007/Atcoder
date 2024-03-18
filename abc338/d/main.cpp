#include<bits/stdc++.h>
//入力系
#define cinll(...) ll __VA_ARGS__; input(__VA_ARGS__);
#define cinint(...) int __VA_ARGS__; input(__VA_ARGS__);
#define cinstr(...) string __VA_ARGS__; input(__VA_ARGS__);
#define cinchar(...) char __VA_ARGS__; input(__VA_ARGS__);
#define cinvll(a,n) vll a(n); rep(i,n) cin>>a[i];
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

//区間更新、区間クエリはこいつにおまかせ！
///////注意/////////
//・T,opT,idTがモナドになっている
//・O,opO,idOがモナドになっている
//・f(O,T) -> Tに対して
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
template<typename T,typename O>
class LazySegmentTree{
public:
    //要素のモナド
    T idT;
    T (*opT)(T,T);
    vector<T> arrayT;
    //作用素のモナド（この値について作用が一つ求まる）
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
        this->arrayO[k] = opO(o,this->arrayO[k]);
        this->arrayT[k] = f(this->arrayO[k],this->arrayT[k]);
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
            //更新履歴の反映は終わってるから、葉なら更新履歴をリセット
            if(l_child(k) >= this->arrayT.size()) this->arrayO[k] = idO;
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
    
    void showTree(){
        int n = 1;
        int j=0;
        while(n+j < this->arrayT.size()){
            while(j < n){
                cout<<this->arrayT[n+j-1]<<"/"<<this->arrayO[n+j-1]<<" ";
                j++;
            }
            cout<<endl;
            n *= 2;
            j = 0;
        }
    }
};

ll add(ll a,ll b){
	return a+b;
}

ll max(ll a,ll b){
	if(a > b) return a;
	return b;
}

int main(void){
	cinll(n,m);
	cinvll(x,m);
	LazySegmentTree<ll,ll> lst(n,max,0,add,0,add);
	ll ans = 0;
	rep(i,m-1){
		ll from = min(x[i],x[i+1])-1;
		ll to = max(x[i],x[i+1])-1;
		ll costRight = to-from;
		ll costLeft = n-costRight;
		//cout<<from<<"から"<<to<<"へのコスト"<<endl;
		//cout<<costRight<<" "<<costLeft<<endl;
		//右回りの方がコストが小さいとき、右回りにロスぶん足す
		if(costRight < costLeft){
			ans += costRight;
			lst.change(from,to-1,costLeft-costRight);
		}
		else if(costLeft < costRight){
			ans += costLeft;
			if(from > 0) lst.change(0,from-1,costRight-costLeft);
			lst.change(to,n-1,costRight-costLeft);
		}
		else{
			ans += costRight;
		}
	}
	ll mini = inf;
	rep(i,n) mini = min(mini,lst.get(i,i));
	cout<<(ans+mini)<<endl;
	return 0;
}
