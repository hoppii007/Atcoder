#include<bits/stdc++.h>
//入力系
#define cinll(...) ll __VA_ARGS__; input(__VA_ARGS__);
#define cinint(...) int __VA_ARGS__; input(__VA_ARGS__);
#define cinstr(...) string __VA_ARGS__; input(__VA_ARGS__);
#define cinchar(...) char __VA_ARGS__; input(__VA_ARGS__);
#define cinvll(a,n) vll a(n); rep(i,n) cin>>a[i];
#define cinvs(a,n) vs a(n); rep(i,n) cin>>a[i];
//繰り返し系
#define rep1(n) for(int i=0;i<n;i++)
#define rep2(i,n) for(int i=0;i<n;i++)
#define rep3(i,a,n) for(int i=a;i<n;i++)
#define rep4(i,a,n,b) for(int i=a;i<n;i+=b)
#define overload4(a,b,c,d,e,...) e
#define rep(...) overload4(__VA_ARGS__,rep4,rep3,rep2,rep1)(__VA_ARGS__)
#define mrep1(n) for(int i=n;i>=0;i--)
#define mrep2(i,n) for(int i=n;i>=0;i--)
#define mrep3(i,n,a) for(int i=n;i>=a;i--)
#define mrep4(i,n,a,b) for(int i=n;i>=a;i-=b)
#define mrep(...) overload4(__VA_ARGS__,mrep4,mrep3,mrep2,mrep1)(__VA_ARGS__)
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

const ll mod = 998244353LL;

//便利系
//vectorをjoinするやつ
template<typename T>
string v_join(vector<T> &a,string c){
	string ret = "";
	rep(i,a.size()-1) ret += (to_string(a[i]) + c);
	return ret + to_string(a[a.size()-1]);
}
//空白区切り表示
template<class T, class... Ts>
void out(const T& a,const Ts&... b){
	cout<<a;
	(cout<< ... << (cout << ' ',b));
	cout<<endl;
}
//入力
template<class... T>
void input(T&... a){
	(cin >> ... >> a);
}


int main(void){
	cinll(n);
	//2^a >= nとなる最小のaを見つける
	ll a = 1;
	while(pow(2LL,a) < n) a++;
	out(a);
	vvll eat(a,vll());
	//3個だったとすると2人でいけて、00,10,01なので、一個目は誰も食わない2個目は1人目が食う、3個目は2人目が食う
	//n個だったとするとa人でいけて、
	rep(i,n){
		//i個目をj人目が食うかどうか
		//10だったら
		rep(j,a){
			if(i&(1LL<<j)) eat[j].push_back(i+1);
		}
	}
	rep(i,a){
		out(eat[i].size(),v_join(eat[i]," "));
	}
	cinstr(s);
	//帰ってきたのが01だったとする
	ll ans = 0;
	rep(i,s.size()) if(s[i] == '1') ans += (1LL << i);
	out(ans+1);
	return 0;
}
