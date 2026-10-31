//============================================================================
// AHC072 A - Somersault Slimes  解法の方針
//----------------------------------------------------------------------------
// 2段構成: (1) スライム1匹ずつのルートを決める → (2) ルートを固定して、合体・分裂・移動の順をビームサーチで決める
// (1) ルート(rt::build)
//     各スライムに、出発マスから自分の巣までの床の道(1マスずつの辺の列)を1本ずつ決める。色は区別しない。
//     辺(マス,方向)を通るルートの数を cnt として、目的関数 J = ROUTE_MARG(=8) * Σ ceil(cnt/8) + ROUTE_LEN(=1) * Σ ルート長。
//     (同じ辺を通るスライムは8匹まで1手でまとめて運べる、という見込み)
//     1匹のルートを外して Dijkstra でつなぎ直す(辺の重み = cnt%8==0 なら ROUTE_MARG、+ROUTE_LEN)と J は増えないので、
//     これを全員に繰り返す(座標降下)。収束したら、ランダムな1匹の近くの ROUTE_KICK(=5) 匹を乱した重みでつなぎ直してから
//     座標降下し、J が悪化しなければ受理する(キック)。ROUTE_TL(=300ms) まで。
// (2) ルート固定のビームサーチ(rb::solve)
//     1手 = 1層。動かす一団の全員について、着地点が自分のルートのまだ通っていない先のマスであるときだけ合法とする
//     (ルートの途中を飛ばすのは可。戻る動きは無い → 各手で必ず誰かのルートが進むので必ず終わる)。
//     評価 = 残りのルートで Σ ceil(cnt/8)(cnt は各スライムのまだ通っていない辺で数え直す)。動いた一団の辺だけ差分計算。
//     幅1から始め、かかった時間から BEAM_TL(=1800ms) に収まる見込みの幅に広げて解き直し、最良を採る。
//============================================================================
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
const ll inf = 1300100100100100100LL;
// const ll inf = 1001001001LL;
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


//================================ AHC072 基盤 ================================

//ミリ秒で経過時間を取得
class TimeKeeper {
public:
    chrono::system_clock::time_point startPoint, endPoint;
    void start(){
        startPoint = chrono::system_clock::now();
    }
    double now(){
        endPoint = chrono::system_clock::now();
        double elapsed = chrono::duration_cast<std::chrono::milliseconds>(endPoint-startPoint).count();
        return elapsed;
    }
};
TimeKeeper timer;

//乱数(xorshift64)
struct Rand {
    uint64_t x = 88172645463325252ULL;
    uint64_t next(){ x ^= x<<7; x ^= x>>9; return x; }
    ll operator()(ll n){ return (ll)(next() % (uint64_t)n); }                //[0,n)
    ll operator()(ll a, ll b){ return a + (ll)(next() % (uint64_t)(b-a)); }  //[a,b)
    double prob(){ return (double)(next() >> 11) * 0x1.0p-53; }              //[0,1)
};
Rand rnd;

//方向 0:U 1:D 2:L 3:R
const ll DI[4]    = {-1,  1,  0,  0};
const ll DJ[4]    = { 0,  0, -1,  1};
const char DCH[4] = {'U','D','L','R'};
//方向文字から番号へ
ll dirOf(char c){
    rep(d,4) if(DCH[d] == c) return d;
    return -1;
}

//パラメータ調整(tune.py)用: -DTUNE でビルドしたときだけ、TUNABLE の定数を同名の環境変数で上書きできる。
//-DTUNE なしのビルド(提出)ではただの定数になる。
#ifdef TUNE
double tuneParam(const char *name, double def){
    const char *v = getenv(name);
    return v ? atof(v) : def;
}
#define TUNABLE(type, name, def) const type name = (type)tuneParam(#name, def)
#else
#define TUNABLE(type, name, def) const type name = def
#endif

const double TL  = 1900;    //制限時間(ms)
const ll MAX_H   = 8;       //塔の高さ上限
const ll MAX_T   = 100000;  //操作回数上限
const ll PENALTY = 100000;  //残り1匹あたりのスコアペナルティ

//色(0..11)を1文字にする(デバッグ表示用)
char colChar(ll c){ return c < 10 ? char('0'+c) : char('a'+c-10); }

//------------------------------ 塔 ------------------------------
//下から順に色を4bitずつ詰める。最大8匹なので32bitに収まる。
struct Tower {
    uint32_t bits = 0;  //下からidx番目の色 = (bits >> 4*idx) & 0xF
    ll h = 0;           //高さ

    ll at(ll idx) const { return (bits >> (4*idx)) & 0xFu; }  //下から0-indexed
    ll top() const { return at(h-1); }                        //一番上の色
    bool empty() const { return h == 0; }

    //一番上に1匹積む
    void push(ll c){
        bits |= (uint32_t)c << (4*h);
        h++;
    }
    //一番上の1匹を取り除く
    void pop(){
        h--;
        bits &= (uint32_t)((1ULL << (4*h)) - 1);
    }
    bool operator==(const Tower &o) const { return bits == o.bits && h == o.h; }
};

//------------------------------ 操作(宙返りジャンプ) ------------------------------
struct Action {
    ll i, j;  //出発点
    ll k;     //下に残す匹数
    ll d;     //方向(0..3)
    ll l;     //飛距離
    Action(){}
    Action(ll i_, ll j_, ll k_, ll d_, ll l_) : i(i_), j(j_), k(k_), d(d_), l(l_) {}
    //着地点
    ll ti() const { return i + DI[d]*l; }
    ll tj() const { return j + DJ[d]*l; }
};
std::ostream& operator<<(std::ostream& os, const Action &a){
    os << a.i << " " << a.j << " " << a.k << " " << DCH[a.d] << " " << a.l;
    return os;
}

//------------------------------ 盤面の静的情報 ------------------------------
struct Field {
    ll n = 0, k = 0;
    vs raw;                    //入力そのまま
    vvb isFloor;               //床か
    vvll nestColor;            //巣の色、なければ-1
    vpll nestPos;              //色 -> 巣の座標
    vvll runLen[4];            //(i,j)からd方向へ連続する床の数 = 飛距離の上限
    vector<Tower> initTower;   //初期の塔(idx = i*n+j)
    ll slimeNum = 0;           //初期スライム総数
    vll colorNum;              //色ごとの初期スライム数
    vector<vvll> nestDist;            //nestDist[c][i][j] = 色cの巣からの床上BFS距離(到達不可は-1)

    ll idx(ll i, ll j) const { return i*n + j; }
    bool in(ll i, ll j) const { return 0 <= i && i < n && 0 <= j && j < n; }
    bool ok(ll i, ll j) const { return in(i,j) && isFloor[i][j]; }

    //入力を読む
    void read(istream &is){
        is >> n >> k;
        raw.resize(n);
        rep(i,n) is >> raw[i];

        isFloor.assign(n, vb(n,false));
        nestColor.assign(n, vll(n,-1));
        nestPos.assign(k, {-1,-1});
        initTower.assign(n*n, Tower());
        colorNum.assign(k, 0);

        rep(i,n) rep(j,n){
            char c = raw[i][j];
            if(c == '#') continue;
            isFloor[i][j] = true;
            if(c == '.') continue;
            if('a' <= c && c <= 'l'){
                ll col = c - 'a';
                initTower[idx(i,j)].push(col);
                colorNum[col]++;
                slimeNum++;
            }else{
                ll col = c - 'A';
                nestColor[i][j] = col;
                nestPos[col] = {i,j};
            }
        }
        calcRunLen();
        calcNestDist();
    }

    //各マスから各方向への飛距離上限を前計算
    void calcRunLen(){
        rep(d,4) runLen[d].assign(n, vll(n,0));
        //進行方向の先から順に埋める
        rep(d,4){
            //Uは上から、Dは下から、Lは左から、Rは右から走査すると先が既に確定している
            rep(a,n) rep(b,n){
                ll i = (d==1 ? n-1-a : a);
                ll j = (d==3 ? n-1-b : b);
                ll ni = i + DI[d], nj = j + DJ[d];
                runLen[d][i][j] = ok(ni,nj) ? runLen[d][ni][nj] + 1 : 0;
            }
        }
    }

    //始点集合からの床上BFS距離(到達不可は-1)
    vvll bfs(const vpll &starts) const {
        vvll dist(n, vll(n,-1));
        queue<pll> que;
        for(auto [si,sj] : starts){
            if(!ok(si,sj)) continue;
            dist[si][sj] = 0;
            que.push({si,sj});
        }
        while(!que.empty()){
            auto [i,j] = que.front(); que.pop();
            rep(d,4){
                ll ni = i + DI[d], nj = j + DJ[d];
                if(!ok(ni,nj) || dist[ni][nj] != -1) continue;
                dist[ni][nj] = dist[i][j] + 1;
                que.push({ni,nj});
            }
        }
        return dist;
    }

    //各色の巣からの距離を前計算
    void calcNestDist(){
        nestDist.assign(k, vvll());
        rep(c,k) nestDist[c] = bfs({nestPos[c]});
    }
};
Field F;

//------------------------------ 盤面の可変状態 ------------------------------
struct State {
    vector<Tower> t;     //塔(idx = i*n+j)
    ll remain = 0;       //盤面に残っているスライム数
    vector<Action> ops;  //ここまでの操作列

    //undo用に操作前の2マスを保存しておく
    struct Undo {
        ll si, ti;
        Tower src, dst;
        ll remain;
    };
    vector<Undo> hist;

    void init(){
        t = F.initTower;
        remain = F.slimeNum;
        ops.clear();
        hist.clear();
    }

    Tower& at(ll i, ll j){ return t[F.idx(i,j)]; }
    const Tower& at(ll i, ll j) const { return t[F.idx(i,j)]; }
    ll height(ll i, ll j) const { return t[F.idx(i,j)].h; }

    //絶対スコア(小さいほど良い)
    ll score() const { return (ll)ops.size() + PENALTY * remain; }
    //全員帰巣したか
    bool done() const { return remain == 0; }

    //操作が合法か
    bool canDo(const Action &a) const {
        if((ll)ops.size() >= MAX_T) return false;
        if(!F.ok(a.i, a.j)) return false;
        ll h = t[F.idx(a.i,a.j)].h;
        if(h <= 0) return false;
        if(a.k < 0 || a.k >= h) return false;
        if(a.d < 0 || a.d >= 4) return false;
        if(a.l < 1 || a.l > a.k + 1) return false;
        if(a.l > F.runLen[a.d][a.i][a.j]) return false;  //途中が全て床か
        ll g = t[F.idx(a.ti(), a.tj())].h;
        if(g + (h - a.k) > MAX_H) return false;          //着地後の高さ(帰巣前で判定)
        return true;
    }

    //巣のマスなら上から同色が続く限り取り除く。取り除いた数を返す
    ll home(ll id){
        ll col = F.nestColor[id / F.n][id % F.n];
        if(col < 0) return 0;
        ll cnt = 0;
        while(t[id].h > 0 && t[id].top() == col){
            t[id].pop();
            cnt++;
        }
        remain -= cnt;
        return cnt;
    }

    //操作を適用する(合法性はチェックしない)
    void doAction(const Action &a){
        ll si = F.idx(a.i, a.j);
        ll ti = F.idx(a.ti(), a.tj());
        hist.push_back({si, ti, t[si], t[ti], remain});

        //出発点の上から順に取り出して着地点に積むと、自然に上下が反転する
        ll cnt = t[si].h - a.k;
        rep(_,cnt){
            ll col = t[si].top();
            t[si].pop();
            t[ti].push(col);
        }
        //出発点と着地点の両方で帰巣
        home(si);
        home(ti);
        ops.push_back(a);
    }

    //合法なら適用する
    bool tryDo(const Action &a){
        if(!canDo(a)) return false;
        doAction(a);
        return true;
    }

    //盤面だけのコピー(操作列とundo履歴は空)。試しに運んで手数の差を見るとき用
    State board() const {
        State r;
        r.t = t;
        r.remain = remain;
        return r;
    }

    //直前の操作を取り消す
    void undo(){
        Undo u = hist.back(); hist.pop_back();
        t[u.si] = u.src;
        t[u.ti] = u.dst;
        remain  = u.remain;
        ops.pop_back();
    }

    //現状から選べる全ての合法手
    vector<Action> enumActions() const {
        vector<Action> ret;
        rep(i,F.n) rep(j,F.n){
            ll h = t[F.idx(i,j)].h;
            if(h == 0) continue;
            rep(k,h){
                rep(d,4){
                    ll lmax = min(k+1, F.runLen[d][i][j]);
                    rep(l,1,lmax+1){
                        ll g = t[F.idx(i + DI[d]*l, j + DJ[d]*l)].h;
                        if(g + (h - k) > MAX_H) continue;
                        ret.push_back(Action(i,j,k,d,l));
                    }
                }
            }
        }
        return ret;
    }

    //操作列を出力
    void print(ostream &os) const {
        for(const Action &a : ops) os << a << "\n";
    }

    //デバッグ用: 高さマップと各塔の中身
    string dump() const {
        string ret = "";
        rep(i,F.n){
            rep(j,F.n){
                if(!F.isFloor[i][j]){ ret += '#'; continue; }
                ret += char('0' + t[F.idx(i,j)].h);
            }
            ret += '\n';
        }
        rep(i,F.n) rep(j,F.n){
            const Tower &w = t[F.idx(i,j)];
            if(w.h == 0) continue;
            ret += "(" + to_string(i) + "," + to_string(j) + ")";
            if(F.nestColor[i][j] >= 0) ret += "[nest" + to_string(F.nestColor[i][j]) + "]";
            ret += " ";
            rep(x,w.h) ret += colChar(w.at(x));  //下から上の順
            ret += '\n';
        }
        ret += "remain=" + to_string(remain) + " T=" + to_string(ops.size())
             + " score=" + to_string(score()) + '\n';
        return ret;
    }
};

//============================================================================
// (1) スライム1匹ずつのルートを決める
//============================================================================
TUNABLE(double, ROUTE_TL, 300);   //ルートの改善を打ち切る時刻(ms)(調整ノブ)
TUNABLE(ll, ROUTE_MARG, 8);       //辺の便(ceil(cnt/8))が1つ増える重み(調整ノブ)
TUNABLE(ll, ROUTE_LEN, 1);        //ルート長1マスあたりの重み(調整ノブ)
TUNABLE(ll, ROUTE_KICK, 5);       //キックでつなぎ直すスライム数(調整ノブ)

namespace rt {
int N, NN, M;
vector<int> col, start_cell, nest_pos;
vector<array<int, 4>> nb;          //nb[x][d] = xからd方向の隣の床(なければ-1)
vector<int> cnt;                   //cnt[x*4+d] = その辺を通るルートの数
vector<vector<int>> route;         //route[s] = スライムsのルート(出発マス..巣)
ll trips = 0, total_len = 0;       //Σ ceil(cnt/8), Σ ルート長

inline int trips_of(int c) { return (c + 7) >> 3; }
inline ll objective() { return ROUTE_MARG * trips + ROUTE_LEN * total_len; }

//隣り合うマス x→y の辺番号
inline int edge_of(int x, int y) {
    int dif = y - x;
    int d = dif == -N ? 0 : dif == N ? 1 : dif == -1 ? 2 : 3;
    return x * 4 + d;
}

//ルートsを cnt に足す(sign=+1)/引く(sign=-1)
void add_route(int s, int sign) {
    const auto &r = route[s];
    for (size_t i = 0; i + 1 < r.size(); i++) {
        int e = edge_of(r[i], r[i + 1]);
        trips -= trips_of(cnt[e]);
        cnt[e] += sign;
        trips += trips_of(cnt[e]);
    }
    total_len += sign * (ll)(r.size() - 1);
}

//スライムsのルートを Dijkstra で引き直す(cnt から外した状態で呼ぶ)。
//share: 辺の重み = (cnt%8==0 ? ROUTE_MARG : 0) + ROUTE_LEN。false なら乱した重みで引く(キック用)
void dijkstra(int s, bool share) {
    static vector<ll> dist;
    static vector<int> prv;
    dist.assign(NN, inf);
    prv.assign(NN, -1);
    int src = start_cell[s], dst = nest_pos[col[s]];
    int ord[4] = {0, 1, 2, 3};
    for (int x = 3; x > 0; x--) swap(ord[x], ord[rnd(x + 1)]);
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<>> pq;
    dist[src] = 0;
    pq.push({0, src});
    while (!pq.empty()) {
        auto [dx, x] = pq.top(); pq.pop();
        if (dx != dist[x]) continue;
        if (x == dst) break;
        for (int q = 0; q < 4; q++) {
            int d = ord[q], y = nb[x][d];
            if (y < 0) continue;
            ll w = ROUTE_LEN + (share ? (cnt[x * 4 + d] % 8 == 0 ? ROUTE_MARG : 0) : rnd(ROUTE_MARG + 1));
            if (dist[y] > dx + w) { dist[y] = dx + w; prv[y] = x; pq.push({dist[y], y}); }
        }
    }
    auto &r = route[s];
    r.clear();
    for (int x = dst; x != -1; x = prv[x]) r.push_back(x);
    reverse(r.begin(), r.end());
}

//全員を1回ずつ引き直す。Jが減ったか
bool descent_pass() {
    ll j0 = objective();
    vector<int> ord(M);
    iota(ord.begin(), ord.end(), 0);
    for (int x = M - 1; x > 0; x--) swap(ord[x], ord[rnd(x + 1)]);
    for (int s : ord) { add_route(s, -1); dijkstra(s, true); add_route(s, +1); }
    return objective() < j0;
}

ll iters = 0, kicks = 0, accepted = 0;
void build(double deadline) {
    N = (int)F.n; NN = N * N;
    nest_pos.assign(F.k, -1);
    rep(c, F.k) nest_pos[c] = (int)F.idx(F.nestPos[c].fst, F.nestPos[c].snd);
    nb.assign(NN, {-1, -1, -1, -1});
    rep(i, F.n) rep(j, F.n) rep(d, 4) if (F.ok(i, j) && F.ok(i + DI[d], j + DJ[d])) nb[F.idx(i, j)][d] = (int)F.idx(i + DI[d], j + DJ[d]);
    col.clear(); start_cell.clear();
    rep(x, NN) if (F.initTower[x].h > 0) { col.push_back((int)F.initTower[x].top()); start_cell.push_back((int)x); }
    M = (int)col.size();
    cnt.assign(NN * 4, 0);
    route.assign(M, {});
    trips = total_len = 0;

    //巣から遠い順に、既にあるルートへ相乗りしながら引く
    vector<int> ord(M);
    iota(ord.begin(), ord.end(), 0);
    sort(ord.begin(), ord.end(), [&](int a, int b) {
        auto da = F.nestDist[col[a]][start_cell[a] / N][start_cell[a] % N];
        auto db = F.nestDist[col[b]][start_cell[b] / N][start_cell[b] % N];
        return da > db;
    });
    for (int s : ord) { dijkstra(s, true); add_route(s, +1); }
    while (descent_pass() && timer.now() < deadline) iters++;

    //キック: ランダムな1匹の出発マスに近い ROUTE_KICK 匹を乱した重みで引き直し、座標降下して悪化しなければ受理
    auto best = route;
    ll best_j = objective();
    while (timer.now() < deadline) {
        kicks++;
        int c = start_cell[rnd(M)];
        vector<pair<int, int>> near;
        for (int s = 0; s < M; s++) {
            int x = start_cell[s];
            near.push_back({abs(x / N - c / N) + abs(x % N - c % N), s});
        }
        int kk = (int)min<ll>(ROUTE_KICK, M);
        partial_sort(near.begin(), near.begin() + kk, near.end());
        for (int a = 0; a < kk; a++) { int s = near[a].snd; add_route(s, -1); dijkstra(s, false); add_route(s, +1); }
        while (descent_pass() && timer.now() < deadline) iters++;
        if (objective() <= best_j) { best = route; best_j = objective(); accepted++; }
        else {
            for (int s = 0; s < M; s++) add_route(s, -1);
            route = best;
            for (int s = 0; s < M; s++) add_route(s, +1);
        }
    }
}
}   //namespace rt

//============================================================================
// (2) ルートを固定して、合体・分裂・移動の順をビームサーチで決める
//============================================================================
TUNABLE(double, BEAM_TL, 1800);   //ビームサーチを打ち切る時刻(ms)(調整ノブ)
TUNABLE(ll, BEAM_WMAX, 3000);     //ビーム幅の上限(メモリ)(調整ノブ)

namespace rb {
constexpr int MAXC = 400, MAXS = 400;
int N, NN, M;
int shift_[4];
vector<int> col, nest_of_cell;
vector<array<int, 4>> run_len;
vector<vector<int>> redge;          //redge[s][i] = ルートのi番目からi+1番目へ進む辺
vector<vector<short>> ridx;         //ridx[s][x] = マスxがルートの何番目か(なければ-1)
vector<vector<uint32_t>> okm;       //okm[s][i] = ルートのi番目にいるとき跳べる(方向,飛距離)。bit d*8+(l-1)

struct RState {
    uint16_t stk[MAXC][8];   //塔のスライム番号(0段目が一番下)
    uint8_t h[MAXC];
    uint16_t at[MAXS];       //スライムがルートの何番目にいるか
    uint16_t cnt[MAXC * 4];  //まだ通っていない辺を通るスライムの数
    int cost, remain;        //cost = Σ ceil(cnt/8)
    uint64_t hash;
};

inline uint64_t hz(int s, int x, int z) {
    uint64_t v = ((uint64_t)s << 12 | (uint64_t)x << 3 | (uint64_t)z) + 0x9e3779b97f4a7c15ULL;
    v = (v ^ (v >> 30)) * 0xbf58476d1ce4e5b9ULL;
    v = (v ^ (v >> 27)) * 0x94d049bb133111ebULL;
    return v ^ (v >> 31);
}
inline int trips_of(int c) { return (c + 7) >> 3; }

//巣のマスxで、上から同色が続く限り帰巣させる
inline void go_home(RState &s, int x) {
    int c = nest_of_cell[x];
    if (c < 0) return;
    while (s.h[x] > 0 && col[s.stk[x][s.h[x] - 1]] == c) {
        int z = --s.h[x];
        s.hash ^= hz(s.stk[x][z], x, z);
        s.remain--;
    }
}

inline void apply(RState &s, int u, int k, int d, int l) {
    int hu = s.h[u], m = hu - k, v = u + shift_[d] * l, g = s.h[v];
    for (int t = 0; t < m; t++) {
        int sl = s.stk[u][hu - 1 - t];
        s.stk[v][g + t] = (uint16_t)sl;
        s.hash ^= hz(sl, u, hu - 1 - t) ^ hz(sl, v, g + t);
        int b = ridx[sl][v];
        for (int i = s.at[sl]; i < b; i++) {
            int e = redge[sl][i];
            s.cost -= trips_of(s.cnt[e]);
            s.cnt[e]--;
            s.cost += trips_of(s.cnt[e]);
        }
        s.at[sl] = (uint16_t)b;
    }
    s.h[u] = (uint8_t)k;
    s.h[v] = (uint8_t)(g + m);
    go_home(s, u);
    go_home(s, v);
}

//手を指した後の cost, remain, hash を盤面を書き換えずに求める
int dec_[MAXC * 4], touched_[MAXC * 4];
inline void evaluate(const RState &s, int u, int k, int d, int l, int &cost, int &rem, uint64_t &hs) {
    int hu = s.h[u], m = hu - k, v = u + shift_[d] * l, g = s.h[v];
    hs = s.hash; rem = s.remain; cost = s.cost;
    int nt = 0;
    for (int t = 0; t < m; t++) {
        int sl = s.stk[u][hu - 1 - t];
        hs ^= hz(sl, u, hu - 1 - t) ^ hz(sl, v, g + t);
        int b = ridx[sl][v];
        for (int i = s.at[sl]; i < b; i++) {
            int e = redge[sl][i];
            if (!dec_[e]++) touched_[nt++] = e;
        }
    }
    for (int a = 0; a < nt; a++) {
        int e = touched_[a];
        cost += trips_of(s.cnt[e] - dec_[e]) - trips_of(s.cnt[e]);
        dec_[e] = 0;
    }
    int cv = nest_of_cell[v];
    if (cv >= 0) {
        for (int z = g + m - 1; z >= 0; z--) {
            int sl = z >= g ? s.stk[u][hu - 1 - (z - g)] : s.stk[v][z];
            if (col[sl] != cv) break;
            hs ^= hz(sl, v, z);
            rem--;
        }
    }
    int cu = nest_of_cell[u];
    if (cu >= 0) {
        for (int z = k - 1; z >= 0; z--) {
            int sl = s.stk[u][z];
            if (col[sl] != cu) break;
            hs ^= hz(sl, u, z);
            rem--;
        }
    }
}

void setup() {
    N = (int)F.n; NN = N * N; M = rt::M;
    rep(d, 4) shift_[d] = (int)(DI[d] * N + DJ[d]);
    col = rt::col;
    nest_of_cell.assign(NN, -1);
    rep(c, F.k) nest_of_cell[rt::nest_pos[c]] = (int)c;
    run_len.assign(NN, {});
    rep(x, NN) rep(d, 4) run_len[x][d] = (int)F.runLen[d][x / N][x % N];
    redge.assign(M, {}); ridx.assign(M, vector<short>(NN, -1)); okm.assign(M, {});
    for (int s = 0; s < M; s++) {
        const auto &r = rt::route[s];
        for (size_t i = 0; i < r.size(); i++) ridx[s][r[i]] = (short)i;
        for (size_t i = 0; i + 1 < r.size(); i++) redge[s].push_back(rt::edge_of(r[i], r[i + 1]));
        for (size_t i = 0; i < r.size(); i++) {
            int u = r[i];
            uint32_t m = 0;
            for (int d = 0; d < 4; d++) for (int l = 1; l <= min(8, run_len[u][d]); l++)
                if (ridx[s][u + shift_[d] * l] > (int)i) m |= 1u << (d * 8 + l - 1);
            okm[s].push_back(m);
        }
    }
}

struct Node { int parent; uint32_t mv; };
struct Cand { float key; int par; uint32_t mv; uint64_t hash; };

//幅Wのビームサーチ。deadlineを過ぎたらfalse(must_finishなら幅1に落としてhard_deadlineまで続ける)
bool beam(int W, double noise, double deadline, double hard_deadline, bool must_finish, const RState &init,
          vector<uint32_t> &out_moves, int &out_remain) {
    static mt19937_64 rng(1);
    uniform_real_distribution<double> ur(0, 1);
    vector<RState> cur{init}, nxt;
    vector<vector<Node>> back;
    vector<Cand> cands;
    unordered_set<uint64_t> seen;
    int goal = -1;
    while (goal < 0) {
        if (timer.now() > deadline) {
            if (!must_finish) return false;
            W = 1;
            if (timer.now() > hard_deadline) break;
        }
        cands.clear();
        for (int p = 0; p < (int)cur.size(); p++) {
            const RState &s = cur[p];
            for (int u = 0; u < NN; u++) {
                int hu = s.h[u];
                if (!hu) continue;
                uint32_t mask = ~0u;
                for (int k = hu - 1; k >= 0; k--) {
                    int sl = s.stk[u][k];
                    mask &= okm[sl][s.at[sl]];
                    if (!mask) break;
                    int m = hu - k;
                    for (uint32_t b = mask; b; b &= b - 1) {
                        int bit = __builtin_ctz(b), d = bit >> 3, l = (bit & 7) + 1;
                        if (l > k + 1) continue;
                        int v = u + shift_[d] * l;
                        if (s.h[v] + m > 8) continue;
                        int cost, rem; uint64_t hs;
                        evaluate(s, u, k, d, l, cost, rem, hs);
                        float key = rem == 0 ? -1e9f : (float)(cost + noise * ur(rng));
                        cands.push_back({key, p, (uint32_t)(u | k << 9 | d << 12 | l << 14), hs});
                    }
                }
            }
        }
        if (cands.empty()) break;
        int take = min((int)cands.size(), 3 * W);
        nth_element(cands.begin(), cands.begin() + take - 1, cands.end(), [](const Cand &a, const Cand &b) { return a.key < b.key; });
        sort(cands.begin(), cands.begin() + take, [](const Cand &a, const Cand &b) { return a.key < b.key; });
        nxt.clear();
        vector<Node> bk;
        seen.clear();
        for (int i = 0; i < take && (int)nxt.size() < W; i++) {
            auto &c = cands[i];
            if (!seen.insert(c.hash).second) continue;
            nxt.push_back(cur[c.par]);
            uint32_t x = c.mv;
            apply(nxt.back(), x & 511, x >> 9 & 7, x >> 12 & 3, x >> 14 & 15);
            bk.push_back({c.par, c.mv});
            if (nxt.back().remain == 0 && goal < 0) goal = (int)nxt.size() - 1;
        }
        swap(cur, nxt);
        back.push_back(move(bk));
        if (goal >= 0) break;
        if ((ll)back.size() >= MAX_T) break;
    }
    int id = goal >= 0 ? goal : 0;
    out_remain = cur[id].remain;
    out_moves.clear();
    for (int dd = (int)back.size() - 1; dd >= 0; dd--) { out_moves.push_back(back[dd][id].mv); id = back[dd][id].parent; }
    reverse(out_moves.begin(), out_moves.end());
    return goal >= 0;
}

//初期盤面からビームサーチで解く。幅1から始め、かかった時間から収まる見込みの幅に広げて解き直す。
//幅1の1回目は hard_deadline まで粘る。最良の手順(Action列)と残り匹数を返す
int last_w = 0;
pair<vector<Action>, ll> solve(double deadline, double hard_deadline) {
    static RState init;
    memset(&init, 0, sizeof(init));
    for (int s = 0; s < M; s++) {
        int x = rt::start_cell[s];
        init.stk[x][0] = (uint16_t)s; init.h[x] = 1;
        init.hash ^= hz(s, x, 0);
        for (int e : redge[s]) init.cnt[e]++;
    }
    for (int e = 0; e < NN * 4; e++) init.cost += trips_of(init.cnt[e]);
    init.remain = M;

    vector<uint32_t> best, mv;
    int best_remain = INT_MAX;
    int W = 1;
    while (true) {
        double t0 = timer.now();
        int rem;
        bool first = best.empty();
        bool ok = beam(W, 1.0, deadline, hard_deadline, first, init, mv, rem);
        if (ok || first) {
            ll sc = (ll)mv.size() + PENALTY * rem;
            ll bs = best.empty() ? inf : (ll)best.size() + PENALTY * best_remain;
            if (sc < bs) { best = mv; best_remain = rem; }
            last_w = W;
        }
        if (!ok) break;
        double used = max(1.0, timer.now() - t0), left = deadline - timer.now();
        int next_w = (int)min<double>(BEAM_WMAX, W * left / used * 0.7);   //時間は幅にほぼ比例する
        if (next_w <= W) break;
        W = next_w;
    }
    vector<Action> acts;
    for (auto x : best) {
        int u = x & 511, k = x >> 9 & 7, d = x >> 12 & 3, l = x >> 14 & 15;
        acts.push_back(Action(u / N, u % N, k, d, l));
    }
    return {acts, (ll)best_remain};
}
}   //namespace rb

//============================================================================

int main(){
    timer.start();
    cin.tie(nullptr);
    ios::sync_with_stdio(false);

    F.read(cin);

    //(1) ルートを決める → (2) ルートを固定して順番をビームサーチで決める
    rt::build(ROUTE_TL);
    double route_ms = timer.now();
    rb::setup();
    auto [acts, rem] = rb::solve(BEAM_TL, BEAM_TL + 50);

    //実際に指して確かめる
    State s;
    s.init();
    for (auto &a : acts) if (!s.tryDo(a)) break;
    s.print(cout);

#ifdef LOCAL
    cerr << "N=" << F.n << " K=" << F.k << " slime=" << F.slimeNum << "\n";
    cerr << "ROUTE J=" << rt::objective() << " trips=" << rt::trips << " len=" << rt::total_len
         << " iters=" << rt::iters << " kicks=" << rt::kicks << " accepted=" << rt::accepted << " time=" << route_ms << "ms\n";
    cerr << "BEAM W=" << rb::last_w << " T=" << s.ops.size() << " E=" << s.remain
         << " (beam T=" << acts.size() << " E=" << rem << ") time=" << timer.now() << "ms\n";
#else
    (void)route_ms; (void)rem;
#endif
    return 0;
}
