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

void Next321Like(vll &digit,int i){
	//下のけたから見ていって、桁上がりするかをやってく
	digit[i]++;
	//一番上のけたじゃないとき
	if(i < digit.size()-1){
		//桁上がりする場合
		if(digit[i] >= digit[i+1]){
			digit[i] = i;
			Next321Like(digit,i+1);
		}
	}
	//一番上のけたの時
	else{
		if(digit[i] == 10){
			digit.push_back(i+1);
			digit[i] = i;
		}
	}
}

int main(void){
	cinll(n);
	vll digit(1,0);
	rep(i,n){
		Next321Like(digit,0);
		//cout<<v_join(digit,"")<<endl;
	}
	reverse(digit.begin(),digit.end());
	cout<<v_join(digit,"")<<endl;
	return 0;
}
