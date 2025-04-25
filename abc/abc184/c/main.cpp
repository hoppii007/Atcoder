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

bool canOne(ll r1,ll c1,ll r2,ll c2){
	return r1+c1 == r2+c2 || r1-c1 == r2-c2 || (abs(r1-r2)+abs(c1-c2)<=3);
}

bool canTwo(ll r1,ll c1,ll r2,ll c2){
	ll x = (c2+r2)-(c1-r1);
	ll x2 = (c1+r1)-(c2-r2);
	if(x%2 == 0 || x2%2 == 0) return true;
	for(ll tx = r1-2;tx <= r1+2;tx++){
		for(ll ty = c1-2;ty <= c2+2;ty++){
			if(canOne(tx,ty,r2,c2)) return true;
		}
	}
	if(canOne(r1-3,c1,r2,c2)) return true;
	if(canOne(r1+3,c1,r2,c2)) return true;
	if(canOne(r1,c1-3,r2,c2)) return true;
	if(canOne(r1,c1+3,r2,c2)) return true;
	return false;
}

int main(void){
	cinll2(r1,c1);
	cinll2(r2,c2);
	ll p = abs(r1-r2);
	ll q = abs(c1-c2);
	if(r1 == r2 && c1 == c2){
		cout<<0<<endl;
		return 0;
	}
	//1回でいける
	if(canOne(r1,c1,r2,c2)){
		cout<<1<<endl;
		return 0;
	}
	//2回で行ける
	else if((p+q)%2 == 0 || p+q <= 6 || abs(p-q) <= 3){
		cout<<2<<endl;
		return 0;
	}
	//それ以外は3回かかる
	else{
		cout<<3<<endl;
	}
	return 0;
}
