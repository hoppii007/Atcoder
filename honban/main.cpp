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

//const ll mod = 998244353LL;
const ll mod = 1000000007LL;
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

// 座標型
struct Point {
    ll x, y;
};

// ベルトコンベアの線分
struct Conveyor {
    Point from, to;
};


ll N,M,K;
//各分別装置設置場所の情報
struct Sorting_machine {
    ll i; //i番目の分別機を置く
    ll u,v;
    Point pos;
};
vector<Sorting_machine> sorting_machines;
//i番目の分別装置で、j種類目のゴミを出口1から吐き出す確率
vvd sorting_machine_probs;

//各処理装置設置場所の情報
struct Trashbox {
    ll i;       //何番目の処理装置を置くか？
    Point pos;
};
vector<Trashbox> trash_boxes;


//nexts : i番目の設置場所から行ける設置場所（N以上ならN-i番目の分別装置）
//交差はない前提
//行ける設置場所には全て何かしらが設置してある
double get_score(vector<Sorting_machine> &sms, ll s){
    vvd dp(M, vd(N,-1));

    //場所nowに、i番目のゴミが来た時に、正しく分別される確率
    function<double(ll,ll)> dfs = [&](ll now,ll i){
        //処理場から先の場合は、0 or 1
        if(now < N) return (now==i)?(double)1:(double)0;
        if(dp[now-N][i] != -1) return dp[now-N][i];
        //↓ここから先は分別機に来た時
        ll exit1 = sms[now-N].u;
        ll exit2 = sms[now-N].v;
        double ret = 0;
        if(exit1 != -1) ret += dfs(exit1,i) * sorting_machine_probs[now-N][i];
        if(exit2 != -1) ret += dfs(exit2,i) * (1-sorting_machine_probs[now-N][i]);
        return ret;
    };

    double ret = 0;
    //ゴミiがちゃんとは運び込まれる確率
    rep(i,N){
        ret += (1-dfs(s, i));
    }

    return 1000000000 * ret / N;
}


bool check_no_intersection(const vector<Conveyor>& conveyors, const Conveyor& new_conv) {
    auto ccw = [](Point a, Point b, Point c) {
        ll area = (b.x - a.x)*(c.y - a.y) - (b.y - a.y)*(c.x - a.x);
        return area;
    };

    for (auto& c : conveyors) {
        auto A = c.from, B = c.to, C = new_conv.from, D = new_conv.to;

        if (A.x == C.x && A.y == C.y) continue;
        if (A.x == D.x && A.y == D.y) continue;
        if (B.x == C.x && B.y == C.y) continue;
        if (B.x == D.x && B.y == D.y) continue;

        if ((ccw(A, B, C) * ccw(A, B, D) <= 0) &&
            (ccw(C, D, A) * ccw(C, D, B) <= 0)) {
            return false;
        }
    }
    return true;
}

void build_random_connections(vector<Sorting_machine>& sms, vector<Trashbox>& tbs, ll s) {
    random_device rd;
    mt19937 rng(rd());

    vector<Conveyor> conveyors;
    set<ll> used;  // DFSで使ったノード
    set<ll> finished; // DFS完了ノード

    auto is_valid_conveyor = [&](const Conveyor& c) {
        return check_no_intersection(conveyors, c);
    };

    // 処理場の使用済みチェック
    set<ll> used_trashbox;

    // DFS 関数
    function<void(ll)> dfs = [&](ll u) {
        used.insert(u);
        Sorting_machine& cur = sms[u];

        vector<ll> candidates;
        rep(i, sms.size()) if (!used.count(i) && i != u) candidates.push_back(i);
        shuffle(all(candidates), rng);

        int connected = 0;

        for (ll v : candidates) {
            Point from = cur.pos;
            Point to = sms[v].pos;
            Conveyor c = {from, to};

            if (!is_valid_conveyor(c)) continue;

            // 接続
            if (connected == 0) cur.u = v;
            else cur.v = v;
            conveyors.push_back(c);

            dfs(v);
            connected++;
            if (connected == 2) break;
        }

        // 足りない出口は処理場に接続
        while (connected < 2) {
            vector<ll> tb_candidates;
            rep(i, tbs.size()) if (!used_trashbox.count(i)) tb_candidates.push_back(i);
            shuffle(all(tb_candidates), rng);

            bool connected_tb = false;
            for (ll tid : tb_candidates) {
                Point to = tbs[tid].pos;
                Conveyor c = {cur.pos, to};
                if (!is_valid_conveyor(c)) continue;

                if (connected == 0) cur.u = ~tid;
                else cur.v = ~tid;
                conveyors.push_back(c);
                used_trashbox.insert(tid);
                connected_tb = true;
                break;
            }
            if (!connected_tb) break;
            connected++;
        }

        finished.insert(u);
    };

    dfs(s); // first_node から開始
}


int main(){
    cin>>N>>M>>K;
    trash_boxes = vector<Trashbox>(N);
    sorting_machines = vector<Sorting_machine>(M);
    sorting_machine_probs = vvd(K, vd(N));
    rep(i,N) cin>>trash_boxes[i].pos.x>>trash_boxes[i].pos.y;
    rep(i,M) cin>>sorting_machines[i].pos.x>>sorting_machines[i].pos.y;
    rep(i,K) rep(j,N) cin>>sorting_machine_probs[i][j];

    //i番目の場所に何番目の装置を置くか？
    rep(i,N) trash_boxes[i].i = i;
    rep(i,M) sorting_machines[i].i = i%K;

    //最初の分別機 or 処理場を選択（今回は最初の分別機とする）
    ll first_position = N;

    //分別機の出口1と出口2の設定
    rep(i,M){
        sorting_machines[i].u = sorting_machines[i].v = -1;
    }
    build_random_connections(sorting_machines, trash_boxes, first_position);

    cout << get_score(sorting_machines, first_position) << endl;

    rep(i,N) cout<<trash_boxes[i].i<<" ";
    cout<<endl;
    cout<<first_position<<endl;
    rep(i,M){
        if(sorting_machines[i].i == -1) cout<<-1<<endl;
        else cout<<sorting_machines[i].i<<" "<<sorting_machines[i].u<<" "<<sorting_machines[i].v<<endl;
    }


    //とりあえず、スコアを計算できるようにしたい
    //dpみたいに、ゴールから逆算すればO(M)でスコアの計算はできそう。

    //うまく組み合わせて分別できないかを考える(勝ちパターンが作れないか？)

    //「常に確率小さい方に行く確率」は、5個くらい重ねれば3%程度

    // いったん何も考えずに山登りするか？

    
    return 0;
}