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

//考察
//f(n) := 「各頂点について、その頂点以下の部分技の中で下から長さKで分割した時に、あまる頂点の個数」とする-inf余る場合はアウトパターン
//nの全ての子供cについて、f(c) > 0となるcが3つ以上あればf(n) = -inf
//f(c) > 0となるcがちょうど2つの時、f(c1) > 0、f(c2) > 0として、f(c1)+f(c2)+1 == K => f(n) = 0、それ以外なら-inf
//f(c) > 0となるcがちょうど一つの時、f(n) = (f(c)+1)%k;
//f(c) > 0となるcがないとき、f(n) = 1%k
ll dfs(ll k, vvll &nexts, ll prev, ll now){
    ll count = 0;
    ll sum = 0;
    for(ll child: nexts[now]) if(child != prev){
        ll fc = dfs(k, nexts, now, child);
        if(fc == -inf) return -inf;
        if(fc > 0) count++;
        sum += fc;
    }
    if(count >= 3) return -inf;
    if(count == 2){
        if(sum+1 == k) return 0;
        else return -inf;
    }
    return (sum+1)%k;
}

int main(void){
    cinll(n,k);
    vvll nexts(n*k);
    rep(i,n*k-1){
        cinll(u,v);
        u--; v--;
        nexts[u].push_back(v);
        nexts[v].push_back(u);
    }
    if(dfs(k, nexts, -1, 0) == 0) cout<<"Yes"<<endl;
    else cout<<"No"<<endl;
    return 0;
}
