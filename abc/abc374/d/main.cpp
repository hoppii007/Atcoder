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

struct Point {
public:
	ll x;
	ll y;
};

struct Line{
public:
	Point s,e;
};
//O(logB)でa^bが計算できる
ll mypow(ll a,ll b){
    if(b == 0) return 1;
    if(b == 1) return a;
    if(b%2 == 0) return mypow(a*a,b/2);
    else return a*mypow(a*a,(b-1)/2);
}

double dist(ll a,ll b,ll c,ll d){
	return sqrt((a-c)*(a-c) + (b-d)*(b-d));
}

int main(void){
	cinll(n,s,t);
	vvll lines(n,vll(4));
	vll tmp(n);
	rep(i,n) tmp[i] = i;
	rep(i,n) rep(j,4) cin>>lines[i][j];
	double ans = inf;
	do {
	    rep(i,mypow(2,n)){
			pll now = {0,0};
	        ll a = 1;
			double ngo = 0;
	        rep(j,n){
				//このとき左から右に行く
	            if(a&i){
					ngo += dist(now.first,now.second,lines[tmp[j]][0],lines[tmp[j]][1])/s+dist(lines[tmp[j]][0],lines[tmp[j]][1],lines[tmp[j]][2],lines[tmp[j]][3])/t;
					now.first = lines[tmp[j]][2];
					now.second = lines[tmp[j]][3];
				}
				//この時右から左に行く
				else{
					ngo += dist(now.first,now.second,lines[tmp[j]][2],lines[tmp[j]][3])/s+dist(lines[tmp[j]][0],lines[tmp[j]][1],lines[tmp[j]][2],lines[tmp[j]][3])/t;
					now.first = lines[tmp[j]][0];
					now.second = lines[tmp[j]][1];
				}
	            a *= 2;
	        }
			ans = min(ans,ngo);
	    }
	}while(next_permutation(all(tmp)));
	cout << std::fixed << std::setprecision(15) << ans << endl;
	return 0;
}
