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
	cinll3(n,m,p);
	cinvll(a,n);
	cinvll(b,m);
	vll bsum(m);
	sort(b.begin(),b.end());
	bsum[0] = b[0];
	rep1(i,m) bsum[i] = bsum[i-1] + b[i];

	ll sum = 0;
	rep(i,n){

		//iを選んだ場合の合計を求める
		//p-a[i]以下のb[j]の合計と、p-a[i]より大きいb[j]についてはpをこの個数分追加すればいい
		sum += min(p,a[i])*m;
		ll tmp = max(0LL,p-a[i]);
		int dist = distance(b.begin(),upper_bound(b.begin(),b.end(),tmp));
		sum += (m-dist)*tmp;
		if(dist > 0) sum += bsum[dist-1];
	}
	cout<<sum<<endl;
	return 0;
}
