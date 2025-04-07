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

//s番目を原点としたpointsに変換する（原点は除く）
vector<pair<double,double>> gentenSlide(vector<pair<double,double>> &ps, ll s){
    vector<pair<double,double>> ret;
    for(pll p: ps){
        ll nx = p.first - ps[s].first;
        ll ny = p.second - ps[s].second;
        if(nx == 0 && ny == 0) continue;
        ret.push_back({nx, ny});
    }
    return ret;
}

//原点からの偏角を計算
//度数法で返す
double henkaku(pair<double,double> p){
    double ret = atan2(p.second, p.first)/(2*PI) * 360;
    if(ret < 0) ret += 360;
    return ret;
}

double nasukaku(double a, double b){
    double ret = abs(a-b);
    if(180 < ret) return 360 - ret;
    return ret;
}

int main(void){
    cinll(n);
    vector<pair<double,double>> ps(n);
    rep(i,n) cin>>ps[i].first>>ps[i].second;
    double ans = 0;
    rep(i,n){
        //cout<<"基準点:"<<i<<"番目"<<endl;
        vector<pair<double,double>> sps = gentenSlide(ps, i);
        vector<double> henks;
        rep(j,n-1){
            henks.push_back(henkaku(sps[j]));
        }
        // cout<<"偏角ｓ"<<endl;
        // rep(j,n-1) cout<< henks[j]<<" ";
        // cout<<endl;
        sort(all(henks));
        rep(j, n-1){
            double targetAngle = henks[j] + 180;
            while(targetAngle >= 360) targetAngle -= 360;
            auto it = lower_bound(all(henks), targetAngle);
            ll ind = it-henks.begin();
            ans = max(ans, nasukaku(henks[j], henks[ind%(n-1)]));
            ans = max(ans, nasukaku(henks[j], henks[(ind+1)%(n-1)]));
            ans = max(ans, nasukaku(henks[j], henks[(ind+2)%(n-1)]));
            ans = max(ans, nasukaku(henks[j], henks[(ind-1+n-1)%(n-1)]));
            ans = max(ans, nasukaku(henks[j], henks[(ind-2+n-1)%(n-1)]));
        }
    }
    cout<<fixed<<setprecision(15)<<ans<<endl;
    return 0;
}
