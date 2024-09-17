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
//コンビネーションを列挙 -> v_combs
//コンビネーションを出すとき、どこを使ったかの情報のみを列挙 -> vb_combs
class Comb{
private:
	//vは最初全部falseにしといてね
	static void vb_combs_sub(vvb &vv, vb &v, int now_count, int now, int k){
		if(now_count == k) return;
		//ちょうど最後の一個を担当するとき
		if(now_count == k-1){
			for(int i=now;i<v.size();i++){
				v[i] = true;
				vv.push_back(v);
				v[i] = false;
			}
			return;
		}
		//まだまだいけるとき
		for(int i=now;i<v.size();i++){
			v[i] = true;
			vb_combs_sub(vv,v,now_count+1,i+1,k);
			v[i] = false;
		}
	}
public:
	Comb(){}
	//vector<bool>の中でk本trueが立つようにする
	//最終的にはコンビネーションの二次元配列を作る
	static vvb vb_combs(int n,int k){
		vb b = vb(n,false);
		vvb bs;
		vb_combs_sub(bs,b,0,0,k);
		return bs;
	}

};

bool loop(vvll &nexts,vb &visit,ll now,ll first=-1){
	if(first == -1){
		first = now;
		visit[now] = true;
		for(ll next:nexts[now]) return loop(nexts,visit,next,first);
		//もし誰ともつながってなければループしない
		return false;
	}
	if(now == first) return true;
	visit[now] = true;
	for(ll next: nexts[now]){
		if(visit[next]) continue;
		return loop(nexts,visit,next,first);
	}
	//次に行くべき場所が無いならループしない
	return false;
}

int main(void){
	cinll(h,w);
	vvll grid(h,vll(w));
	rep(i,h) rep(j,w) cin>>grid[i][j];
	ll n = h-1+w-1;
	Comb comb;
	vvb combs = comb.vb_combs(n,w-1);
	ll ans = 0;
	for(vb c:combs){
		ll t = 0;
		ll y = 0;
		set<ll> exists;
		bool isOK = true;
		rep(i,c.size()){
			if(exists.count(grid[t][y]) > 0){
				isOK = false;
				break;
			}
			exists.insert(grid[t][y]);
			if(c[i]) y++;
			else t++;
		}
		if(exists.count(grid[t][y]) > 0) isOK = false;
		if(isOK) ans++;
	}
	cout<<ans<<endl;
	return 0;
}
