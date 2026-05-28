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
#define cvvll1(name,a,b) vvll name(a, vll(b))
#define cvvll2(name,a,b,c) vvll name(a, vll(b,c))
#define cvvlloverload2(name,a,b,c,d,...) d
#define make_vvll(...) cvvlloverload2(__VA_ARGS__,cvvll2,cvvll1)(__VA_ARGS__)
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
const ll inf = 1300100100100100100LL;
const double PI=3.1415926535897932384626433832795028841971;

//表示
#define overload1(a,b,NAME,...) NAME
#define coutYesReturn() do {coutYes(); return 0; } while(0)
#define coutYesReturnIf(a) do { if(a){ coutYesReturn(); }} while(0)
#define coutNoReturn() do {coutNo(); return 0;} while(0)
#define coutNoReturnIf(a) do {if(a){ coutNoReturn(); }} while(0)
#define coutReturnIf(a,s) do{if(a){cout<<s<<endl; return 0;}}while(0)
template<typename... T>
void coutll(T... a){ ((cout << a <<" "),...) << endl; }
void coutvll(vll &a){ rep(i,a.size()) cout<<a[i]<<" "; cout<<endl; }
void coutvll(string name, vll &a){ cout<<name<<":"; coutvll(a); }
void coutvlln(vll &a){ rep(i,a.size()) cout<<a[i]<<endl; }
void coutYes(){ cout<<"Yes"<<endl; }
void coutNo(){ cout<<"No"<<endl; }
void coutYesNo(bool a){ cout<<(a?"Yes":"No")<<endl; }
void coutIf(bool a, string s, string t){ cout<<(a?s:t)<<endl; }
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


// --------------------- ここまでテンプレ ------------------------

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

    ll LOG = 60;

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
    cinll(n, q, x);
    cinvll(w, n);
    ll total = accumulate(all(w), 0LL);
    // 全ての場所 i から、リセットされて次入る最初の場所を記録
    vll nexts(n);
    vpll vnexts(n);
    ll r = 0;
    ll now = w[0];
    ll now_cnt = 1;
    rep(i, n){
        if(x-now >= total){
            now_cnt += (x-now)/total * n;
            now += (x-now)/total * total;
        }
        while(now < x){
            now += w[(r+1)%n];
            now_cnt++;
            r++;
        }
        nexts[i] = (r+1)%n;
        vnexts[i] = {now_cnt, (r+1)%n};
        now_cnt--;
        now -= w[i];
    }

    Doubling doub(nexts);
    while(q--){
        cinll(k);
        cout << vnexts[doub.kth_next(0, k-1)].fst << endl;
    }
    
    return 0;
}