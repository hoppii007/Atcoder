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

//便利系
//vectorをjoinするやつ
template<typename T>
string v_join(vector<T> &a,string c){
	string ret = "";
	rep(i,a.size()-1) ret += (to_string(a[i]) + c);
	return ret + to_string(a[a.size()-1]);
}

//dp[i][j][k][l] := i桁決めていて、桁和はjで、mod sをとるとkになる
//lが1の時最小が確定
ll dp[16][130][130][2];

int main(void){
	//dp[i][j][k] = 桁数がi+1で、桁和がjで、mod jがkのやつ
	//最高桁を1にする
	//dp[i][j]
	cinll(n);
	vll digit;
	while(n > 0){
		digit.push_back(n%10);
		n /= 10;
	}
	reverse(digit.begin(),digit.end());
	ll m = digit.size();
	ll ans = 0;
	//桁和がsになる奴を数える
	rep1(s,127){
		rep(i,m+1) rep(j,s+1) rep(k,s) rep(l,2){
			dp[i][j][k][l] = 0;
		}
		dp[0][0][0][0] = 1;
		rep(i,m) rep(j,s+1) rep(k,s) rep(l,2){
			rep(d,10){
				ll ni = i+1;
				ll nj = j + d;
				ll nk = (10*k + d)%s;
				ll nl = l;
				if(nj > s) continue;
				//以下が確定していないとき
				if(l == 0){
					if(d > digit[i]) continue;
					if(d < digit[i]) nl = 1;
				}
				dp[ni][nj][nk][nl] += dp[i][j][k][l];
			}
		}
		ans += dp[m][s][0][1] + dp[m][s][0][0];
	}
	cout<<ans<<endl;

	return 0;
}
