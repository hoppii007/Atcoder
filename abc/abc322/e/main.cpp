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

const ll INF = 1100100100LL;

//便利系
//vectorをjoinするやつ
template<typename T>
string v_join(vector<T> &a,string c){
	string ret = "";
	rep(i,a.size()-1) ret += (to_string(a[i]) + c);
	return ret + to_string(a[a.size()-1]);
}

bool canOnePlay(vll &a, ll p0,ll p1,ll p2,ll p3,ll p4){
	return p0 <= a[0] && p1 <= a[1] && p2 <= a[2] && p3 <= a[3] && p4 <= a[4];
}
bool tyoudo(vll &a , ll p0,ll p1,ll p2,ll p3,ll p4){
    return p0 >= a[0] && p1 >= a[0] && p2 >= a[0] && p3 >= a[0] && p4 >= a[0]; 
}

//dp[i][p0][p1][p2][p3][p4] := iまでを使って、それぞれの開発をp0,p1,p2,p3,p4以上にするときの最小
//dp[i][0][0][0][0][0] = 0
//dp[i][a_i_0]
int main(){
	
	cinll3(n,k,p);
	vll c(n);
	vvll a(n,vll(5));
	rep(i,n){
		cin>>c[i];
		rep(j,k) cin>>a[i][j];
		for(int j=k;j<5;j++) a[i][j] = 0;
	}
	ll dp[100][6][6][6][6][6];
	rep(i,n){
		rep(p1,6) rep(p2,6) rep(p3,6) rep(p4,6) rep(p5,6){
			dp[i][p1][p2][p3][p4][p5] = INF;
		}
		dp[i][0][0][0][0][0] = 0;
	}

	rep(i,n){
		rep(p0,6) rep(p1,6) rep(p2,6) rep(p3,6) rep(p4,6){
		    //全部0の時は考えなくてOK
		    if(p0 + p1 + p2 + p3 + p4 == 0 ) continue;
		    if(i == 0){
		        if(canOnePlay(a[i],p0,p1,p2,p3,p4)){
		            dp[i][p0][p1][p2][p3][p4] = c[i];
		        }
		        continue;
		    }
			dp[i][p0][p1][p2][p3][p4] = min(dp[i][p0][p1][p2][p3][p4],dp[i-1][max(0LL,p0-a[i][0])][max(0LL,p1-a[i][1])][max(0LL,p2-a[i][2])][max(0LL,p3-a[i][3])][max(0LL,p4-a[i][4])]+c[i]);
			//今までのやつが最小の可能性もある
			dp[i][p0][p1][p2][p3][p4] = min(dp[i][p0][p1][p2][p3][p4],dp[i-1][p0][p1][p2][p3][p4]);
		}

	}

	//後は同じi番目まで使う中で、全部1以上にする最少より全部2以上にする最小の方が小さい可能性がある

	int p0 = (k >= 1)? p:0;
	int p1 = (k >= 2)? p:0;
	int p2 = (k >= 3)? p:0;
	int p3 = (k >= 4)? p:0;
	int p4 = (k >= 5)? p:0;
	ll ans = dp[n-1][p0][p1][p2][p3][p4];
	if(ans == INF) cout<<-1<<endl;
	else cout<<ans<<endl;

	return 0;
}
