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

//強連結成分ごとにid（0から）を振った配列を返す
//sからの連結成分については全て処理できる
vll scc(vvll &nexts){
	//最終的にsccIDを格納するやつ
	vll sccIds(nexts.size(),-1);

	//逆辺のグラフを作成
	vvll invNexts(nexts.size());
	rep(i,nexts.size()){
		for(ll next: nexts[i]){
			invNexts[next].push_back(i);
		}
	}

	//帰りがけ順で記録していくやつ
	ll order = 0;
	ll sccId = 0;
	
	vll orders(nexts.size());
	vb visitIki(nexts.size(), false);
	std::function<void(ll)> scc1 = [&](ll now){
		visitIki[now] = true;
		for(ll next: nexts[now]){
			if(visitIki[next]) continue;
			scc1(next);
		}
		
		orders[now] = order++;
	};
	std::function<void(ll)> scc2 = [&](ll now){
		sccIds[now] = sccId;
		for(ll next: invNexts[now]){
			if(sccIds[next] != -1) continue;
			scc2(next);
		}
	};
	//全部の頂点について、そこから行けるところのorderを全部記録する
	rep(i, nexts.size()) if(visitIki[i] == false) scc1(i);

	//{帰りがけのOrder、index}のペアを降順にソート
	vpll orderIds(nexts.size());
	rep(i,nexts.size()) orderIds[i] = {orders[i], i};
	sort(all(orderIds));
	reverse(all(orderIds));

	rep(i,nexts.size()) if(sccIds[orderIds[i].second] == -1){
		scc2(orderIds[i].second);
		sccId++;
	} 
	return sccIds;
}

int main(void){
    cinll(n,m);
	vector<set<ll>> nextSets(n);
	rep(i,m){
		cinll(a,b);
		a--; b--;
		nextSets[a].insert(b);
	}
	vvll nexts(n);
	rep(i,n) for(ll next: nextSets[i]){
		nexts[i].push_back(next);
	}

	vll sccIds = scc(nexts);
	map<ll,ll> counts;
	rep(i,n) {
		if(counts.count(sccIds[i]) == 0) counts[sccIds[i]] = 0;
		counts[sccIds[i]]++;
	}
	ll ans = 0;
	for(pll p: counts){
		ans += (p.second-1)*p.second/2;
	}
	cout<<ans<<endl;

	return 0;
}
