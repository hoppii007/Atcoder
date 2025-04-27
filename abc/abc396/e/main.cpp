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


int main(void){
    cinll(n,m);
	vll x(m), y(m), z(m);
	rep(i,m){
		cin>>x[i]>>y[i]>>z[i];
		x[i]--; y[i]--;
	}
	vector<vpll> nexts(n);
	rep(i,m){
		nexts[x[i]].push_back({z[i], y[i]});
		nexts[y[i]].push_back({z[i], x[i]});
	}

	vb visit(n, false);
	vb finished(n, false);
	vll colors(n, -1);
	std::function<ll(ll,ll,ll,ll)> dfs = [&](ll t, ll now,ll parent, ll color){
		visit[now] = true;
		colors[now] = color;
		ll ret = 0;
		for(pll next: nexts[now]) if(next.second != parent) {
			ll nextColor = -1;
			//1が立つようにしたい時　
			if(next.first&t){
				if(color == 1) nextColor = 0;
				else nextColor = 1;
			}
			//0にしたい時
			else{
				if(color == 1) nextColor = 1;
				else nextColor = 0;
			}
			if(colors[next.second] != -1){
				if(colors[next.second] != nextColor) return inf;
				continue;
			}
			ll nextDfs = dfs(t, next.second, now, nextColor);
			if(nextDfs == inf) return inf;
			ret += nextDfs;
		}
		finished[now] = true;
		return ret + color;
	};

	std::function<void(ll,ll)> resetColors = [&](ll now, ll parent){
		//cout<<"reset:"<<now<<endl;
		colors[now] = -1;
		for(pll next: nexts[now]) if(next.second != parent) if(colors[next.second] != -1) {
			resetColors(next.second, now);
		}
	};

	ll t = 1;
	vll ans(n,0);
	while(t <= 1000000000LL){
		//まずは0スタートでセットしてみる
		rep(i,n) visit[i] = false;
		rep(i,n) colors[i] = -1;
		rep(i,n) if(colors[i] == -1) {
			ll z = dfs(t, i, -1, 0);
			if(z == inf){
				cout<<-1<<endl;
				return 0;
			}
			resetColors(i, -1);
			ll o = dfs(t, i, -1, 1);
			if(o == inf){
				cout<<-1<<endl;
				return 0;
			}
			resetColors(i, -1);

			if(z < o){
				dfs(t, i, -1, 0);
			}
			else{
				dfs(t, i, -1, 1);
			}
		}
		rep(i,n) if(colors[i] == 1) ans[i] += t;

		t <<= 1LL;
	}

	rep(i,n) cout<<ans[i]<<" ";
	cout<<endl;

	return 0;
}
