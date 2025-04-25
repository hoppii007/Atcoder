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

vll tov(ll n,ll a){
	vll ret(n);
	rep(i,n){
		ret[i] = a%3;
		a /= 3;
	}
	return ret;
}

ll toll(vll &a){
	ll ret = 0;
	ll t = 1;
	rep(i,a.size()){
		ret += t*a[i];
		t *= 3;
	}
	return ret;
}

int main(void){
	cinll(n);
	cinstr(s,t);
	s += "..";
	t += "..";
	n += 2;
	vll sv(n),tv(n);
	rep(i,n){
		if(s[i] == '.') sv[i] = 0;
		else if(s[i] == 'W') sv[i] = 1;
		else sv[i] = 2;

		if(t[i] == '.') tv[i] = 0;
		else if(t[i] == 'W') tv[i] = 1;
		else tv[i] = 2;
	}
	queue<ll> q;
	map<ll,ll> d;
	ll goal = toll(tv);
	q.push(toll(sv));
	d[toll(sv)] = 0;
	while(!q.empty()){
		ll now = q.front();
		q.pop();
		if(now == goal){
			cout<<d[now]<<endl;
			return 0;
		}
		vll nowv = tov(n,now);
		ll aki = -1;
		rep(i,n-1) if(nowv[i]==0 && nowv[i+1] == 0) aki = i;
		if(aki == -1) continue;
		rep(i,n-1){
			if(nowv[i] != 0 && nowv[i+1] != 0){
				nowv[aki] = nowv[i];
				nowv[aki+1] = nowv[i+1];
				nowv[i] = 0;
				nowv[i+1] = 0;
				ll next = toll(nowv);
				if(d.count(next) == 0){
					d[next] = d[now]+1;
					q.push(next);
				}
				nowv[i] = nowv[aki];
				nowv[i+1] = nowv[aki+1];
				nowv[aki] = 0;
				nowv[aki+1] = 0;
			}
		}
	}
	cout<<"-1"<<endl;
	return 0;
}
