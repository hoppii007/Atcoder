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
using vll = vector<long long>;
using vvll = vector<vector<long long>>;
using vi = vector<int>;
using vvi = vector<vector<int>>;
using vb = vector<bool>;
using vvb = vector<vector<bool>>;
using vd = vector<double>;
using vvd = vector<vector<double>>;
using vc = vector<char>;
using vvc = vector<vector<char>>;
using vs = vector<string>;
using pll = pair<long long,long long>;
using pi = pair<int,int>;
using pd = pair<double,double>;
using sll = set<long long>;
using vsll = vector<set<long long>>;
using vpll = vector<pair<long long,long long>>;
using vpi = vector<pi>;
using vpd = vector<pair<double, double>>;
using vvpll  = vector<vector<pair<long long, long long>>>;
#define vmll vector<mll>
#define vvmll vector<vector<mll>>

const ll mod = 998244353LL;
//const ll mod = 1000000007LL;
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

//aをn進数表記する
//[0,1,1]が帰ってきたとき、0a^0 + 1a^1 + 1a^2
vll to_nsinsu(ll a, ll n){
    vll ret;
    while(a > 0){
        ret.push_back(a%n);
        a /= n;
    }
    return ret;
}

int solve(){
    cinll(n, k);

    //Nを適当な心数表記したやつ（最上位桁が一番左）
    vll digits = to_nsinsu(n, 2);
    reverse(all(digits));
    //dp[i桁目まで考えて（0桁目は最上位の上）][状態がjで][smaller]なやつの値
    ll stateNum = 61;
    vector<vvll> dp(digits.size()+1, vvll(stateNum, vll(2, -1)));
    vector<vvll> dpc(digits.size()+1, vvll(stateNum, vll(2, 0)));
    
    //初期値の設定
    //だいたい最上位の上を0固定にして、Nと完全一致が一つある、としてスタートすればいい
    dp[0][/*最上位の上に0が一つある状態のstate*/0][0] = 0;
    dpc[0][0][0] = 1;
    
    //digits[0]から始めるけど、dp[1]から更新、みたいな感じ
    rep(i,digits.size()) rep(state,stateNum) rep(smaller,2){
        if(dp[i][state][smaller] == -1) continue;
        //何進数かでここは変わる
        ll lim = (smaller==1)?1:digits[i];
        rep(nd, lim+1){
            //nsmallerの設定はだいたいこれ
            ll nsmaller = smaller || nd < digits[i];
            //nstateの設定は問題ごとにadhoc
            //stateでndを選んだ時の新しいstate
            ll nstate = state + nd;
            //stateが変になる時、つまり遷移できない・しても意味がない時はcontinueでoK
            if(nstate >= stateNum) continue;
    
            //現状の一つのstateと、その遷移先が定まったから、あとは遷移
            //dpをmllで作っとけばmodとらなくてOK
            //遷移は桁和だったり、個数だったり総和だったりするから適切に足していく
            //総和の場合は個数用DPと組み合わせたりする
            //とにかく答えを出そうとして、そのために必要なSubDPを揃えていく感じ

            if(dp[i+1][nstate][nsmaller] == -1) dp[i+1][nstate][nsmaller] = 0;
            dp[i+1][nstate][nsmaller] = (dp[i+1][nstate][nsmaller]+dp[i][state][smaller])%mod;
            dpc[i+1][nstate][nsmaller] += dpc[i][state][smaller];
            if(nd == 1){
                dp[i+1][nstate][nsmaller] = (dp[i+1][nstate][nsmaller] + (1LL << (digits.size()-i-1))%mod*dpc[i][state][smaller]%mod)%mod;
            }
        }
    }
    cout << (max(dp[digits.size()][k][0],0LL) + max(0LL, dp[digits.size()][k][1]))%mod << endl;

    return 0;
}

int main(){
    cinll(t);
    while(t--) solve();
}