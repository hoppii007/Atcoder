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

ll f(ll n){
	if(n%2 == 0) return n/2;
	else return n/2+1;
}

ll calcRootMaru(vc &s, vll &ldot, vll &rdot, ll i){
	ll num = rdot[i]-ldot[i]+1;
	return f(num);
}

int main(void){
    cinll(n,k);
	cinstr(ss);
	vc s(n);
	rep(i,n) s[i] = ss[i];
	rep(i,n) if(s[i] == 'o'){
		if(i-1 >= 0 && s[i-1] == '?') s[i-1] = '.';
		if(i+1 < n && s[i+1] == '?') s[i+1] = '.';
	}
	//この時点で.o.?????..??.o..o.o.???.o.???となっている
	//つまり、oと隣り合った?は存在しない
	
	//ldot[i]:=i以下で、繋がったまま最後に現れる?の座標
	//s[i] == '?'じゃない時は-inf
	vll ldot(n,-inf);
	rep(i,n){
		if(s[i] == '?'){
			if(i == 0 || ldot[i-1] == -inf) ldot[i] = i;
			else ldot[i] = ldot[i-1];
		}
	}
	vll rdot(n,inf);
	mrep(i,n-1){
		if(s[i] == '?'){
			if(i == n-1 || rdot[i+1] == inf) rdot[i] = i;
			else rdot[i] = rdot[i+1];
		}
	}

	//現状何個oが作れるか？
	ll maru = 0;
	rep(i,n){
		if(s[i] == '?'){
			maru += calcRootMaru(s, ldot, rdot, i);
			i = rdot[i];
		}
	}

	//元々のoの個数
	ll root = 0;
	rep(i,n) if(s[i] == 'o') root++;

	//各s[i]==?のところについて、s[i]を.にしたらmaru+root<Kとなるならo確定だし、oとしたらmaru+root<Kとなるなら.確定
	rep(i,n) if(s[i] == '?'){
		ll rNum = rdot[i] - i;
		ll lNum = i - ldot[i];
		ll rootMaru = calcRootMaru(s, ldot, rdot, i);
		//.にした時の最大個数が足りなければダメ
		ll tmp = maru - rootMaru + f(rNum) + f(lNum);
		if(tmp + root < k){
			s[i] = 'o';
			continue;
		}
		//oにした時の個数
		rNum = max(rNum-1, 0LL);
		lNum = max(lNum-1, 0LL);
		tmp = maru - rootMaru + 1 + f(rNum) + f(lNum);
		if(tmp+root < k){
			s[i] = '.';
			continue;
		}
		if(1+root > k){
			s[i] = '.';
			continue;
		}

	}

	rep(i,n) cout<<s[i];
	cout<<endl;
	return 0;
}
