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

int main(void){
    cinll(n,m,k);
	cinvll(a,n);
	vpll ap(n);
	rep(i,n) ap[i] = {a[i],i};
	sort(all(ap));
	sort(all(a));
	vll ra(n+1,0);
	rep(i,n) ra[i+1] = ra[i]+a[i];

	if(n == m){
		rep(i,n) cout<<0<<" ";
		cout<<endl;
		return 0;
	}

	//残り配れる枚数
	ll remain = k;
	rep(i,n) remain -= a[i];

	vll order(n);
	rep(i,n) order[ap[i].second] = i;

	//now番目のやつが当選するための最小獲得数
	//now番目のやつを落とせないような最小付与枚数
	std::function<ll(ll)> f = [&](ll od){
		//二分探索のジャッジ用のやつ
		//now番目にmid枚配られた時に、落とすことができるかを判定
		auto nibutan_judge = [&](ll mid){
			//n-m番目以降か、それより前かで分岐
			if(od >= n-m){
				//n-m番までと、n-m-1番の合計
				//最初の勝てないところを求める
				auto it = upper_bound(all(a), a[od]+mid);
				ll loseIdx = it - a.begin();
				//n-m-1番目から、loseIdxの一個前まで、a[od]+mid-a[j]+1の合計を求める
				//これは、m人勝つために必要な枚数
				ll necessary = 0;
				if(n-m-1 < loseIdx) necessary = (loseIdx - (n-m-1))*(a[od]+mid+1) - (ra[loseIdx] - ra[n-m-1]) - (mid+1);

				if(necessary <= remain-mid) return true;
				else return false;
			}
			else{
				//n-m番までと、n-m-1番の合計
				//最初の勝てないところを求める
				auto it = upper_bound(all(a), a[od]+mid);
				ll loseIdx = it - a.begin();
				//n-m-1番目から、loseIdxの一個前まで、a[od]+mid-a[j]+1の合計を求める
				//これは、m人勝つために必要な枚数
				ll necessary = 0;
				if(n-m < loseIdx) necessary = (loseIdx - (n-m))*(a[od]+mid+1) - (ra[loseIdx] - ra[n-m]);

				if(necessary <= remain-mid) return true;
				else return false;
			}
		};
		//-1枚付与で確定落とせる
		ll ok = -1;
		//残り+1枚配ったら確定落とせない
		ll ng = remain+1;
		//ooooxxxxxxみたいな感じ
		while(abs(ok-ng) > 1){
			ll mid = (ok+ng)/2;
			if(nibutan_judge(mid)) ok = mid;
			else ng = mid;
		}
		if(ng == remain+1) return -1LL;
		return ng;
	};

	rep(i,n){
		cout<<f(order[i])<<" ";
	}
	cout<<endl;
	return 0;
}
