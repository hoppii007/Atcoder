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

/*
**どの問題?**

**difficulty**

**solved**
(自力AC / 解説AC / 未AC)

**felt**
（簡単 / 中 / 難しい）

**考察ノート**

**難所**

## 問題全体のメモ


int main(){

    return 0;
}

*/


// --------------------- ここまでテンプレ ------------------------

/*
**どの問題?**
ABC212F

**difficulty**
2332

**solved**
(自力AC / 解説AC / 未AC)

**felt**
難しい

**考察ノート**
Zの制約が小さければ、クエリを小さい順に並び替えた後にその時刻にどこにいるかをシミュレートしていけば良さそうですが、Zがデカいですね...
いや、でもやっぱりクエリを時刻小さい順に並べておけば、自分の状態としてあり得るのは「どこかの都市にいるか、どこかのバスにいるか」なので、
N+M個しか状態がないはずです。で、状態の切り替わりのタイミングだけ記録しつつ進んでいけばいい...と思ったけど、これはスタート地点が固定されている場合ですね
クエリをスタート地点ごとに分割してやるてもありますが、スタート地点の候補がN個、その後進んでいく状態の個数がN+M個なので、O(N*(N+M))とかになってダメそうです...
うーん困った
グラフの拡張みたいな感じでなんとかできないでしょうか。
頂点aを時刻pに出発するバス、みたいなのを一つの頂点として、行き先は例えばa -> a'という辺で、出発時刻をs、到着時刻をtとすると、
(a,p) -> (a',t以降で最初にa'からバスが出る時間)としておくと、すべての状態から行き先が一意にきまります。
そうするとダブリングが使えて、すべての場所、時刻からN回進んだ場所、みたいなのが記録でき、それを使って二分探索で初めてZを超える場所を探す、みたいなことをすればいい気がします
ただすべての場所、時刻に対してやろうとすると計算量は大丈夫でしょうか？
考慮すべきすべての頂点は、バスの本数しかないはずです（各バスの場所と開始時刻を頂点とするため）
とすると2*m敷かないはずなので、なんかいけそうな気がしてきました
整理してみましょう。
まず、頂点集合Sを、S = {(s_i, t_i) | s_i=i番目のバスの出発都市、t_i=i番目のバスの出発時刻}
として、各頂点から辺を(s_i, t_i) -> (s'_i, t'_i)（s'_iはバスiの到着都市、t'_iはバスiの到着時刻以降、最初に出発するバスの時刻（ない場合はinf））とします
すると、|S| = M、|E={辺の集合}| = Mです。（各辺に対して行き先が一意に定まるので）
この上でダブリングを行う計算量は、|S| * log(パスの最大長)なので、|S| * log |E|=MlogM
ダブリングの構築は間に合いそうです。
あとは各クエリに対して、そのクエリ設定で最初に出発する都市aと時刻pを求め、(a,p)から進んだ頂点(a', p')であって初めてp'がzを超える場所を二分探索で探し、その前の頂点から(a', p')に来る道中で、時刻Zのタイミングでバスに乗っているのかa'で待機しているのかを判断すれば良さそうです
これ、実装が大変だな...
丁寧に実装します。

**難所**
考察も難しいけど、グラフの拡張系のひらめき考察は得意なので、思ったより早めに方針はたった。が、そこからの実装がかなり重かった。

## 問題全体のメモ
*/

struct Bus{
    ll a, b, s, t;
    ll i;
    bool operator<(const Bus& bus) const {
        if(a != bus.a) return a < bus.a;
        return s < bus.s;
    }
};

/**
 * @brief ダブリング（Doubling）
 *
 * 各状態から1回の遷移先が決まっているとき、任意の状態から K 回先の遷移先を高速に求める。
 * 前処理でダブリングテーブルを構築し、クエリごとに K のビットを見て遷移を合成する。
 *
 * @note 前処理 O(N log K), クエリ O(log K)（N = 状態数, K = 最大遷移回数）
 */
struct Doubling{
    /** @brief dub[i][j] := 状態 i から 2^j 回遷移した先（遷移先がなければ -1） */
    vvll dub;

    ll LOG = 30;

    Doubling(){}

    /**
     * @brief ダブリングテーブルを構築する
     * @param nexts 各状態からの1回の遷移先。遷移先がない場合は -1。
     *              nexts のサイズが状態数 N となる。
     */
    Doubling(vll &nexts){
        ll n = nexts.size();
        dub.resize(n, vll(LOG+1));
        // 2^0 = 1回の遷移先をそのまま格納
        rep(i,n) dub[i][0] = nexts[i];
        // 2^j 回の遷移 = 2^(j-1) 回の遷移を2回適用
        rep(j,1,LOG+1) rep(i,n){
            if(dub[i][j-1] == -1) dub[i][j] = -1;
            else dub[i][j] = dub[dub[i][j-1]][j-1];
        }
    }

    /**
     * @brief 状態 n から k 回遷移した先を返す（単発クエリ用）
     * @param n 開始状態
     * @param k 遷移回数（0 <= k <= 2^60 ≒ 10^18）
     * @return k 回遷移した先の状態。途中で遷移先がなくなった場合は -1。
     * @note 全状態を一括で求める場合は kth_next_all を使うこと（キャッシュ効率が良い）
     */
    ll kth_next(ll n, ll k){
        for(ll j=0;(1LL << j) <= k;j++){
            if(k & (1LL << j)) n = dub[n][j];
            if(n == -1) return -1;
        }
        return n;
    }

    /**
     * @brief 全状態から k 回遷移した先を一括で求める
     * @param k 遷移回数（0 <= k <= 2^60 ≒ 10^18）
     * @return vll dest[i] := 状態 i から k 回遷移した先。遷移先がなければ -1。
     * @note kth_next を N 回呼ぶよりキャッシュ効率が良く高速
     */
    vll kth_next_all(ll k){
        ll n = dub.size();
        vll cur(n);
        rep(i,n) cur[i] = i;
        for(ll j=0;(1LL << j) <= k;j++){
            if(k & (1LL << j)){
                vll nxt(n);
                rep(i,n){
                    if(cur[i] == -1) nxt[i] = -1;
                    else nxt[i] = dub[cur[i]][j];
                }
                cur = nxt;
            }
        }
        return cur;
    }
};

int main(){
    cinll(n, m, q);

    // 各都市から出発するバスの時刻集合
    vector<vector<Bus>> stimes(n);
    vector<Bus> buses;

    rep(i, m){
        cinll(a, b, s, t);
        a--; b--;
        stimes[a].pb(Bus{a, b, s, t});
        buses.pb(Bus{a,b,s,t});
    }
    rep(i, n){
        stimes[i].pb(Bus{i,i,inf,inf});
        buses.pb(Bus{i,i,inf,inf});
    }
    rep(i, n) sort(all(stimes[i]));
    sort(all(buses));
    map<Bus,ll> to_idx;
    rep(i, buses.size()){
        buses[i].i = i;
        to_idx[buses[i]] = i;
    }
    rep(i, n) rep(j, stimes[i].size()) stimes[i][j].i = to_idx[stimes[i][j]];

    vll nexts(buses.size());
    rep(i, buses.size()){
        Bus bus = buses[i];
        if(bus.s == inf) {
            nexts[i] = i;
            continue;
        }
        // 頂点 bus.b で、時刻 t 以降で最初に出るバスを探す
        Bus nbus = *lower_bound(all(stimes[bus.b]), Bus{bus.b, bus.b, bus.t, bus.t});
        nexts[i] = nbus.i;
    }

    Doubling doub(nexts);

    // rep(i, buses.size()){
    //     cout << buses[i].a << " " << buses[i].b << " " << buses[i].s << " " << buses[i].t << endl;
    // }

    vll prev(buses.size());
    rep(i, buses.size()) prev[nexts[i]] = i;

    while(q--){
        cinll(x, y, z);
        y--;
        // 時刻x以降で最初に出るバスを求める
        Bus bus = *lower_bound(all(stimes[y]), Bus{y, y, x, x});
        //cout << bus.a<<"を" << bus.s << "に出発" << endl;
        if(bus.s == inf){
            cout << y+1 << endl;
            continue;
        }
        // busから始めて、最初にzより大きくなる箇所を探す
        //二分探索のジャッジ用のやつ
        auto nibutan_judge = [&](ll mid){
            return z <= buses[doub.kth_next(bus.i, mid)].s;
        };
        ll ok = inf;
        ll ng = -1;
        while(abs(ok-ng) > 1){
            ll mid = (ok+ng)/2;
            if(nibutan_judge(mid)) ok = mid;
            else ng = mid;
        }
        //cout << "ok=" << ok << endl;
        if(ok == 0){
            //cout << "ngo" << endl;
            // その場にとどまり続けてるか、バスに乗れるか？
            if(z <= bus.s) cout << y+1 << endl;
            else cout << bus.a+1 << " " << bus.b+1 << endl;
            continue;
        }
        // ok-1回移動したバスの到着時刻と比べればいい
        Bus bus2 = buses[doub.kth_next(bus.i, ok-1)];
        if(z <= bus2.t) cout << bus2.a+1 << " " << bus2.b+1 << endl;
        else cout << bus2.b+1 << endl;
    }


    // rep(i, n){
    //     cout << "i:" << i << " ";
    //     for(Bus b: stimes[i]) cout << "{" << b.b << " " << b.s << " " << b.t << "}, ";
    //     cout << endl;
    // }


    return 0;
}
