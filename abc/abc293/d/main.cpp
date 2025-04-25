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


struct Node {
public:
	ll i = -1;
	char c = 'G';
	Node(){}
	Node(ll i,char c){
		this->i = i;
		this->c = c;
	}
	bool operator <(const Node &n)const{
		if(i == n.i) return c < n.c;
		return i < n.i;
	}
	bool operator ==(const Node &n)const{
		return i == n.i && c == n.c;
	}
};

bool loop(map<Node,vector<Node>> &nexts,set<Node> &visit,Node now,Node first=Node(-1,'G'),Node prev=Node(-1,'G')){
	if(first.i == -1){
		first = now;
	}
	visit.insert(now);
	bool ret = false;
	for(Node next: nexts[now]){
		if(next == prev) continue;
		if(next == first) return true;
		if(visit.count(next) > 0) continue;
		ret = ret || loop(nexts,visit,next,first,now);
	}
	//次に行くべき場所が無いならループしない
	return ret;
}

int main(void){
	cinll(n,m);
	map<Node,vector<Node>> nexts;
	rep(i,m){
		ll a,c;
		char b,d;
		cin>>a>>b>>c>>d;
		nexts[Node(a,b)].push_back(Node(c,d));
		nexts[Node(c,d)].push_back(Node(a,b));
	}
	rep(i,1,n+1){
		nexts[Node(i,'R')].push_back(Node(i,'B'));
		nexts[Node(i,'B')].push_back(Node(i,'R'));
	}
	set<Node> visit;
	ll l = 0;
	ll all = 0;
	rep(i,1,n+1) for(char c : {'R','B'}){
		if(visit.count(Node(i,c)) > 0) continue;
		if(loop(nexts,visit,Node(i,c))) l++;
		all++;
	}
	cout<<l<<" "<<(all-l)<<endl;
	return 0;
}
