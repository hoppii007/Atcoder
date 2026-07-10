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
/////////////////////////
**どの問題?**

**difficulty**

**solved**
(自力AC / 解説AC / 未AC)

**felt**
（簡単 / 中 / 難しい）

**考察ノート**

**難所**

## 問題全体のメモ
//////////////////////////
*/


// --------------------------- ここまでテンプレ ----------------------------

/*
------------------------
**どの問題?**
ABC214E

**difficulty**
1835

**solved**
(自力AC / 解説AC / 未AC)

**felt**
（簡単 / 中 / 難しい）

**考察ノート**
とりあえず、自分だけが入れられる箱がある人は、そこに入れていい。
その箱に入れられる人が複数ある時、どんな順番に入れていけばいいか？を考えると良さそう
貪欲かなぁ
区間スケジューリング的に、終わりが早い人を選択するといいのかも。終わりが遅い人はまだ入れられる可能性があり、終わりが早い人はチャンスが少ないということなので。
一つ以上被っている区間に対して、一番左から入れるボールを決定していけば良さそう？決定の回数はN回なので、次を選ぶ処理をO(1)でできれば計算量はよさそう。
証明しとこうかな。
いやー、厳密には証明してませんが、「より可能性がある方を残す」貪欲なのでいいと思います。
その箱に入るボールが1つしかいない時にそれを選べばいいのはそれはそうで、複数あった場合、最適解がこの貪欲の候補Dを選ばず、D'を選んだとします。
最適解のその後にDが選ばれていない場合は、ここでDを選んでも良く、選ばれている場合は先のDを今選んだことにして、後からDを入れていたところにD'を入れられる（D'の方が終了が遅いので）
よって証明できてるのでこれで良さそう

**難所**

## 問題全体のメモ
-------------------------
*/

//trueだったら第1引数が手前に来る
//p2 < p1として作ると降順になる
//p1 < p2として作ると昇順になる

struct set_comp{
    bool operator()(const pll &p1,const pll &p2) const{
      return p1.snd < p2.snd;
    }
};

int solve(){
    cinll(n);

    // 座標に対して、その座標で始まる奴らと終わる奴ら
    map<ll,vpll> starts,ends;

    // 区間が開始する座標集合
    vll start_ps;

    rep(i, n){
        cinll(l, r);
        starts[l].pb({l, r});
        ends[r].pb({l, r});
        start_ps.pb(l);
    }
    sort(all(start_ps));

    // 現状入れられる区間（r昇順）
    multiset<pll,set_comp> now;

    ll now_p = start_ps[0];
    for(pll p: starts[now_p]) now.insert(p);
    ll ans = 0;
    while(true){
        while(!now.empty() && (*now.begin()).snd < now_p) now.erase(now.begin());
        if(now.empty()){
            auto it = lower_bound(all(start_ps), now_p);
            if(it == start_ps.end()) break;
            now_p = *it;
            for(pll p: starts[now_p]) now.insert(p);
            continue;
        }
        auto it = now.begin();
        ans++;
        now.erase(it);
        now_p++;
        for(pll p: starts[now_p]) now.insert(p);

    }

    if(ans == n) cout << "Yes" << endl;
    else cout << "No" << endl;
    return 0;
}

int main(){
    cinll(t);
    while(t--) solve();
    return 0;
}