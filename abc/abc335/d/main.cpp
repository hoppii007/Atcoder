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
//便利系
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

string v_join(vll &a,string c){
	string ret = "";
	rep(i,a.size()-1) ret += (to_string(a[i]) + c);
	return ret + to_string(a[a.size()-1]);
}


int main(void){
	cinll(n);
    vvll grid = vvll(n,vll(n,-2));
    grid[n/2][n/2] = -1;
    ll i = n/2;
    ll j = n/2;
    ll count = 1;
    pll d = {0,1};
    while(count < n*n){
        if(grid[i][j] != -2){
            i--;
            //右に行くようにする
            d = {0,1};
            continue;
        }
        grid[i][j] = count++;
        if(grid[i][j] == -2) grid[i][j] = count++;
        i += d.first;
        j += d.second;
        if(count == n*n) continue;
        if(d.first==0 && d.second==1 && grid[i+1][j] == -2) d = {1,0};
        if(d.first==1&& d.second==0 && grid[i][j-1] == -2) d = {0,-1};
        if(d.first==0&& d.second==-1 && grid[i-1][j] == -2) d = {-1,0};
        if(d.first==-1&& d.second==0 && grid[i][j+1] == -2) d = {0,1};
    }
    rep(i,n){
		rep(j,n-1){
			if(grid[i][j] == -1) cout<<"T ";
			else cout<<grid[i][j]<<" ";
		}
		cout<<grid[i][grid[i].size()-1]<<endl;
    }
}
