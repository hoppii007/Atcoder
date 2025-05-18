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

//O((E+V)logV)でsからの最短経路がもとまる
//負の辺があるとダメ
//nextsには{cost,index}にしといてね
//始点が複数あって全部の中から最短が欲しい時は、ちょっと変更して最初から全部入れるようにしたほうがいい（dijkstraは何回も呼ばないほうがいい）
//辺のコストが1しかないならbfsでOK
//辺のコストが0か1しかないならbfs01でOK
vll dijkstra(vector<vpll> &nexts, vll starts){
    struct dijk_comp{
        bool operator()(const pll &p1,const pll &p2) const{
            return p1.first > p2.first;
        }
    };
    vll dijk(nexts.size(), inf);
    priority_queue<pll,vector<pll>,dijk_comp> q;
    for(ll s: starts){
        dijk[s] = 0;
        q.push({0,s});
    }
    while(!q.empty()){
        pll now = q.top();
        q.pop();
        //すでに更新されていれば無視
        if(now.first > dijk[now.second]) continue;
        for(pll next: nexts[now.second]){
            ll ncost = next.first + now.first;
            if(dijk[next.second] > ncost){
                dijk[next.second] = ncost;
                q.push({ncost, next.second});
            }
        }
    }
    return dijk;
}

//t以下がいくつあるか？
//aは昇順ソート済み
ll cntLte(vll &a, ll t){
    auto it = upper_bound(all(a), t);
    return it - a.begin();
}

int main(void){
    cinll(n,m);
    cinll(s,t,l,k);
    s--; t--;
    vector<vpll> nexts(n);
    rep(i,m){
        cinll(a,b,c);
        a--; b--;
        nexts[a].push_back({c,b});
        nexts[b].push_back({c,a});
    }
    vll dijks = dijkstra(nexts, {s});
    vll dijkt = dijkstra(nexts, {t});

    if(dijks[t] <= k){
        cout<<n*(n-1)/2<<endl;
        return 0;
    }

    vll sds;
    rep(i,n) sds.push_back(dijks[i]);
    sort(all(sds));
    vll sdt;
    rep(i,n) sdt.push_back(dijkt[i]);
    sort(all(sdt));
    
    ll ans = 0;
    rep(i,n){
        ans += cntLte(sdt, k-l-dijks[i]);
    }
    cout<<ans<<endl;

    return 0;
}
