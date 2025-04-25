#include<bits/stdc++.h>
#define inll(a) ll a; cin>>a; 
#define inll2(a,b) ll a,b; cin>>a>>b;
#define inll3(a,b,c) ll a,b,c; cin>>a>>b>>c;
#define inint(a) int a; cin>>a;
#define inint2(a,b) int a,b; cin>>a>>b;
#define inint3(a,b,c) int a,b,c; cin>>a>>b>>c;
#define instring(s) string s; cin>>s;
#define instring2(s,t) string s,t; cin>>s>>t;
#define instring3(s,t,u) string s,t,u; cin>>s>>t>>u;
#define rep(i,n) for(int i=0;i<n;i++)
using namespace std;
using ll = long long;
using vll = vector<long long>;
using vvll = vector<vector<long long>>;
using vi = vector<int>;
using vvi = vector<vector<int>>;
using vb = vector<bool>;
using vvb = vector<vector<bool>>;

int main(){
	inll(n);
	rep(i,n+1) rep(j,n+1) rep(k,n+1){
		if(i+j+k <= n) cout<<i<<" "<<j<<" "<<k<<endl;
	}
	return 0;
}