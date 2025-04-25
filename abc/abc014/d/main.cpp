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
const double PI=3.1415926535897932384626433832795028841971;

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

//一回の行き先が決まってる状態から、K回先を計算する O(N log K)
//要素数:n
//各状態(0 ~ n)から、2^60回までの遷移先を求める(10^18ちょいくらい)
// dub[i][j] := iから2^j回遷移した先
//一回の行き先をnextsに入れとく
//行き先がないやつは-1
vvll dubling(vll &nexts){
	vvll dub(nexts.size(), vll(61));
	rep(i,nexts.size()) dub[i][0] = nexts[i];
	rep(j,1,61) rep(i,nexts.size()){
		if(dub[i][j-1] == -1) dub[i][j] = -1;
		else dub[i][j] = dub[dub[i][j-1]][j-1];
	}
	return dub;
}

//木の中で、rootからの距離を入れる
//O(N)
vll treeDepth(ll root, vvll &nexts){
	vll ret(nexts.size(), inf);
	std::function<void(ll,ll)> subDfs = [&](ll now, ll d){
		ret[now] = d;
		for(ll next: nexts[now]) if(ret[next] == inf) subDfs(next, d+1);
	};
	subDfs(root, 0);
	return ret;
}

//木の中で、rootを指定して、各頂点のparentsを設定する
//親の親は自分自身にしておく
//O(N)
vll treeParents(ll root, vvll &nexts){
	vll ret(nexts.size(), inf);
	std::function<void(ll,ll)> subDfs = [&](ll now, ll parent){
		ret[now] = parent;
		for(ll next: nexts[now]) if(ret[next] == inf) subDfs(next, now);
	};
	//親の親を-1にしたりしたいときはここの第二引数を変更
	//infにするときは注意
	subDfs(root, -1);
	return ret;
}

//各頂点は0 ~ N-1の番号で管理
//共通祖先をO(Log V)で出すやつ
//前計算はO(V Log V)
//nexts : 有向グラフにしておく（親にも子にも行けるから実質無向グラフ）
struct LcaTree {
public:
	//親
	ll root;

	//parents[i] : iの親を示す（親は必ず一人だからvllでいい）
	//親の親は自分自身にする
	vll parents;

	//根からの距離を入れる
	vll distToRoot;

	//iから2^j回遡ったところ
	vvll backDub;

	LcaTree(ll r, vvll &nexts){
		//parentsとrootをセット
		root = r;
		parents = treeParents(root, nexts);

		//rootからの距離をセット
		distToRoot = treeDepth(root, nexts);

		//遡る用のdubling配列を用意
		backDub = dubling(parents);
	}

	//頂点uからn回遡りたい
	ll back(ll u, ll n){
		ll t = 0;
		while((1LL << t) <= n){
			if((1LL << t) & n){
				u = backDub[u][t];
				if(u == -1) return root;
			}
			t++;
		}
		return u;
	}

	ll lca(ll u, ll v){
		//高さを揃える
		if(distToRoot[u] < distToRoot[v]) v = back(v, distToRoot[v]-distToRoot[u]);
		else if(distToRoot[u] > distToRoot[v]) u = back(u, distToRoot[u]-distToRoot[v]);

		//今のu,vからx遡ったら同じになる、最小のxを求める
		//二分探索のジャッジ用のやつ
		auto nibutan_judge = [&](ll mid){
			return back(u,mid) == back(v,mid);
		};
		ll ok = parents.size();
		ll ng = -1;
		while(abs(ok-ng) > 1){
			ll mid = (ok+ng)/2;
			if(nibutan_judge(mid)) ok = mid;
			else ng = mid;
		}
		return back(u,ok);
	}

	//2点間の距離
	ll dist(ll u, ll v){
		ll l = this->lca(u,v);
		return distToRoot[u] + distToRoot[v] - 2*distToRoot[l];
	}
	
};


int main(void){

	// cinll(n);
	// vvll nexts(n);
	// ll root = 0;
	// rep(i,n){
	// 	cinll(p);
	// 	if(p == -1){
	// 		root = i;
	// 		continue;
	// 	}
	// 	p--;
	// 	nexts[p].push_back(i);
	// }

	// LcaTree lt(root, nexts);
	
	// cinll(q);
	// while(q--){
	// 	cinll(a,b);
	// 	a--; b--;
	// 	ll l = lt.lca(a,b);
	// 	if(l == b) cout<<"Yes"<<endl;
	// 	else cout<<"No"<<endl;
	// }

	cinll(n);
	vvll nexts(n);
	rep(i,n-1){
		cinll(x,y);
		x--; y--;
		nexts[x].push_back(y);
		nexts[y].push_back(x);
	}
	LcaTree lt(0, nexts);

	cinll(q);
	while(q--){
		cinll(a,b);
		a--; b--;
		cout<<lt.dist(a,b)+1<<endl;
	}
    
	return 0;
}
