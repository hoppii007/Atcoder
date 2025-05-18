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
//書くのが長いやつ
#define fst first
#define snd second
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
using sll = set<long long>;
using vsll = vector<set<long long>>;
using vpll = vector<pair<long long,long long>>;
using vvpll  = vector<vector<pair<long long, long long>>>;
#define vmll vector<mll>
#define vvmll vector<vector<mll>>

const ll mod = 998244353LL;
//const ll mod = 1000000007LL;
const ll inf = 1301001001001001000LL;
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
template<typename T>
T chmax(T &a, T b){ if(a < b) a = b; return a; }
template<typename T>
T chmin(T &a, T b){ if(a > b) a = b; return a; }

//O(logB)でa^b % modが計算できる
ll mpow(ll a,ll b){
    if(b == 0) return 1;
    if(b == 1) return a%mod;
    if(b%2 == 0) return mpow((a*a)%mod,b/2)%mod;
    else return (a%mod)*mpow((a*a)%mod,(b-1)/2)%mod;
}

//aをn進数表記する
//[0,1,1]が帰ってきたとき、0a^0 + 1a^1 + 1a^2
vll toNsinsu(ll a, ll n){
    vll ret;
    while(a > 0){
        ret.push_back(a%n);
        a /= n;
    }
    return ret;
}

//割り算とかするならmodは素数前提
//modを変えたい時はmll::mod = 10みたいな感じ
struct mll{
public:
    static ll mod;
    ll value;
    mll(){
        this->value = 0;
    }
    mll(ll v){
        this->value = v%mll::mod;
        while(this->value < 0) this->value += mll::mod;
    }
    friend ostream& operator << (ostream& os, const mll& p){
        os << p.value;
        return os;
    }
    mll operator =(const ll &a){
        value = a;
        return *this;
    }
    friend mll operator -(const mll &a, const mll &b){ return mll(a.value-b.value); }
    friend mll operator -(const mll &a, const ll &b){ return mll(a.value-(b%mll::mod)); }
    friend mll operator -(const ll &a, const mll &b){ return mll((a%mll::mod)-b.value); }
    friend mll operator +(const mll &a, const mll &b){ return mll(a.value+b.value); }
    friend mll operator +(const mll &a, const ll &b){ return mll(a.value+(b%mll::mod)); }
    friend mll operator +(const ll &a, const mll &b){ return mll((a%mll::mod)+b.value); }
    friend mll operator *(const mll &a, const mll &b){ return mll(a.value*b.value); }
    friend mll operator *(const mll &a, const ll &b){ return mll(a.value*(b%mll::mod)); }
    friend mll operator *(const ll &a, const mll &b){ return mll((a%mll::mod)*b.value); }
    friend mll operator ^(const mll &a, const ll &b){
        if(b == 0) return mll(1);
        if(b%2 == 0) return (a*a)^(b/2);
        else return a*(a^(b-1));
    }
    friend mll operator /(const mll &a, const mll &b){ return a*(b^(mll::mod-2)); }
    friend mll operator /(const mll &a, const ll &b){ return a*(mll(b)^(mll::mod-2)); }
    friend mll operator /(const ll &a, const mll &b){ return a*(b^(mll::mod-2)); }
    friend bool operator <(const mll &a, const mll &b){ return a.value < b.value; }
    friend bool operator <(const mll &a, const ll &b){ return a.value < b; }
    friend bool operator <(const ll &a, const mll &b){ return a < b.value; }
    friend bool operator <=(const mll &a, const mll &b){ return a.value <= b.value; }
    friend bool operator <=(const mll &a, const ll &b){ return a.value <= b; }
    friend bool operator <=(const ll &a, const mll &b){ return a <= b.value; }
    friend bool operator >(const mll &a, const mll &b){ return a.value > b.value; }
    friend bool operator >(const mll &a, const ll &b){ return a.value > b; }
    friend bool operator >(const ll &a, const mll &b){ return a > b.value; }
    friend bool operator >=(const mll &a, const mll &b){ return a.value >= b.value; }
    friend bool operator >=(const mll &a, const ll &b){ return a.value >= b; }
    friend bool operator >=(const ll &a, const mll &b){ return a >= b.value; }
    friend bool operator ==(const mll &a, const mll &b){ return a.value == b.value; }
    friend bool operator ==(const mll &a, const ll &b){ return a.value == b; }
    friend bool operator ==(const ll &a, const mll &b){ return a == b.value; }
    friend bool operator !=(const mll &a, const mll &b){ return a.value != b.value; }
    friend bool operator !=(const mll &a, const ll &b){ return a.value != b; }
    friend bool operator !=(const ll &a, const mll &b){ return a != b.value; }
    friend void operator +=(mll &a, const mll &b){ a = a+b; }
    friend void operator +=(mll &a, const ll &b){ a = a+b; }
    friend void operator +=(ll &a, const mll &b){ a = (a+b).value; }
    friend void operator -=(mll &a, const mll &b){ a = a-b; }
    friend void operator -=(mll &a, const ll &b){ a = a-b; }
    friend void operator -=(ll &a, const mll &b){ a = (a-b).value; }
    friend void operator *=(mll &a, const mll &b){ a = a*b; }
    friend void operator *=(mll &a, const ll &b){ a = a*b; }
    friend void operator *=(ll &a, const mll &b){ a = (a*b).value; }
    friend void operator /=(mll &a, const mll &b){ a = a/b; }
    friend void operator /=(mll &a, const ll &b){ a = a/b; }
    friend void operator /=(ll &a, const mll &b){ a = (a/b).value; }
    friend void operator ++(mll &a, int){ a += 1; }
    friend void operator ++(mll &a){ a += 1; }
    friend void operator --(mll &a, int){ a -= 1; }
    friend void operator --(mll &a){ a -= 1; }
};
ll mll::mod = 998244353LL;

int main(){
    cinll(t);
    while(t--){
        //cout<<"t:"<<t<<endl;
        cinll(n,k);


        
    }
    return 0;
}