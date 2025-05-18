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

//{v,i}の順で入れる
template<typename T>
pair<T,ll> maxv(vector<T> &a){
    pair<T,ll> ret = {a[0],0};
    rep(i,a.size()) if(ret.first < a[i]){
        ret = {a[i],i};
    }
    return ret;
}

//{v,i}の順で入れる
template<typename T>
pair<T,ll> minv(vector<T> &a){
    pair<T,ll> ret = {a[0],0};
    rep(i,a.size()) if(ret.first > a[i]){
        ret = {a[i],i};
    }
    return ret;
}

//aは昇順ソート済み
//以下の個数
ll cntLte(vll &a, ll t){
    auto it = upper_bound(all(a), t);
    return it - a.begin();
}
//未満の個数
ll cntLt(vll &a, ll t){
    auto it = lower_bound(all(a), t);
    return it - a.begin();
}
//以上の個数
ll cntGte(vll &a, ll t){
    auto it = lower_bound(all(a), t);
    ll idx = it-a.begin();
    return a.size() - idx;
}
//より大きい個数
ll cntGt(vll &a, ll t){
    auto it = upper_bound(all(a), t);
    ll idx = it-a.begin();
    return a.size() - idx;
}
int main(void){
    cinll(n,l);
    cinvll(x,n);
    cinll(q);
    sort(all(x));
    ll minx = minv(x).first;
    ll maxx = maxv(x).first;
    vll rx(n+1,0);
    rep(i,n) rx[i+1] = rx[i]+x[i];
    while(q--){
        cinll(s,g,t);
        ll ans = n;
        if(g <= s) {
            //まず一番右まで行く（移動コストかかる）
            ans += max(s,maxx)-s;
            //g以上の全てについて、sum(x-g)を計算
            //まずはg以上の最初のidxを取る
            ll idx = lower_bound(all(x), g) - x.begin();
            ans += rx[n]-rx[idx];
            ans -= cntGte(x, g)*g;
            //移動コスト
            ans += max(s,maxx)-g;

            //一番左まで行く
            ans += (g-min(g,minx)) * (cntGt(x,g)+1);
            idx = upper_bound(all(x), g) - x.begin();
            ans -= rx[idx];
            ans += cntLte(x, g) * g;
            ans += (g-min(g,minx)) * (cntGt(x,g)+1);
        }
        else{
            ans += s-min(s,minx);
            ll idx = upper_bound(all(x), g) - x.begin();
            ans -= rx[idx];
            ans += cntLte(x, g)*g;
            ans += g-min(s,minx);

            ans += (max(g,maxx) - g) * (cntLt(x,g)+1);
            idx = lower_bound(all(x), g) - x.begin();
            ans += rx[n]-rx[idx];
            ans -= cntGte(x, g)*g;
            ans += (max(g,maxx) - g) * (cntLt(x,g)+1);

        }
        if(ans <= t) cout<<"Yes"<<endl;
        else cout<<"No"<<endl;
    }
    return 0;
}
