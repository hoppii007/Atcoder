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

struct UnionFind {
public:
    vll par; // par[i]:iの親の番号　(例) par[3] = 2 : 3の親が2
    vll kosu;
    ll Count; //全体として何個の集合があるか
    
    UnionFind(ll n) : par(n),kosu(n),Count(n) { //最初は全てが根であるとして初期化
        rep(i,n){
            par[i] = i;
            kosu[i] = 1;
        }
    }

    ll root(ll x) { // データxが属する木の根を再帰で得る：root(x) = {xの木の根}
        if (par[x] == x) return x;
        return par[x] = root(par[x]);
    }

    void unite(vector<priority_queue<ll>> &vq, ll x, ll y) { // xとyの木を併合
        ll rx = root(x); //xの根をrx
        ll ry = root(y); //yの根をry
        if (rx == ry) return; //xとyの根が同じ(=同じ木にある)時はそのまま

		//数が少ない方を大きい方にマージ
		if(vq[rx].size() < vq[ry].size()){
			par[rx] = ry;
			while(vq[rx].empty() == false){
				vq[ry].push(vq[rx].top());
				vq[rx].pop();
			}
		}
		else{
			par[ry] = rx;
			while(vq[ry].empty() == false){
				vq[rx].push(vq[ry].top());
				vq[ry].pop();
			}
		}
    }

    bool same(ll x, ll y) { // 2つのデータx, yが属する木が同じならtrueを返す
        ll rx = root(x);
        ll ry = root(y);
        return rx == ry;
    }
    
    ll size(ll x){
        return kosu[root(x)];
    }
    ll size(){
        return Count;
    }
};



int main(void){
	cinll(n,q);
	vector<priority_queue<ll>> vq(n);
	rep(i,n) vq[i].push(i);
	UnionFind uf(n);
	rep(i,q){
		// if(i  == 8){
		// 	ll r = uf.root(2);
		// 	cout<<"root:"<<vq[r].size()<<endl;
		// 	ll tmp = vq[r].size();
		// 	vll ngo;
		// 	cout<<"size:"<<vq[r].size()<<endl;
		// 	rep(j,tmp){
		// 		ngo.push_back(vq[r].top());
		// 		vq[r].pop();
		// 	}
		// 	cout<<v_join<ll>(ngo," ")<<endl;
		// 	rep(j,tmp){
		// 		vq[r].push(ngo[ngo.size()-1]);
		// 		ngo.pop_back();
		// 	}
		// 	cout<<"size:"<<vq[r].size()<<endl;
		// 	cout<<endl;
		// }
		cinll(ki);
		if(ki == 1){
			cinll(u,v);
			u--; v--;
			ll ur = uf.root(u);
			ll vr = uf.root(v);
			if(ur == vr) continue;
			uf.unite(vq,u,v);
		}
		else{
			cinll(v,k);
			v--;
			ll r = uf.root(v);
			if(vq[r].size() < k) cout<<-1<<endl;
			else{
				vll tmp;
				rep(j,k-1){
					tmp.push_back(vq[r].top());
					vq[r].pop();
				}
				cout<<vq[r].top()+1<<endl;
				rep(j,k-1){
					vq[r].push(tmp[tmp.size()-1]);
					tmp.pop_back();
				}

			}
		}
		
	}
	return 0;
}
