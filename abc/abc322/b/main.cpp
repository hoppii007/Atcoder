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
//繰り返し系
#define rep(i,n) for(int i=0;i<n;i++)
using namespace std;
//型系
using ll = long long;
using vll = vector<long long>;
using vvll = vector<vector<long long>>;
using vi = vector<int>;
using vvi = vector<vector<int>>;
using vb = vector<bool>;
using vvb = vector<vector<bool>>;
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

bool isSetto(string s,string t){
	rep(i,s.size()) if(s[i] != t[i]) return false;
	return true;
}

bool isSetubi(string s,string t){
	rep(i,s.size()) if(s[s.size()-i-1] != t[t.size()-i-1]) return false;
	return true;
}

int main(void){
	cinll2(n,m);
	cinstring2(s,t);
	bool isMae = isSetto(s,t);
	bool isUsi = isSetubi(s,t);
	if(isMae && isUsi) cout<<0<<endl;
	else if(isMae) cout<<1<<endl;
	else if(isUsi) cout<<2<<endl;
	else cout<<3<<endl;
	return 0;
}
