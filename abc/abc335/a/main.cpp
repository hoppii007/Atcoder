#include<bits/stdc++.h>
#define in_ll(a) ll a; cin>>a; 
#define in_string(s) string s; cin>>s;
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
	in_string(s);
	string t = "";
	rep(i,s.size()-1) t += s[i];
	t += "4";
	cout<<t<<endl;
	return 0;
}