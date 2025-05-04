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

struct S{
	ll i, nol, ok, ng, nor;
};

//一回の行き先が決まってる状態から、K回先を計算する O(N log K)
//要素数:n
//各状態(0 ~ n)から、2^60回までの遷移先を求める(10^18ちょいくらい)
// dub[i][j] := iから2^j回遷移した先
//一回の行き先をnextsに入れとく
//行き先がないやつは-1
vector<vpll> dubling(vpll &nexts){
	vector<vpll> dub(nexts.size(), vpll(61));
	rep(i,nexts.size()) dub[i][0] = nexts[i];
	rep(j,1,61) rep(i,nexts.size()){
		dub[i][j] = dub[dub[i][j-1].second][j-1];
		dub[i][j].first += dub[i][j-1].first;
	}
	return dub;
}

int main(void){
    cinll(d,l,n);
	cinvll(c,d);
	rep(i,d) c[i]--;
	vll cc(d*2);
	rep(i,d) cc[i] = cc[i+d] = c[i];
	vll nextIdx(d);
	map<ll,ll> prevFound;
	rep(i,d*2) prevFound[cc[i]] = inf;
	mrep(i,d*2-1){
		if(i < d) nextIdx[i] = prevFound[cc[i]];
		prevFound[cc[i]] = i;
	}

	//各料理が出てきたindex
	vvll dishIdxs(100001);
	rep(i,d*2) dishIdxs[cc[i]].push_back(i);

	//dubling用
	vpll nexts(d);
	rep(i,d){
		if(i < nextIdx[i]) nexts[i] = {(nextIdx[i]-i-1)/l+1, nextIdx[i]%d};
		else nexts[i] = {(nextIdx[i]+d-i-1)/l+1, nextIdx[i]%d};
	}
	vector<vpll> dub = dubling(nexts);

	rep(i,n){
		cinll(k,f,t);
		k--; f--;
		if(dishIdxs[k].size() == 0){
			cout<<0<<endl;
			continue;
		}
		if(t == 1){
			if(c[f] == k) cout<<1<<endl;
			else cout<<0<<endl;
			continue;
		}

		//一番近いkを食べにいく
		ll neary = dishIdxs[k][lower_bound(all(dishIdxs[k]), f) - dishIdxs[k].begin()];
		//[f,neary]で食堂に行く回数
		if(f+(t-1)*l < neary){
			cout<<0<<endl;
			continue;
		}
		t -= max(0LL,(neary-f-1))/l+1+(f==neary?0:1);
		f = neary%d;
		//cout<<"neary:"<<neary<<" "<<t<<endl;

		ll ans = 0;
		ll now = f;
		ll sum = 0;
		mrep(i,30){
			if(t < sum+dub[now][i].first) continue;
			//cout<<"i="<<i<<" "<<now<<" "<<dub[now][i].first<<endl;
			sum += dub[now][i].first;
			ans += (1LL << i);
			now = dub[now][i].second;
		}
		//cout<<"sum="<<sum<<endl;
		if(ans == 0){
			if(c[f] == k) cout<<1<<endl;
			else cout<<0<<endl;
		}
		else{
			cout<<ans+1<<endl;
		}
	}
	// cout<<"nexts:";
	// rep(i,d) cout<<"["<<nexts[i].first<<","<<nexts[i].second<<"] ";
	// cout<<endl;
	// rep(i,d){
	// 	cout<<"dub["<<i<<"] : ";
	// 	rep(j,3) cout<<"["<<dub[i][j].first<<","<<dub[i][j].second<<"] ";
	// 	cout<<endl;
	// }
	return 0;
}
