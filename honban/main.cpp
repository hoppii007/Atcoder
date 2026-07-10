#include<bits/stdc++.h>
//入力系
#define cinll(...) ll __VA_ARGS__; input(__VA_ARGS__);
#define cinint(...) int __VA_ARGS__; input(__VA_ARGS__);
#define cinstr(...) string __VA_ARGS__; input(__VA_ARGS__);
#define cinchar(...) char __VA_ARGS__; input(__VA_ARGS__);
#define cindouble(...) double __VA_ARGS__; input(__VA_ARGS__);
#define cinvll(a,n) vll a(n); rep(i,n) cin>>a[i];
#define cinvvll(a,n,m) vvll a(n,vll(m)); rep(i,n) rep(j,m) cin>>a[i][j];
#define cinvs(a,n) vs a(n); rep(i,n) cin>>a[i];
#define cinvpll(a,n) vpll a(n); rep(i,n) cin>>a[i].fst>>a[i].snd;
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
#define rall(a) a.rbegin(),a.rend()
//vector系
#define pb push_back
//書くのが長いやつ
#define fst first
#define snd second
using namespace std;
//型系
using ll = long long;
using vll = vector<ll>;
using vvll = vector<vll>;
using vi = vector<int>;
using vvi = vector<vi>;
using vb = vector<bool>;
using vvb = vector<vb>;
using vd = vector<double>;
using vvd = vector<vd>;
using vc = vector<char>;
using vvc = vector<vc>;
using vs = vector<string>;
using pll = pair<long long,long long>;
using pi = pair<int,int>;
using pd = pair<double,double>;
using sll = set<long long>;
using vsll = vector<sll>;
using vpll = vector<pll>;
using vpi = vector<pi>;
using vpd = vector<pd>;
using vvpll  = vector<vpll>;

const ll mod = 998244353LL;
// const ll mod = 1000000007LL;
// const ll mod = 10007;
//const ll inf = 1300100100100100100LL;
const ll inf = 1001001001LL;
const double PI=3.1415926535897932384626433832795028841971;

//入力
template<class... T>
void input(T&... a){
    (cin >> ... >> a);
}
//複数ソート
template<class... T>
void sorts(vector<T>&... a){
    (sort(all(a)),...);
}
//便利関数
template<typename T> bool chmax(T &a, T b){ if(a < b) {a = b; return true;} return false; }
template<typename T> bool chmin(T &a, T b){ if(a > b) {a = b; return true;} return false; }

//配列表示用
template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) { 
    rep(i,vec.size()) os << vec[i] << (i==(ll)vec.size()-1?"":" ");
    return os;
}
// pair 表示用
template <typename T, typename S>
std::ostream& operator<<(std::ostream& os, const std::pair<T,S>& p) { 
    os << p.fst << " " << p.snd;
    return os;
}


/*
------------------------
**どの問題?**

**difficulty**

**solved**
(自力AC / 解説AC / 未AC)

**felt**
（簡単 / 中 / 難しい）

**考察ノート**

**難所**

## 問題全体のメモ
-------------------------
*/


// --------------------------- ここまでテンプレ ----------------------------

/*
------------------------
**どの問題?**
ABC214C

**difficulty**
319

**solved**
(自力AC / 解説AC / 未AC)

**felt**
（簡単 / 中 / 難しい）

**考察ノート**
グラフっぽい見た目をしているのでグラフにしたい。
時刻管理より辺にコストをつけて最短経路とかにしたいので、
高橋くんが始めて宝石を誰かに手渡す時刻Sを0として、そこから各人iに宝石が渡る時間を求めれば良さそう
グラフとしては高橋くんの頂点+各す抜けくんの頂点で、すぬけくんの頂点は円環になっててコストはtのままでOK
高橋くん -> 各すぬけくんのコストは、渡す時刻-Sで良さそう


**難所**

## 問題全体のメモ
-------------------------
*/

//O(E+V)でsからの最短経路がもとまる
//変のコストが0か1しかない場合は使える
//nextsには{cost,index}にしといてね
//始点が複数あって全部の中から最短が欲しい時は、ちょっと変更して最初から全部入れるようにしたほうがいい（何回も呼ばないほうがいい）
vll bfs01(vector<vpll> &nexts, vll starts){
    vll bfs01s(nexts.size(), inf);
    deque<pll> q;
    for(ll s: starts){
        bfs01s[s] = 0;
        q.push_front({0,s});
    }
    while(!q.empty()){
        pll now = q.front();
        q.pop_front();
        for(pll next: nexts[now.second]){
            ll ncost = bfs01s[now.second]+next.first;
            if(bfs01s[next.second] <= ncost) continue;
            bfs01s[next.second] = ncost;
            if(next.first == 0) q.push_front({ncost, next.second});
            else q.push_back({ncost, next.second});
        }
    }
    return bfs01s;
}

int main(){
    cinll(n);
    cinvll(s, n);
    cinvll(t, n);
    ll mi = *min_element(all(t));
    vvpll nexts(n+1);
    rep(i, n){
        ll ni = (i+1)%n;
        nexts[i].pb({s[i],ni});
        nexts[n].pb({t[i]-mi, i});
    }
    vll bfs = bfs01(nexts, {n});
    rep(i, n) cout << mi+bfs[i] << endl;
    return 0;
}