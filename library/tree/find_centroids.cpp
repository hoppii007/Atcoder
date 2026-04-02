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



//木の重心を求めるやつ
//木DPで全部の部分木の大きさを求めるやり方
//n : 木の頂点数
//centroids : 最初は空にしといて、重心の候補が入る
//size_of_subtree : 各頂点を根としたときの部分ぎの大きさ。最初は全部0にしとく
//nexts : 次に行ける場所全部
//これを呼び出すときに渡したvが根となる。
void findCentroids(const ll &n, vll &centroids, vll &size_of_subtree, vvll &nexts, ll v, ll parent = -1){
	//まずは自分をカウント
	size_of_subtree[v] = 1;
	//全子供のサイズがn/2以下か
	bool is_centroid = true;
	//全部の子供のやつを求める
	for(ll next : nexts[v]){
		//親にはいかなくていい
		if(next == parent) continue;
		//子供の部分木のサイズを求める
		findCentroids(n, centroids, size_of_subtree, nexts, next, v);
		//子供の部分木のサイズを自分のサイズに加算
		size_of_subtree[v] += size_of_subtree[next];
		//子供の部分木のサイズがn/2を超えたらアウト
		if(size_of_subtree[next] > n/2) is_centroid = false;
	}
	//自分より上のサイズもn/2を超えたらアウト
	if(n-size_of_subtree[v] > n/2) is_centroid = false;

	if(is_centroid == true) centroids.push_back(v);
}


//木の重心を求めるやつ
//存在証明に近いやつ（更新が終わったら重心になってるの証明ガチ天才）