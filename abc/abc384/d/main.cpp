#include<bits/stdc++.h>
//入力系
#define cinll(...) ll __VA_ARGS__; input(__VA_ARGS__);
#define cinill(...) ll __VA_ARGS__; input_index(__VA_ARGS__);
#define cinint(...) int __VA_ARGS__; input(__VA_ARGS__);
#define ciniint(...) int __VA_ARGS__; input_index(__VA_ARGS__);
#define cinstr(...) string __VA_ARGS__; input(__VA_ARGS__);
#define cinchar(...) char __VA_ARGS__; input(__VA_ARGS__);
#define cinvll(a,n) vll a(n); rep(i,n) cin>>a[i];
#define cinvvll(a,n,m) vvll a(n,vll(m)); rep(i,n) rep(j,m) cin>>a[i][j];
#define cinvs(a,n) vs a(n); rep(i,n) cin>>a[i];
//繰り返し系
#define rep1(n) for(ll i=0;i<n;i++)
#define rep2(i,n) for(ll i=0;i<n;i++)
#define rep3(i,a,n) for(ll i=a;i<n;i++)
#define rep4(i,a,n,b) for(ll i=a;i<n;i+=b)
#define overload4(a,b,c,d,e,...) e
#define rep(...) overload4(__VA_ARGS__,rep4,rep3,rep2,rep1)(__VA_ARGS__)
#define mrep1(n) for(ll i=n;i>=0;i--)
#define mrep2(i,n) for(ll i=n;i>=0;i--)
#define mrep3(i,n,a) for(ll i=n;i>=a;i--)
#define mrep4(i,n,a,b) for(ll i=n;i>=a;i-=b)
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
const double PI=3.1415926535897932384626433832795028841971;

//便利系
//vectorをjoinするやつ
template<typename T>
string join(const vector<T> &a,string c){
	string ret = "";
	if(a.size()==0) return ret;
	
    //もともとstringなら
	if constexpr (std::is_same_v<T, std::string> || std::is_same_v<T, char>) {
	    rep(i,a.size()-1) ret += a[i]+c;
	    return ret + a[a.size()-1];
    }
    //もともとstringじゃなければto_stringして結合
	else{
	    rep(i,a.size()-1) ret += to_string(a[i]) + c;
	    return ret + to_string(a[a.size()-1]);
	}
	
}
//空白区切り表示
template<class T, class... Ts>
void out(const T& a,const Ts&... b){
	cout<<a;
	(cout<< ... << (cout << ' ',b));
	cout<<endl;
}
template<class T>
void out(const vector<T> &a){
    out(join(a," "));
}
//入力
template<class... T>
void input(T&... a){
	(cin >> ... >> a);
}

template<class... T>
void input_index(T&... a){
    (cin >> ... >> a);
    ((a--), ...);
}

int main(void){
    cinll(n,s);
	cinvll(a,n);
	vll ra(n+1,0);
	rep(i,n) ra[i+1] = ra[i]+a[i];
	s %= ra[n];

	vll a2(n*2);
	rep(i,2*n) a2[i] = a[i%n];
	vll ra2(n*2+1, 0);
	rep(i,2*n) ra2[i+1] = ra2[i]+a2[i];


	ll sum = 0;
	bool isOK = false;
	
	//尺取り法
	//伸びれば伸びるほどfalseになるようにする
	//区間の長さn(配列の要素数)
	//区間の伸び縮みによって状態が変化する時に使える
	//O(N)で条件を満たす全区間[0,n)を走査
	//[i,i)でもdiscoveryが呼ばれることに注意（でも[n,n)では呼ばれない）
	//区間[l,r)
	ll l = 0;
	ll r = 0;
	//現状の区間[l,r)が条件を満たすか？
	std::function<bool()> syaku_check = [&](){
		//区間がない時はtrueにする
		return sum <= s;
	};
	//rを追加する処理（r++は別でやるから、状態変化のみ）
	std::function<void()> add_r = [&](){
		sum += a2[r];
	};
	//lを削除する処理（状態変化のみ）
	std::function<void()> remove_l = [&](){
		sum -= a2[l];
	};
	//OKな区間を見つけた時に（その状態の時に）やる処理
	std::function<void()> discovery = [&](){
		if(sum == s) isOK = true;
	};
	while(l < n*2){                                                                                                                                       
		//rをなるたけ進める
		while(r < n*2 && syaku_check()){
			//まずOKな列を一つ発見
			discovery();
	
			//rを区間に入れる処理
			add_r();
	
			//実際に区間を広げる
			r++;
		}
	
		//rを伸ばさなくてもOK or rを伸ばし切ってまだOKの時にここに来る
		if(syaku_check()) discovery();
	
		//一旦縮める
		remove_l();
		l++;
		while(r < l) r++;
	
		//lをokになるまで縮める(空の時はOK)
		while(!syaku_check()){
			remove_l();
			l++;
		}
	}
	if(isOK) cout<<"Yes"<<endl;
	else cout<<"No"<<endl;
	return 0;
}
