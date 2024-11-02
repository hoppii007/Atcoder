#include<bits/stdc++.h>
//入力系
#define cinll(...) ll __VA_ARGS__; input(__VA_ARGS__);
#define cinint(...) int __VA_ARGS__; input(__VA_ARGS__);
#define cinstr(...) string __VA_ARGS__; input(__VA_ARGS__);
#define cinchar(...) char __VA_ARGS__; input(__VA_ARGS__);
#define cinvll(a,n) vll a(n); rep(i,n) cin>>a[i];
#define cinvvll(a,n,m) vvll a(n,vll(m)); rep(i,n) rep(j,m) cin>>a[i][j];
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
//iterator系
#define all(a) a.begin(),a.end()
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
const ll inf = 100100100100100100LL;

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

ll f(pll a,pll b,ll q){
	ll n = 0;
	ll ret = inf;
	pll p1,p2;
	//aの方がコスパが良いとき
	if(a.first * b.second > b.first * a.second){
		p1 = a;
		p2 = b;
	}
	else{
		p1 = b;
		p2 = a;
	}
	//コスパいい方で割り切れるときはそれが最小
	if(q%p1.first == 0){
		return q/p1.first * p1.second;
	}
	ret = (q/p1.first + 1) * p1.second;
	ll tmp = ret - p1.second;
	ll nokori = q%p1.first;
	if(nokori%p2.first == 0) tmp += (nokori/p2.first) * p2.second;
	else tmp += (nokori/p2.first + 1) * p2.second;
	return min(ret,tmp);
}

int main(void){
	cinll(n,x);
	vvll m(n,vll(4));
	rep(i,n) rep(j,4) cin>>m[i][j];

	//x円を使って、「一番生産できない日」を最大化する
	//x円を使って、Wの最小をQにすることができるか？
	// -> これがある程度高速に解ければ、O(logQ*N*)とかにできる。
	// -> なぜなら、Qは最大でも10^9程度
	// -> 各日について、処理数がQを超える最小コストを求めればいい。
	// -> ある整数のペア（a,b）（c,d）があったとき、firstの合計がQを超える組み合わせのうち、一番小さい奴
	// -> コスパいい方を優先して全部買って、一個減らしてみるところまで試してみてもOK

	return 0;
}
