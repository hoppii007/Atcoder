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
const ll inf = 1301001001001001000LL;
const double PI=3.1415926535897932384626433832795028841971;

//表示
#define overload1(a,b,NAME,...) NAME
#define coutYesReturn() do {coutYes(); return 0; } while(0)
#define coutYesReturnIf(a) do { if(a){ coutYesReturn(); }} while(0)
#define coutNoReturn() do {coutNo(); return 0;}
#define coutNoReturnIf(a) do {if(a){ coutNoReturn(); }} while(0)
#define coutIfReturn(a,s) do{if(a){cout<<s<<endl; return 0;}}while(0)
template<typename... T>
void coutll(T... a){ ((cout << a <<" "),...) << endl; }
void coutvll(vll a){ rep(i,a.size()) cout<<a[i]<<" "; }
void coutvlln(vll a){ rep(i,a.size()) cout<<a[i]<<endl; }
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

struct S {
    ll l,r,p;
};

//座標圧縮した後の座標が返ってくる
//元の数字 -> 圧縮後の座標が取れるmap
map<ll,ll> zaatu(vll &a){
    set<ll> s;
    rep(i,a.size()) s.insert(a[i]);
    map<ll,ll> ret;
    ll i = 0;
    for(ll t: s) ret[t] = i++;
    return ret;
}

//{v,i}の順で入れる
template<typename T>
pair<T,ll> maxvi(vector<T> &a){
    pair<T,ll> ret = {a[0],0};
    rep(i,a.size()) if(ret.first < a[i]){
        ret = {a[i],i};
    }
    return ret;
}

template<typename T>
T maxv(vector<T> &a){
    T ret = a[0];
    rep(i,a.size()) if(ret < a[i]){
        ret = a[i];
    }
    return ret;
}

//{v,i}の順で入れる
template<typename T>
pair<T,ll> minvi(vector<T> &a){
    pair<T,ll> ret = {a[0],0};
    rep(i,a.size()) if(ret.first > a[i]){
        ret = {a[i],i};
    }
    return ret;
}

template<typename T>
T minv(vector<T> &a){
    T ret = a[0];
    rep(i,a.size()) if(ret > a[i]){
        ret = a[i];
    }
    return ret;
}

int main(void){
    cinll(n,w,c);
    vector<S> stones;
    vll ps;
    ll total = 0;
    rep(i,n){
        cinll(l,r,p);
        total += p;
        stones.push_back(S{l,r,p});
        ps.push_back(stones[i].l);
        ps.push_back(stones[i].r);
    }
    ps.push_back(0);
    ps.push_back(w+1);

    //座標圧縮
    sort(all(ps));
    ps.erase(unique(all(ps)), ps.end());
    map<ll,ll> ips;
    rep(i,ps.size()) ips[ps[i]] = i;
    
    rep(i,n){
        stones[i].l = ips[stones[i].l];
        stones[i].r = ips[stones[i].r];
    }
    
    vll ls(ps.size(),0);
    rep(i,stones.size()) ls[stones[i].l] += stones[i].p;
    mrep(i,ps.size()-2) ls[i] += ls[i+1];
    vll rs(ps.size(),0);
    rep(i,stones.size()) rs[stones[i].r] += stones[i].p;
    rep(i,1,ps.size()) rs[i] += rs[i-1];
    
    cout<<"ps  :";
    rep(i,ps.size()) cout<<ps[i]<<" ";
    cout<<endl;
    cout<<"ls  :";
    coutvll(ls);
    cout<<endl;
    cout<<"rs  :";
    coutvll(rs);
    cout<<endl;

    ll ans = inf;
    rep(i,ps.size()){
        ll idx = lower_bound(all(ps), ps[i]+c-1) - ps.begin();
        if(idx == ps.size()) continue;
        cout<<"l:"<<ps[i]<<" "<<"r:"<<ps[idx]<<endl;
        ans = min(ans, total-ls[idx]-rs[i]);
    }
    cout<<ans<<endl;

    return 0;
}
