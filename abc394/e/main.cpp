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

struct Node {
public:
	string s = "";
	ll l = 0;
	ll i;
	bool isStart;
	Node(ll i, bool sta): i(i), isStart(sta){}
};

bool isKaibun(string s){
	rep(i,s.size()/2) if(s[i] != s[(ll)s.size()-1-i]) return false;
	return true;
}

string rev(string s){
	reverse(all(s));
	return s;
}

ll kaiLen(vs &c, ll i, ll j){
	if(i == j) return 0;
	//型を入力
	queue<Node> q;
	
	//ここでスタート地点を入れてく
	q.push(Node(i, true));
	q.push(Node(j, false));

	vs visitS(c.size(), "NO");
	vs visitE(c.size(), "NO");
	visitS[i] = "";
	visitE[j] = "";
	
	while(!q.empty()){
		Node now = q.front();
		q.pop();
	
		//ここで次の奴らを追加していく
		rep(next,c.size()){
			if(now.isStart){
				//辺がなければいけない
				if(c[now.i][next] == '-') continue;
				if(visitS[next] != "NO") continue;
				visitS[next] = visitS[now.i]+string(1,c[now.i][next]);
				if(visitE[next] != "NO"){
					if(isKaibun(visitS[next] + rev(visitE[next]))){
						return visitS[next].size()+visitE[next].size();
					}
				}
				q.push(Node(next, now.isStart));
			}
			//終わりから伸びてきたとき
			else{
				//辺がなければいけない
				if(c[next][now.i] == '-') continue;
				if(visitE[next] != "NO") continue;
				visitE[next] = visitE[now.i]+string(1,c[next][now.i]);
				if(visitS[next] != "NO"){
					if(isKaibun(visitS[next] + rev(visitE[next]))){
						return visitS[next].size()+visitE[next].size();
					}
				}
				q.push(Node(next, now.isStart));
			}

		}
	}
	return -1;
}

int main(void){
    cinll(n);
	cinvs(c,n);
	rep(i,n){
		rep(j,n) cout<<kaiLen(c,i,j)<<" ";
		cout<<endl;
	}
	return 0;
}
