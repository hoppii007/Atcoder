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

ll myMax(ll a,ll b){ return (a<b ? a:b); }

int main(void){
	cinll(n);
	SegmentTree<ll> st(n+1,myMax,inf);
	st.change(n,inf);
	cinvll(h,n);
	rep(i,n) h[i]--;
	rep(i,n){
		st.change(h[i],i);
	}
	vb u(n,false);
	vll ans(n,0);
	//1以降で高さの最大を更新したところを保存していく
	ll now = -1;
	rep(i,1,n){
		if(h[i] > now){
			u[i] = true;
			now = h[i];
			ans[0]++;
		}
	}
	st.change(h[0],inf);
	rep(i,1,n-1){
		//u[i]は絶対oだから問答無用で-1
		ans[i] = ans[i-1]-1;
		st.change(h[i],inf);
		//一個先が更新してなかったとき
		if(u[i+1] == false){
			//xが続く間更新するか見ていく
			ll j = i+1;
			ll now = -1;
			while(j < n && u[j] == false){
				//if(i == 1) cout<<"j:"<<j<<endl;
				if(h[j] > now){
					ans[i]++;
					now = h[j];
					u[j] = true;
				}
				//if(i == 1) cout<<j<<endl;
				j = st.get(h[j]+1,n);
				//if(i == 1) cout<<"next:"<<j<<endl;
			}
		}
		st.change(h[i],inf);
	}
	cout<<v_join<ll>(ans," ")<<endl;
	//cout<<st.get(7,n)<<endl;
	return 0;
}
