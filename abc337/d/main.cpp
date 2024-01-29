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

//i行目の答えを返す
ll rowSolve(vs &grid,int i,ll k){
	ll bsum = 0;
	ll dsum = 0;
	rep(j,k){
		if(grid[i][j] == '.') dsum++;
		if(grid[i][j] == 'x') bsum++;
	}
	ll ret = mod;
	if(bsum == 0) ret = min(ret,dsum);
	rep(j,k,grid[i].size()){
		if(grid[i][j-k] == '.') dsum--;
		if(grid[i][j-k] == 'x') bsum--;
		if(grid[i][j] == '.') dsum++;
		if(grid[i][j] == 'x') bsum++;
		if(bsum == 0) ret = min(ret,dsum);
	}
	return ret;
}
//j列目の答えを返す
ll columnSolve(vs &grid, int j,ll k){
	ll bsum = 0;
	ll dsum = 0;
	ll ret = mod;
	rep(i,k){
		if(grid[i][j] == '.') dsum++;
		if(grid[i][j] == 'x') bsum++;
	}
	if(bsum == 0) ret = min(ret,dsum);
	rep(i,k,grid.size()){
		if(grid[i-k][j] == '.') dsum--;
		if(grid[i-k][j] == 'x') bsum--;
		if(grid[i][j] == '.') dsum++;
		if(grid[i][j] == 'x') bsum++;
		if(bsum == 0) ret = min(ret,dsum);
	}
	return ret;
}

int main(void){
	cinll(h,w,k);
	cinvs(grid,h);
	ll ans = mod;
	//横に可能性があるとき
	if(k <= w) rep(i,h) ans = min(ans,rowSolve(grid,i,k));
	if(k <= h) rep(j,w) ans = min(ans,columnSolve(grid,j,k));
	if(ans == mod) out(-1);
	else out(ans);
	//縦に可能性があるとき
	return 0;
}
