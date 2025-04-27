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



struct UnionFind {
public:
    vll par; // par[i]:iの親の番号　(例) par[3] = 2 : 3の親が2
    vll kosu;
    ll Count; //全体として何個の集合があるか
    
    UnionFind(ll n) : par(n),kosu(n),Count(n) { //最初は全てが根であるとして初期化
        rep(i,n){
            par[i] = i;
            kosu[i] = 1;
        }
    }

    ll root(ll x) { // データxが属する木の根を再帰で得る：root(x) = {xの木の根}
        if (par[x] == x) return x;
        return par[x] = root(par[x]);
    }

    void unite(ll x, ll y) { // xとyの木を併合
        ll rx = root(x); //xの根をrx
        ll ry = root(y); //yの根をry
        if (rx == ry) return; //xとyの根が同じ(=同じ木にある)時はそのまま
        par[rx] = ry; //xとyの根
        kosu[ry] += kosu[rx];
        Count--;
    }

    bool same(ll x, ll y) { // 2つのデータx, yが属する木が同じならtrueを返す
        ll rx = root(x);
        ll ry = root(y);
        return rx == ry;
    }
    
    ll size(ll x){
        return kosu[root(x)];
    }
    ll size(){
        return Count;
    }
};

vll getuf(UnionFind &uf,ll n,ll r){
	vll ret;
	rep(i,n) if(uf.root(i) == r) ret.push_back(i);
	return ret;
}

ll q = 0;
ll query(ll a,ll b){
	q++;
	vll blacks = {1,2,3,4,5,6,34,45,21,87};
	sort(all(blacks));
	bool ina = binary_search(all(blacks),a);
	bool inb = binary_search(all(blacks),b);
	//if(a == 2 && b == 3) cout<<ina<<" "<<inb<<endl;
	if(ina == inb) return 0;
	return 1;
}

bool debug = true;
int main(void){
	ll n = 1000;
	UnionFind uf(n);
	//そのrootの仲間が全員同じ色かどうか
	map<ll,bool> isSame;
	//まずは二個ずつに分ける(500回)
	vll sames;
	vvll noSames;
	ll amari4 = -1;
	ll amari8 = -1;
	for(ll i=0;i<n-1;i += 2){
		ll t;
		if(debug){
			t = query(i+1,i+2);
		}
		else{
			cout<<"? "<<(i+1)<<" "<<(i+2)<<endl;
			cin>>t;
		}
		ll l = uf.root(i);
		ll r = uf.root(i+1);
		uf.unite(i,i+1);
		if(t == 0){
			isSame[uf.root(i)] = true;
			sames.push_back(uf.root(i));
		}
		else if(t == -1){
			return 0;
		}
		else{
			isSame[uf.root(i)] = false;
			noSames.push_back({l,r});
		}
	}
	// cout<<"bunkatu1"<<endl;
	// rep(i,n) if(isSame[uf.root(i)] == false) cout<<i<<" ";
	// cout<<endl;
	//今のところsamesには要素数2の塊がある。これは全部割り切れるはず
	//2個ずつをさらにくっつけていく(最大250回)（合計750回）
	vll sames2;
	for(ll i=0;i<(ll)sames.size()-1;i+=2){
		ll t;
		if(debug){
			t = query(sames[i]+1,sames[i+1]+1);
		}
		else{
			cout<<"? "<<sames[i]+1<<" "<<sames[i+1]+1<<endl;
			cin>>t;
		}
		ll l = uf.root(sames[i]);
		ll r = uf.root(sames[i+1]);
		uf.unite(sames[i],sames[i+1]);
		if(t == 0){
			isSame[uf.root(sames[i])] = true;
			sames2.push_back(uf.root(sames[i]));
		}
		else if(t == -1){
			return 0;
		}
		else{
			isSame[uf.root(sames[i])] = false;
			noSames.push_back({l,r});
		}
	}
	//cout<<"bunkatu2"<<endl;
	sames = vll(sames2.size());
	rep(i,sames2.size()) sames[i] = sames2[i];
	sames2 = vll();
	//更にくっつけて、要素数8を作る（最大125回）（合計875回）
	for(ll i=0;i<(ll)sames.size()-1;i+=2){
		ll t;
		if(debug){
			t = query(sames[i]+1,sames[i+1]+1);
		}
		else{
			cout<<"? "<<sames[i]+1<<" "<<sames[i+1]+1<<endl;
			cin>>t;
		}
		ll l = uf.root(sames[i]);
		ll r = uf.root(sames[i+1]);
		uf.unite(sames[i],sames[i+1]);
		if(t == 0){
			isSame[uf.root(sames[i])] = true;
			sames2.push_back(uf.root(sames[i]));
		}
		else if(t == -1){
			return 0;
		}
		else{
			isSame[uf.root(sames[i])] = false;
			noSames.push_back({l,r});
		}
	}
	//cout<<"bunkatu3"<<endl;
	//奇数個を分けた時、最後の一個は誰ともくっつかずにあまるから、それを保存しておく
	if(sames.size()%2 == 1){
		amari4 = sames[sames.size()-1];
	}
	sames = vll(sames2.size());
	rep(i,sames2.size()) sames[i] = sames2[i];
	sames2 = vll();
	//更にくっつけて、要素数16を作る（最大62回）（合計937回）
	for(ll i=0;i<(ll)sames.size()-1;i+=2){
		ll t;
		if(debug){
			t = query(sames[i]+1,sames[i+1]+1);
		}
		else{
			cout<<"? "<<sames[i]+1<<" "<<sames[i+1]+1<<endl;
			cin>>t;
		}
		ll l = uf.root(sames[i]);
		ll r = uf.root(sames[i+1]);
		uf.unite(sames[i],sames[i+1]);
		if(t == 0){
			isSame[uf.root(sames[i])] = true;
			sames2.push_back(uf.root(sames[i]));
		}
		else if(t == 0) {
			return 0;
		}
		else{
			isSame[uf.root(sames[i])] = false;
			noSames.push_back({l,r});
		}
	}
	//奇数個を分けた時、最後の一個は誰ともくっつかずにあまるから、それを保存しておく
	if(sames.size()%2 == 1){
		amari8 = sames[sames.size()-1];
	}
	//cout<<"分割はできた"<<endl;
	//ここまで来て長さ16のやつがあれば間違いなくセーフだから、それを一つ求めておく。
	ll white = -1;
	rep(i,n){
		if(uf.size(i) == 16){
			white = i;
			break;
		}
	}
	//cout<<"white:"<<white<<endl;
	//違うやつに入っている全部と、iを比べていって、違うやつが答え。

	vll ans;
	rep(i,noSames.size()){
		cout<<i<<endl;
		ll t;
		if(debug){
			t = query(white+1,noSames[i][0]+1);
		}
		else{
			cout<<"? "<<white+1<<" "<<noSames[i][0]+1<<endl;
			cin>>t;
		}
		if(t == 0){
			vll tmp = getuf(uf,n,noSames[i][1]);
			if(debug) cout<<tmp.size()<<endl;
			rep(j,tmp.size()) ans.push_back(tmp[j]);
		}
		else if(t == 1){
			vll tmp = getuf(uf,n,noSames[i][0]);
			if(debug) cout<<tmp.size()<<endl;
			rep(j,tmp.size()) ans.push_back(tmp[j]);
		}
		else{
			return 0;
		}
	}
	if(debug){
		rep(i,noSames.size()){
			cout<<v_join<ll>(noSames[i]," ")<<endl;
		}
	}
	if(ans.size() == 6) rep(i,n) if(uf.root(i) == amari4) ans.push_back(i);
	else if(ans.size() == 2) rep(i,n) if(uf.root(i) == amari8) ans.push_back(i);
	cout<<"! ";
	rep(i,ans.size()-1) cout<<ans[i]+1<<" ";
	cout<<ans[ans.size()-1]+1<<endl;
	return 0;
}
