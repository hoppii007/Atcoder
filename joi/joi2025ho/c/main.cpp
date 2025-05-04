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

//座標圧縮した後の座標が返ってくる
//元の数字 -> 圧縮後の座標が取れるmap
//　小さい順になる
map<ll,ll> zaatu(vll &a){
	set<ll> s;
	rep(i,a.size()) s.insert(a[i]);
	map<ll,ll> ret;
	ll i = 0;
	for(ll t: s) ret[t] = i++;
	return ret;
}

struct Q {
	ll i;
	ll l,r,x;
};

map<ll,ll> iza;

int main(void){
    cinll(n,m,p);
	multiset<pll> ms;
	vvll abc(m);
	vll coms;
	rep(i,m){
		cinll(a,b,c);
		a--; b--; c--;
		abc[i] = {a,b,c};
		coms.push_back(c);
	}
	
	//step1. comを座標圧縮
	map<ll,ll> za = zaatu(coms);
	for(pll z: za) iza[z.second] = z.first;
	vector<vpll> com(za.size());
	rep(i,abc.size()) com[za[abc[i][2]]].push_back({abc[i][0], abc[i][1]}); 

	//step2. 各i(<=com.size())に対して、i以降のみ使う場合の一番短い範囲（閉区間）Lminを求める
	//を使って、まずi = com.size()-1から、UFのsizeが1になるまで追加（その間Lmin[i]はinfにし続ける）
	//sizeが1になるところが見つかったら、辺を削除できる間削除し続ける
	//出てきたOK区間を、区間の短い順にソートしておく
	//useLminを用意し、空にしておく(set<pll>で、一つ目の小さい順に並び替え)
	vll inNum(n,0);
	vll Lmin(com.size(),inf);
	ll botti = n;
	ll i = 0;
	multiset<pll> edges;
	while(i < com.size()){
		//i番目の変を全て追加
		for(pll p: com[i]){
			edges.insert(p);
			if(inNum[p.second] == 0) botti--;
			inNum[p.second]++;
		}
		if(botti == 1){
			Lmin[0] = i;
			break;
		}
		i++;
	}
	if(i == com.size()){
		cinll(q);
		while(q--){
			cinll(l,r,x);
			cout<<"No"<<endl;
		}
		return 0;
	}
	i = 1;
	while(i < com.size()){
		//i-1が経営してる辺を全て消す
		for(pll p: com[i-1]){
			inNum[p.second]--;
			if(inNum[p.second] == 0) botti++;
		}
		Lmin[i] = Lmin[i-1];
		while(botti > 1){
			Lmin[i]++;
			if(Lmin[i] == com.size()) break;
			for(pll p: com[Lmin[i]]){
				if(inNum[p.second] == 0) botti--;
				inNum[p.second]++;
			}
		}
		if(Lmin[i] == com.size()){
			Lmin[i] = inf;
			break;
		}
		i++;
	}
	// cout<<"Lmin :";
	// rep(i,com.size()) cout<<Lmin[i]<<" ";
	// cout<<endl;

	//trueだったら第1引数が手前に来る
	//p2 < p1として作ると降順になる
	//p1 < p2として作ると昇順になる
	
	std::function<bool(pll&,pll&)> compf2 = [&](const pll& p1, const pll &p2){
		return p1.second-p1.first+1 < p2.second-p2.first+1;
	};
	vpll Lminv;
	rep(i,com.size()) if(Lmin[i] != inf) Lminv.push_back({iza[i], iza[Lmin[i]]});
	sort(all(Lminv), compf2);

	// cout<<"Lminq:";
	// for(pll p: Lminq) cout<<"["<<p.first<<","<<p.second<<"] ";
	// cout<<endl;

	multiset<pll> useLmin;

	//step3. クエリを、考えられる区間が短い順に並び替える
	vector<Q> querys;
	cinll(q);
	rep(i,q){
		cinll(l,r,x);
		l--; r--;
		querys.push_back(Q{i,l,r,x});
	}
	//trueだったら第1引数が手前に来る
	//p1 < p2として作ると昇順になる
	//p2 < p1として作ると降順になる
	
	//sort(all(v), compf);みたいに使ってね
	std::function<bool(Q&,Q&)> compf = [&](const Q &p1,const Q &p2) {
		return p1.r-p1.l+1+p1.x < p2.r-p2.l+1+p2.x;
	};
	sort(all(querys), compf);

	ll left = 0;
	vb ans(q, false);
	//step4. 区間が一番短いクエリを取り出し、区間の長さが｜クエリ｜以下のOK区間を全てuseLminに入れる
	for(Q query: querys){
		//cout<<"query: "<<query.i<<" "<<query.l<<" "<<query.r<<" "<<query.x<<endl;
		while(left < Lminv.size()){
			if(Lminv[left].second-Lminv[left].first+1 > query.r-query.l+1+query.x) break;
			useLmin.insert(Lminv[left++]);
		}
		// cout<<"useLmin :";
		// for(pll use: useLmin){
		// 	cout<<"["<<use.first<<","<<use.second<<"] ";
		// }
		// cout<<endl;
		auto it = useLmin.lower_bound({query.l-query.x, -inf});
		if(it != useLmin.end() && (*it).second <= query.r+query.x){
			ans[query.i] = true;
		}
		else ans[query.i] = false;
	}
	rep(i,q) cout<<(ans[i]?"Yes":"No")<<endl;
	// そのクエリの左側-x以上になる最初のOK区間を取り出し、その右側がqr+x以下ならOK!

	return 0;
}
