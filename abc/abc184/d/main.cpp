#include<bits/stdc++.h>
//入力系
#define cinll(a) ll a; cin>>a; 
#define cinll2(a,b) ll a,b; cin>>a>>b;
#define cinll3(a,b,c) ll a,b,c; cin>>a>>b>>c;
#define cinint(a) int a; cin>>a;
#define cinint2(a,b) int a,b; cin>>a>>b;
#define cinint3(a,b,c) int a,b,c; cin>>a>>b>>c;
#define cinstring(s) string s; cin>>s;
#define cinstring2(s,t) string s,t; cin>>s>>t;
#define cinstring3(s,t,u) string s,t,u; cin>>s>>t>>u;
#define cinchar(c) char c; cin>>c;
#define cinvll(a,n) vll a(n); rep(i,n) cin>>a[i];
#define cinvs(a,n) vs a(n); rep(i,n) cin>>a[i];
//繰り返し系
#define rep(i,n) for(int i=0;i<n;i++)
#define rep1(i,n) for(int i=1;i<n;i++)
#define mrep(i,n) for(int i=n;i>=0;i--)
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

//便利系
//vectorをjoinするやつ
template<typename T>
string v_join(vector<T> &a,string c){
	string ret = "";
	rep(i,a.size()-1) ret += (to_string(a[i]) + c);
	return ret + to_string(a[a.size()-1]);
}


int main(void){
	static double dp[101][101][101];
	cinll3(a,b,c);
	rep(i,101) rep(j,101){
		dp[100][i][j] = 0;
		dp[i][100][j] = 0;
		dp[i][j][100] = 0;
	}
	mrep(i,99) mrep(j,99) mrep(k,99){
		if(i+j+k == 0) continue;
		dp[i][j][k] = 0;
		double sum = i+j+k;
		if(i > 0) dp[i][j][k] += (1+dp[i+1][j][k])*(i/sum);
		if(j > 0) dp[i][j][k] += (1+dp[i][j+1][k])*(j/sum);
		if(k > 0) dp[i][j][k] += (1+dp[i][j][k+1])*(k/sum);
	}
	cout<<fixed<<setprecision(9)<<dp[a][b][c]<<endl;
	
	return 0;
}
