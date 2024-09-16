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

//O(logB)でa^bが計算できる
ll mypow(ll a,ll b){
    if(b == 0) return 1;
    if(b == 1) return a;
    if(b%2 == 0) return mypow(a*a,b/2);
    else return a*mypow(a*a,(b-1)/2);
}

bool allOK(vvll &nexts){
	vb visit(nexts.size(),false);
	visit[0] = true;
	queue<ll> q;
	q.push(0);
	while(!q.empty()){
		ll now = q.front();
		q.pop();
		for(ll next: nexts[now]){
			if(visit[next] == false){
				q.push(next);
				visit[next] = true;
			}
		}
	}
	rep(i,nexts.size()) if(visit[i] == false) return false;
	return true;
}

//コンボリューションを列挙 -> v_combs
//コンボリューションを出すとき、どこを使ったかの情報のみを列挙 -> vb_combs
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
	//最終的にはコンボリューションの二次元配列を作る
	static vvb vb_combs(int n,int k){
		vb b = vb(n,false);
		vvb bs;
		vb_combs_sub(bs,b,0,0,k);
		return bs;
	}


};

int main(void){
	cinll(n,m,k);
	vvll edges(m,vll(3));
	rep(i,m){
		cin>>edges[i][0]>>edges[i][1]>>edges[i][2];
		edges[i][0]--;
		edges[i][1]--;
	}
	ll ans = inf;
	Comb comb;
	vvb combs = comb.vb_combs(m,n-1);
	for(vb c:combs){
		//rep(i,c.size()) cout<<(c[i]?1:0)<<" ";
		//cout<<endl;
		ll sum = 0;
		vvll nexts(n);
		rep(i,c.size()) if(c[i]){
			nexts[edges[i][0]].push_back(edges[i][1]);
			nexts[edges[i][1]].push_back(edges[i][0]);
			sum = (sum+edges[i][2])%k;
		}
		if(allOK(nexts)) ans = min(ans,sum);
	}
	
	cout<<ans<<endl;
	return 0;
}
