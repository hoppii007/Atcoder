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


int main(void){
	cinint2(n,x);
	cinstring(s);
	rep(i,n){
		if(s[i] == 'o') x++;
		else x = max(0,x-1);
	}
	cout<<x<<endl;
	return 0;
}
