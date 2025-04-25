#include<bits/stdc++.h>
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
#define rep(i,n) for(int i=0;i<n;i++)
using namespace std;
using ll = long long;
using vll = vector<long long>;
using vvll = vector<vector<long long>>;
using vi = vector<int>;
using vvi = vector<vector<int>>;
using vb = vector<bool>;
using vvb = vector<vector<bool>>;
using pll = pair<long long,long long>;
using vpll = vector<pair<long long,long long>>;

int main(){
	cinll2(n,q);
	vpll a;
	pll now = {1,0};
	a.push_back(now);
	while(q--){
		cinint(k);
		if(k == 1){
			cinchar(c);
			if(c == 'U') now.second += 1;
			else if(c == 'D') now.second -= 1;
			else if(c == 'L') now.first -= 1;
			else now.first += 1;
			a.push_back(now);
		}
		else{
			cinll(p);
			if(a.size() > p) cout<<a[a.size()-p].first<<" "<<a[a.size()-p].second<<endl;
			else cout<<(p-a.size()+1)<<" "<<0<<endl;
		}
	}

	return 0;
}