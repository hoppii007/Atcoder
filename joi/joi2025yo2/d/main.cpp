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

struct S {
	ll a,b,i;
};

struct SS {
	ll i,br,s;
};

int main(void){
    cinll(n,m,q);
	cinvll(a,n);
	cinvll(b,n);
	vector<set<ll>> kirai(n);
	rep(i,m){
		cinll(u,v);
		u--; v--;
		kirai[u].insert(v);
		kirai[v].insert(u);
	}
	rep(i,n) kirai[i].insert(i);

	vector<S> as(n);
	rep(i,n) as[i] = {a[i], b[i], i};
	std::function<bool(S&, S&)> compfa = [&](const S &p1,const S &p2) {
		return p1.a > p2.a;
	};

	vector<S> bs(n);
	rep(i,n) bs[i] = {a[i], b[i], i};
	std::function<bool(S&, S&)> compfb = [&](const S &p1,const S &p2) {
		return p1.b > p2.b;
	};
	sort(all(bs), compfb);
	
	struct comp{
		bool operator()(const SS &p1,const SS &p2) const{
			return p1.s < p2.s;
		}
	};

	priority_queue<SS,vector<SS>,comp> que;
	set<pll> used;

	rep(i,n){
		//i番目が最初に行けるやつを探す
		ll br = -1;
		rep(j,n) if(kirai[as[i].i].count(bs[j].i) == 0){
			br = j;
			break;
		}
		if(br == -1) continue;
		pll p = {min(as[i].i, bs[br].i), max(as[i].i, bs[br].i)};
		//cout<<"いったんね "<<as[i].i<<" "<<bs[br].i<<" "<<endl;
		kirai[as[i].i].insert(bs[br].i);
		kirai[bs[br].i].insert(as[i].i);
		que.push({i,br,max(as[i].a, bs[br].a)+max(as[i].b,bs[br].b)});
	}

	vll xs;

	while(!que.empty() && xs.size() <= 400010){
		SS now = que.top();
		//cout<<now.i<<" "<<bs[now.br].i<<" "<<now.s<<endl;
		que.pop();
		xs.push_back(now.s);
		//brで次に入れれるやつがあれば入れる
		ll br = now.br+1;
		while(br < bs.size() && (kirai[as[now.i].i].count(bs[br].i)>0) ) br++;
		if(br < bs.size()){
			kirai[as[now.i].i].insert(bs[br].i);
			kirai[bs[br].i].insert(as[now.i].i);
			//cout<<now.i<<" "<<bs[br].i<<" 入れるべかな"<<endl;
			que.push({now.i, br, max(as[now.i].a,bs[br].a)+max(as[now.i].b,bs[br].b)});
		}
	}

	while(q--){
		cinll(x);
		x--;
		cout<<xs[x]<<endl;
	}


	return 0;
}
