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

class Tyokke{
public:
	vll dpSub;
	Tyokke(){}
	Tyokke(ll n){
		dpSub = vll(n,-1);
	}
	ll tyokkeSub(vector<vvll> &nexts, vb &visit,ll now){
		ll ret = 0;
		for(vll next:nexts[now]){
			if(visit[next[0]]) continue;
			visit[next[0]] = true;
			ret = max(ret,tyokkeSub(nexts,visit,next[0])+next[1]);
		}
		dpSub[now] = ret;
		return dpSub[now];
	}

	//nexts[i][j][0] := iから伸びる辺で、j番目の行ける場所
	//nexts[i][j][1] := iから伸びる辺で、j番目のコスト
	ll tyokke(vector<vvll> &nexts){
		//まずは0番目から各頂点までの距離を出す
		vb visit(nexts.size(),false);
		visit[0] = true;
		tyokkeSub(nexts,visit,0);
		ll maxv = -1;
		ll maxi = -1;
		rep(i,dpSub.size()){
			if(maxv < dpSub[i]){
				maxv = dpSub[i];
				maxi = i;
			}
		}
		cout<<"ittan:"<<maxv<<" "<<dpSub[3]<<endl;
		//今度はmaxiを頂点として、一番長いところを出す
		visit = vb(nexts.size(),false);
		visit[maxi] = true;
		tyokkeSub(nexts,visit,maxi);
		maxv = -1;
		rep(i,dpSub.size()) maxv = max(maxv,dpSub[i]);
		return maxv;
	}

};


int main(void){
	cinll(n);
	vector<vvll> nexts(n);
	rep(i,n-1){
		cinll(a,b,c);
		a--;b--;

		nexts[a].push_back({b,c});
		nexts[b].push_back({a,c});
	}
	Tyokke tyokke(n);
	ll t = tyokke.tyokke(nexts);
	cout<<t<<endl;
	ll sum = 0;
	rep(i,n){
		for(vll next:nexts[i]) sum += next[1];
	}
	cout<<2*sum-t<<endl;
	return 0;
}
