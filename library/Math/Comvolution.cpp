#include<bits/stdc++.h>
//入力系
#define cinll(a) ll a; cin>>a; 
#define cinll2(a,b) ll a,b; cin>>a>>b;
#define cinll3(a,b,c) ll a,b,c; cin>>a>>b>>c;
#define cinint(a) int a; cin>>a;
#define cinint2(a,b) int a,b; cin>>a>>b;
#define cinint3(a,b,c) int a,b,c; cin>>a>>b>>c;
#define cinstring(s) string s; cin>>s;
#define cinstring2(s,t) string s,t; cin>>s>>t;
#define cinstring3(s,t,u) string s,t,u; cin>>s>>t>>u;
#define cinchar(c) char c; cin>>c;
#define cinvll(a,n) vll a(n); rep(i,n) cin>>a[i];
#define cinvs(a,n) vs a(n); rep(i,n) cin>>a[i];
//繰り返し系
#define rep(i,n) for(int i=0;i<n;i++)
#define rep1(i,n) for(int i=1;i<n;i++)
#define mrep(i,n) for(int i=n;i>=0;i--)
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

//コンボリューションを列挙 -> v_combs
//コンボリューションを出すとき、どこを使ったかの情報のみを列挙 -> vb_combs
class Comb{
private:
	//vは最初全部falseにしといてね
	static void vb_combs_sub(vvb &vv, vb &v, int now_count, int now, int k){
		if(now_count == k) return;
		//ちょうど最後の一個を担当するとき
		if(now_count == k-1){
			for(int i=now;i<v.size();i++){
				v[i] = true;
				vv.push_back(v);
				v[i] = false;
			}
			return;
		}
		//まだまだいけるとき
		for(int i=now;i<v.size();i++){
			v[i] = true;
			vb_combs_sub(vv,v,now_count+1,i+1,k);
			v[i] = false;
		}
	}
public:
	Comb(){}
	//vector<bool>の中でk本trueが立つようにする
	//最終的にはコンボリューションの二次元配列を作る
	static vvb vb_combs(int n,int k){
		vb b = vb(n,false);
		vvb bs;
		vb_combs_sub(bs,b,0,0,k);
		return bs;
	}

	//T型の配列aのコンボリューションを列挙する
	template<typename T>
	vector<vector<T>> v_combs(vector<T> &a, int k){
		vvb bs = vb_combs(a.size(),k);
		vector<vector<T>> ret;
		rep(i,bs.size()) ret.push_back(extract<T>(a,bs[i]));
		return ret;
	}
};