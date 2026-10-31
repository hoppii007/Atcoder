//============================================================================
// AHC072 A - Somersault Slimes  集めてから配る方式(cluster-first, route-second)の実験版
//----------------------------------------------------------------------------
// main.cpp とは別の実験。色ごとに運ぶのではなく、色を問わず近くのスライムを塔にまとめてから巣を巡って配る。
// (1) クラスタ分け(annealClusters)
//     全スライムを8匹以下のクラスタに分ける。クラスタの見込み手数 = 集める手数 + 配る手数:
//       集める手数 = クラスタのマスの最小全域木(床の距離)
//       配る手数   = ハブ(クラスタのどれかのマス)から、含まれる色の巣を全部巡る最短路(Held-Karp、色は8以下)
//     初期解は色ごとに近いもの8匹ずつ(色ごとに運ぶのと同じ形)。近傍は、近くのスライムのクラスタへ移す /
//     近くのスライムと入れ替える / 1匹だけのクラスタにする。CLUSTER_TL まで焼きなます。
// (2) 集める(gatherTree / execGather)
//     ハブを根、クラスタのマスを葉とする木を作り(main.cpp の buildTree と同じ要領、色を問わない)、帰りがけ順に合流させる。
//     ハブの塔は合計8匹まで。巣のマスには着地しない。ハブは配る手数の見込みが一番小さいマス。
// (3) 配る(deliver)
//     ハブの塔を「同じ色が続く連」の列とみなし、両端の連から1つずつ巣へ届ける。状態 = (残りの連の区間, 塔のマス, 向き)で
//     Dijkstra して手数最小の配り方を求める。塔ごと動くと1手ごとに上下が反転するので、何手で着くかの偶奇で上に来る端が決まる。
//       - 巣へ歩いて入る: 着いたときに上に来る連がその巣の色なら帰巣する
//       - 投げ込む: 巣から一直線上のマスで、一番上の連だけを下の仲間を踏み台にして巣へ跳ばす(飛距離 = 下の匹数+1 まで)
//     塔の移動は(マス, 手数の偶奇)のBFS。他のスライムがいるマスにも着地でき、そのときは下に残して跳ぶので遠くまで跳べる。
// (4) 取りこぼしは main.cpp と同じ finishRest で巣へ運ぶ。
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

//------------------------------ 解法 ------------------------------

const ll NG = -1;        //そのルートでは目標まで行けない
const ll COLLECT_R = 3;  //出発前に同色を回収しに行く距離の上限(調整ノブ)
const ll SIDE_R    = 2;  //帰路でまっすぐ拾いに行く距離の上限(調整ノブ)

//(i,j)の一番上から連続している色cのスライムの数
ll topRun(const State &s, ll i, ll j, ll c){
    const Tower &w = s.at(i,j);
    ll cnt = 0;
    while(cnt < w.h && w.at(w.h-1-cnt) == c) cnt++;
    return cnt;
}

//m匹運んで(ci,cj)にいるときに行ける範囲をBFSする。
//着地できないマス(高さ+m>8)は通れない。COLLECT_Rマスまでで打ち切る。
//firstDir[i][j] = そのマスへ向かうために最初に進むべき方向
void reachable(const State &s, ll ci, ll cj, ll m, vvll &dist, vvll &firstDir){
    dist.assign(F.n, vll(F.n,-1));
    firstDir.assign(F.n, vll(F.n,-1));
    dist[ci][cj] = 0;
    queue<pll> que;
    que.push({ci,cj});
    while(!que.empty()){
        auto [i,j] = que.front(); que.pop();
        if(dist[i][j] >= COLLECT_R) continue;
        rep(d,4){
            ll ni = i + DI[d], nj = j + DJ[d];
            if(!F.ok(ni,nj) || dist[ni][nj] != -1) continue;
            if(s.height(ni,nj) + m > MAX_H) continue;
            dist[ni][nj] = dist[i][j] + 1;
            firstDir[ni][nj] = (dist[i][j] == 0 ? d : firstDir[i][j]);
            que.push({ni,nj});
        }
    }
}

//巣に向かう前に、近くにある同色スライムをできるだけ回収する。
//一番近い同色(同距離なら回収数が多い方)へ1マスずつ寄っていき、
//満杯(8匹)になるか近くに同色がいなくなったら終わる。
void collectNearby(State &s, ll &ci, ll &cj, ll c){
    static vvll dist, firstDir;
    ll budget = MAX_H * COLLECT_R;   //念のため歩数に上限を付ける
    while(budget-- > 0){
        if((ll)s.ops.size() >= MAX_T || timer.now() > TL) return;
        ll m = topRun(s,ci,cj,c);
        if(m <= 0 || m >= MAX_H) return;        //持っていない/もう積めない
        reachable(s, ci, cj, m, dist, firstDir);
        ll bi = -1, bj = -1, bd = inf, bg = 0;
        rep(i,F.n) rep(j,F.n){
            if(dist[i][j] <= 0) continue;       //自分のマスと未到達は除く
            ll g = topRun(s,i,j,c);
            if(g == 0) continue;
            if(dist[i][j] < bd || (dist[i][j] == bd && g > bg)){
                bd = dist[i][j]; bg = g; bi = i; bj = j;
            }
        }
        if(bi < 0) return;                      //近くに同色がいない
        ll d = firstDir[bi][bj];
        ll h = s.height(ci,cj);
        if(!s.tryDo(Action(ci,cj,h-m,d,1))) return;
        ci += DI[d]; cj += DJ[d];
        if(F.nestColor[ci][cj] == c) return;    //自分の巣に着いたら帰巣済み
    }
}

//(ci,cj)からd方向へまっすぐtマス進んで戻る往復が成立するか調べる。
//成立し、かつ1匹以上拾えるなら、回収数をgsum、戻ったときの匹数をmendに入れてtrueを返す。
bool trySide(const State &s, ll ci, ll cj, ll m, ll c, ll d, ll t,
             ll &gsum, ll &mend){
    ll hv = s.height(ci,cj);
    ll mm = m;
    vll h(t), g(t);
    gsum = 0;
    rep(x,t){
        ll ni = ci + DI[d]*(x+1), nj = cj + DJ[d]*(x+1);
        if(!F.ok(ni,nj)) return false;
        h[x] = s.height(ni,nj);
        if(h[x] + mm > MAX_H) return false;          //往路: 跳び移れない
        g[x] = topRun(s,ni,nj,c);
        mm += g[x];
        gsum += g[x];
    }
    //復路: 通ってきたマスは同色を抜いたので高さは h[x]-g[x] になっている
    mrep(x,t-2,0) if(h[x] - g[x] + mm > MAX_H) return false;
    if((hv - m) + mm > MAX_H) return false;          //出発マスに戻れない
    mend = mm;
    return gsum > 0;
}

//(ci,cj)にm匹で立っているとき、巣から遠い側へ「まっすぐSIDE_Rマス以内」にいる
//同色を、往復して回収する。回収できた分だけmを増やす。
//斜めは狙わない(例えば右上は、右に動いたあとの上として拾える)。
//巣に近づく側はルート探索(val/nxt)側が扱うので、ここでは触らない。
void sidePick(State &s, ll ci, ll cj, ll &m, ll c,
              const vvll &dist, const vector<vvll> &nxt){
    while(true){
        if(m >= MAX_H) return;                       //もう積めない
        ll bd = -1, bt = 0, bg = 0;
        rep(d,4){
            ll i1 = ci + DI[d], j1 = cj + DJ[d];
            if(!F.ok(i1,j1)) continue;
            if(dist[i1][j1] <= dist[ci][cj]) continue;   //巣から遠い側だけ
            rep(t,1,SIDE_R+1){
                ll gsum, mend;
                if(!trySide(s,ci,cj,m,c,d,t,gsum,mend)) continue;
                if((ll)s.ops.size() + 2*t > MAX_T) continue;
                if(nxt[mend][ci][cj] < 0) continue;      //拾うと巣へ進めなくなる
                //拾える数が多い方、同数なら手数が少ない方
                if(gsum > bg){ bg = gsum; bd = d; bt = t; }
            }
        }
        if(bd < 0) return;
        if(timer.now() > TL) return;

        ll pi = ci, pj = cj, mm = m;
        rep(x,bt){                                   //往路
            ll ni = pi + DI[bd], nj = pj + DJ[bd];
            ll g = topRun(s,ni,nj,c);
            ll h = s.height(pi,pj);
            if(h < mm || !s.tryDo(Action(pi,pj,h-mm,bd,1))) return;
            pi = ni; pj = nj; mm += g;
        }
        rep(x,bt){                                   //復路
            ll h = s.height(pi,pj);
            if(h < mm || !s.tryDo(Action(pi,pj,h-mm,bd^1,1))) return;
            pi += DI[bd^1]; pj += DJ[bd^1];
        }
        m = mm;
    }
}

//色cの一団を目標(gi,gj)まで運ぶときのルートDP。
//dist  : 目標からのBFS距離(-1は到達不可)。1マスずつ近づく道だけを使う。
//allow : 通ってよいマス(巣へ帰すときは床全体、ブロック内集約ではそのブロックのみ)
//gain[i][j]   : そのマスで回収できる数(一番上から連続する色cの数)
//val[m][i][j] : m匹運んでそのマスにいるとき、以降で追加回収できる最大数
//nxt[m][i][j] : そのとき進む方向
void buildRoute(const State &s, ll c, ll gi, ll gj,
                const vvll &dist, const vvb &allow,
                vvll &gain, vector<vvll> &val, vector<vvll> &nxt){
    gain.assign(F.n, vll(F.n, 0));
    rep(i,F.n) rep(j,F.n) if(allow[i][j]) gain[i][j] = topRun(s,i,j,c);

    val.assign(MAX_H+1, vvll(F.n, vll(F.n, NG)));
    nxt.assign(MAX_H+1, vvll(F.n, vll(F.n, -1)));

    vector<vpll> byDist(F.n*F.n + 1);
    rep(i,F.n) rep(j,F.n){
        if(allow[i][j] && dist[i][j] >= 0) byDist[dist[i][j]].push_back({i,j});
    }
    rep(m,1,MAX_H+1) val[m][gi][gj] = 0;  //目標に着いたら終わり
    rep(D,1,F.n*F.n+1){
        for(auto [i,j] : byDist[D]){
            rep(m,1,MAX_H+1){
                ll best = NG, bd = -1;
                rep(d,4){
                    ll ni = i + DI[d], nj = j + DJ[d];
                    if(!F.ok(ni,nj) || !allow[ni][nj] || dist[ni][nj] != D-1) continue;
                    if(s.height(ni,nj) + m > MAX_H) continue;   //着地できない
                    ll mu = m + gain[ni][nj];                   //着地後に持つ匹数
                    if(mu > MAX_H || val[mu][ni][nj] == NG) continue;
                    if(chmax(best, gain[ni][nj] + val[mu][ni][nj])) bd = d;
                }
                val[m][i][j] = best;
                nxt[m][i][j] = bd;
            }
        }
    }
}

//(i,j)から持ち上げる匹数を決める。回収総数が最大のものを選び、同数なら多く持つ方。
//(一番上の連続分を全部持つと高さ制限で進めない場合があるので少なくする余地を残す)
ll pickLift(ll gmax, ll i, ll j, const vector<vvll> &val){
    ll m0 = 0, bestTotal = -1;
    rep(m,1,gmax+1){
        if(val[m][i][j] == NG) continue;
        ll total = m + val[m][i][j];
        if(total > bestTotal || (total == bestTotal && m > m0)){ bestTotal = total; m0 = m; }
    }
    return m0;
}

//(i,j)から目標より遠い側へまっすぐSIDE_Rマス以内に、一番上が色cのマスがあるか
//(sidePickが拾いに行く候補。sidePickと同じく目標から遠ざかる方向だけを見る)
bool hasSideTarget(const State &s, ll i, ll j, ll c, const vvll &dist){
    rep(d,4){
        ll i1 = i + DI[d], j1 = j + DJ[d];
        if(!F.ok(i1,j1) || dist[i1][j1] <= dist[i][j]) continue;
        rep(t,1,SIDE_R+1){
            ll ni = i + DI[d]*t, nj = j + DJ[d]*t;
            if(!F.ok(ni,nj)) break;
            if(topRun(s,ni,nj,c) > 0) return true;
        }
    }
    return false;
}

//(ci,cj)からルート(nxt)に沿ってm匹をd方向へ運ぶとき、1手で何マス先まで跳べるかを返す(最低1)。
//下に残るk匹を踏み台にすると飛距離をk+1まで伸ばせるので、ルートがまっすぐ続く限り、
//「回収するものが無いマス」を飛び越える。次のマスでは止まる(飛び越えない):
//  目標のマス / 一番上が色cのマス(回収できる) / ルートが曲がるマス /
//  useSideのとき横に拾える同色がいるマス
//ルートDPの経路をそのままなぞるので、着地点の高さ制限はDPで確認済み。
ll routeJump(const State &s, ll ci, ll cj, ll m, ll c, ll d,
             const vvll &dist, const vector<vvll> &nxt, bool useSide){
    ll k = s.height(ci,cj) - m;                      //踏み台になる匹数
    ll lmax = min(k + 1, F.runLen[d][ci][cj]);
    ll l = 1;
    ll pi = ci + DI[d], pj = cj + DJ[d];             //いま飛び越えるか検討しているマス
    while(l < lmax){
        if(dist[pi][pj] == 0) break;
        if(topRun(s,pi,pj,c) > 0) break;
        if(nxt[m][pi][pj] != d) break;
        if(useSide && hasSideTarget(s,pi,pj,c,dist)) break;
        pi += DI[d]; pj += DJ[d];
        l++;
    }
    return l;
}

//決めたルート(nxt)に沿って、m匹の色cを目標(gi,gj)まで運ぶ。
//回収するものが無いマスは、下の仲間を踏み台にして飛び越える(routeJump)。
//useSideがtrueなら、進む前に横にいる同色も拾う。1手でも動かせたらtrue。
bool carryAlong(State &s, ll &ci, ll &cj, ll &m, ll c, ll gi, ll gj,
                const vvll &dist, const vector<vvll> &nxt, bool useSide){
    bool moved = false;
    while(ci != gi || cj != gj){
        if((ll)s.ops.size() >= MAX_T) break;
        if(timer.now() > TL) break;
        if(useSide) sidePick(s, ci, cj, m, c, dist, nxt);
        ll d = nxt[m][ci][cj];
        if(d < 0) break;
        ll h = s.height(ci,cj);
        if(h < m) break;
        ll l = routeJump(s, ci, cj, m, c, d, dist, nxt, useSide);
        if(!s.tryDo(Action(ci,cj,h-m,d,l))) break;
        moved = true;
        ci += DI[d]*l; cj += DJ[d]*l;
        m = topRun(s,ci,cj,c);          //着地点の同色を回収した分だけ増える
        if(m <= 0 || m > MAX_H) break;
    }
    return moved;
}

//(si,sj)の一番上にいるスライムを、同色を回収しながら自分の巣まで運ぶ。
//まず近くの同色を回収し、そのあと巣へ向かう。1マスでも動かせたらtrue。
bool carryTop(State &s, ll si, ll sj){
    if(s.height(si,sj) == 0) return false;
    ll c = s.at(si,sj).top();
    auto [gi,gj] = F.nestPos[c];
    if(si == gi && sj == gj) return false;  //帰巣済みのはずなので来ない

    //--- 前半: 近くの同色を回収する ---
    ll ci = si, cj = sj;
    ll before = (ll)s.ops.size();
    collectNearby(s, ci, cj, c);
    bool moved = (ll)s.ops.size() > before;
    if(ci == gi && cj == gj) return moved;          //回収中に巣へ着いた
    if(topRun(s,ci,cj,c) == 0) return moved;        //持っていたはずの一団が無い

    //--- 後半: 同色を回収しながら巣へ向かう ---
    const vvll &dist = F.nestDist[c];
    static vvll gain;
    static vector<vvll> val, nxt;
    buildRoute(s, c, gi, gj, dist, F.isFloor, gain, val, nxt);

    ll m0 = pickLift(gain[ci][cj], ci, cj, val);
    if(m0 == 0) return moved;  //巣まで行けるルートが無い

    //決めたルートに沿って、横の同色も拾いながら運ぶ
    ll m = m0;
    if(carryAlong(s, ci, cj, m, c, gi, gj, dist, nxt, true)) moved = true;
    return moved;
}

//仕上げ: 取りこぼしを、左上から順に一番上のスライムからcarryTopで巣へ帰していく
void finishRest(State &s){
    while(!s.done() && (ll)s.ops.size() < MAX_T && timer.now() <= TL){
        bool moved = false;
        rep(i,F.n) rep(j,F.n){
            while(s.height(i,j) > 0 && carryTop(s,i,j)) moved = true;
        }
        if(!moved) break;  //どのスライムも動かせない
    }
}


//============================================================================
// 集めてから配る
//============================================================================
const double CLUSTER_TL = 1200;   //クラスタ分けの焼きなましを終える時刻(ms)(調整ノブ)
const ll CLUSTER_CAP = 8;         //1クラスタの匹数の上限
const ll NEAR_NUM = 12;           //近傍で相手に選ぶ近いスライムの数(調整ノブ)
const double SA_T0 = 4.0, SA_T1 = 0.3;   //焼きなましの温度(調整ノブ)

vector<vector<short>> DD;   //DD[x][y] = 床を1マスずつ歩いたときの距離
vll nestIdx;                //色 -> 巣のidx
vll sCell, sCol;            //スライム番号 -> 初期マス, 色
vvll nearS;                 //スライム番号 -> 近いスライム(近い順)

void calcDD(){
    ll nn = F.n * F.n;
    DD.assign(nn, vector<short>(nn, 10000));
    rep(x,nn){
        if(!F.isFloor[x / F.n][x % F.n]) continue;
        vvll d = F.bfs({{x / F.n, x % F.n}});
        rep(y,nn) if(d[y / F.n][y % F.n] >= 0) DD[x][y] = (short)d[y / F.n][y % F.n];
    }
    nestIdx.assign(F.k, 0);
    rep(c,F.k) nestIdx[c] = F.idx(F.nestPos[c].fst, F.nestPos[c].snd);
    sCell.clear(); sCol.clear();
    rep(x,nn) if(F.initTower[x].h > 0){ sCell.pb(x); sCol.pb(F.initTower[x].top()); }
    ll S = sCell.size();
    nearS.assign(S, vll());
    rep(a,S){
        vll o;
        rep(b,S) if(b != a) o.pb(b);
        sort(all(o), [&](ll p, ll q){ return DD[sCell[a]][sCell[p]] < DD[sCell[a]][sCell[q]]; });
        if((ll)o.size() > NEAR_NUM) o.resize(NEAR_NUM);
        nearS[a] = o;
    }
}

//------------------------------ クラスタの見込み手数 ------------------------------
//クラスタのマスの最小全域木(Prim)
ll mstCost(const vll &mem){
    ll s = mem.size();
    if(s <= 1) return 0;
    ll best[CLUSTER_CAP]; bool used[CLUSTER_CAP];
    rep(i,s){ best[i] = inf; used[i] = false; }
    best[0] = 0;
    ll tot = 0;
    rep(_,s){
        ll u = -1;
        rep(i,s) if(!used[i] && (u < 0 || best[i] < best[u])) u = i;
        used[u] = true; tot += best[u];
        rep(i,s) if(!used[i]) chmin(best[i], (ll)DD[sCell[mem[u]]][sCell[mem[i]]]);
    }
    return tot;
}

//ハブから含まれる色の巣を全部巡る最短路(Held-Karp)。hubが-1ならクラスタのどのマスをハブにしてもよい
ll delivCost(const vll &mem, ll hub = -1){
    ll cs[CLUSTER_CAP], d = 0;
    ll seen = 0;
    for(ll x : mem) if(!(seen >> sCol[x] & 1)){ seen |= 1LL << sCol[x]; cs[d++] = sCol[x]; }
    if(d == 0) return 0;
    static ll g[1 << CLUSTER_CAP][CLUSTER_CAP];
    ll full = (1LL << d) - 1;
    rep(msk,full+1) rep(i,d) g[msk][i] = inf;
    rep(i,d){
        ll nest = nestIdx[cs[i]], b = inf;
        if(hub >= 0) b = DD[hub][nest];
        else for(ll x : mem) chmin(b, (ll)DD[sCell[x]][nest]);
        g[1LL << i][i] = b;
    }
    rep(msk,1,full+1) rep(i,d){
        if(g[msk][i] >= inf) continue;
        rep(j,d){
            if(msk >> j & 1) continue;
            chmin(g[msk | 1LL << j][j], g[msk][i] + DD[nestIdx[cs[i]]][nestIdx[cs[j]]]);
        }
    }
    ll ret = inf;
    rep(i,d) chmin(ret, g[full][i]);
    return ret;
}

ll clusterCost(const vll &mem){
    if(mem.empty()) return 0;
    return mstCost(mem) + delivCost(mem);
}

//------------------------------ (1) クラスタ分け ------------------------------
//初期解: 色ごとに、巣から一番遠いものと、それに近い同色を合わせて8匹ずつ
vector<vll> initClusters(){
    ll S = sCell.size();
    vector<vll> cl;
    vector<char> done(S,0);
    rep(c,F.k){
        while(true){
            ll far = -1;
            rep(a,S) if(!done[a] && sCol[a] == c && (far < 0 || DD[sCell[a]][nestIdx[c]] > DD[sCell[far]][nestIdx[c]])) far = a;
            if(far < 0) break;
            vll o;
            rep(a,S) if(!done[a] && sCol[a] == c) o.pb(a);
            sort(all(o), [&](ll p, ll q){ return DD[sCell[far]][sCell[p]] < DD[sCell[far]][sCell[q]]; });
            if((ll)o.size() > CLUSTER_CAP) o.resize(CLUSTER_CAP);
            for(ll a : o) done[a] = 1;
            cl.pb(o);
        }
    }
    return cl;
}

//クラスタ分けを焼きなます
vector<vll> annealClusters(double deadline){
    ll S = sCell.size();
    vector<vll> cl = initClusters();
    cl.resize(S);   //空のクラスタ(1匹だけのクラスタにする移動先)
    vll cid(S), cost(S, 0);
    rep(c,S){ for(ll a : cl[c]) cid[a] = c; cost[c] = clusterCost(cl[c]); }
    vll empt;
    rep(c,S) if(cl[c].empty()) empt.pb(c);

    auto removeFrom = [&](vll &v, ll a){ v.erase(find(all(v), a)); };
    double t0 = timer.now(), T = SA_T0;
    ll iter = 0;
#ifdef LOCAL
    ll init = 0; rep(c,S) init += cost[c];
#endif
    while(true){
        if((iter & 63) == 0){
            double now = timer.now();
            if(now > deadline) break;
            double p = (now - t0) / max(1.0, deadline - t0);
            T = SA_T0 * pow(SA_T1 / SA_T0, p);
        }
        iter++;
        ll a = rnd(S);
        if(nearS[a].empty()) break;
        ll b = nearS[a][rnd(nearS[a].size())];
        ll A = cid[a], B = cid[b];
        double r = rnd.prob();
        if(r < 0.45){
            //aをbのクラスタへ移す
            if(A == B || (ll)cl[B].size() >= CLUSTER_CAP) continue;
            vll na = cl[A], nb = cl[B];
            removeFrom(na, a); nb.pb(a);
            ll ca = clusterCost(na), cb = clusterCost(nb);
            ll delta = ca + cb - cost[A] - cost[B];
            if(delta <= 0 || rnd.prob() < exp(-delta / T)){
                cl[A] = na; cl[B] = nb; cost[A] = ca; cost[B] = cb; cid[a] = B;
                if(cl[A].empty()) empt.pb(A);
            }
        }else if(r < 0.9){
            //aとbを入れ替える
            if(A == B) continue;
            vll na = cl[A], nb = cl[B];
            removeFrom(na, a); removeFrom(nb, b); na.pb(b); nb.pb(a);
            ll ca = clusterCost(na), cb = clusterCost(nb);
            ll delta = ca + cb - cost[A] - cost[B];
            if(delta <= 0 || rnd.prob() < exp(-delta / T)){
                cl[A] = na; cl[B] = nb; cost[A] = ca; cost[B] = cb; cid[a] = B; cid[b] = A;
            }
        }else{
            //aを1匹だけのクラスタにする
            if(cl[A].size() <= 1) continue;
            while(!empt.empty() && !cl[empt.back()].empty()) empt.pop_back();
            if(empt.empty()) continue;
            ll E = empt.back();
            vll na = cl[A];
            removeFrom(na, a);
            ll ca = clusterCost(na), ce = clusterCost({a});
            ll delta = ca + ce - cost[A];
            if(delta <= 0 || rnd.prob() < exp(-delta / T)){
                empt.pop_back();
                cl[A] = na; cl[E] = {a}; cost[A] = ca; cost[E] = ce; cid[a] = E;
            }
        }
    }
    vector<vll> ret;
    rep(c,S) if(!cl[c].empty()) ret.pb(cl[c]);
#ifdef LOCAL
    ll fin = 0; rep(c,S) fin += cost[c];
    cerr << "CLUSTER iter=" << iter << " init=" << init << " final=" << fin << " clusters=" << ret.size() << "\n";
#endif
    return ret;
}

//------------------------------ (2) 集める ------------------------------
//ハブを根、クラスタのマスを葉とする木(色は問わない)
struct GTree {
    ll root = -1;
    vll par, parDir, parLen, load;
    vector<char> inTree;
    vvll child;
};

//main.cpp の buildTree と同じ要領で、木から一番近い葉を木へつなぐことを繰り返す。
//ハブの塔は合計8匹まで。どのノードでも base + load <= 8。クラスタに含まれる色(mask)の巣には着地しない
void gatherTree(const State &s, const vector<char> &isMem, ll mask, ll hub, GTree &T){
    ll nn = F.n * F.n;
    T.root = hub;
    T.par.assign(nn,-1); T.parDir.assign(nn,-1); T.parLen.assign(nn,0);
    T.load.assign(nn,0); T.inTree.assign(nn,0); T.child.assign(nn,vll());
    T.inTree[hub] = 1;

    vll g(nn,0), base(nn,0);
    rep(x,nn){
        if(!F.isFloor[x / F.n][x % F.n]) continue;
        g[x] = (isMem[x] && x != hub) ? 1 : 0;
        base[x] = s.t[x].h - g[x];
    }
    ll rootCap = MAX_H - s.t[hub].h;

    vll slack, dist, nxt(nn), ndir(nn), nlen(nn), src(nn), stk, que;
    while(true){
        slack.assign(nn,-1);
        slack[hub] = rootCap - T.load[hub];
        stk.assign(1,hub);
        while(!stk.empty()){
            ll u = stk.back(); stk.pop_back();
            for(ll ch : T.child[u]){
                slack[ch] = min(slack[u], MAX_H - base[ch] - T.load[ch]);
                stk.pb(ch);
            }
        }
        dist.assign(nn,-1);
        que.clear();
        rep(x,nn) if(T.inTree[x] && slack[x] >= 1){ dist[x] = 0; src[x] = x; que.pb(x); }
        ll found = -1;
        for(ll qh = 0; qh < (ll)que.size() && found < 0; qh++){
            ll u = que[qh];
            ll ui = u / F.n, uj = u % F.n;
            ll d0 = rnd(4);
            rep(t,4){
                ll d = (d0 + t) % 4;   //vからd方向へl跳ぶとuに着く
                ll lmax = min(MAX_H, F.runLen[d^1][ui][uj]), step = DI[d]*F.n + DJ[d];
                rep(l,1,lmax+1){
                    ll v = u - step*l;
                    if(l > base[v] + 1) continue;
                    if(dist[v] != -1 || T.inTree[v]) continue;
                    ll nc = F.nestColor[v / F.n][v % F.n];
                    if(nc >= 0 && (mask >> nc & 1)) continue;
                    if(g[v] == 0 && base[v] + 1 > MAX_H) continue;
                    dist[v] = dist[u] + 1; nxt[v] = u; ndir[v] = d; nlen[v] = l; src[v] = src[u];
                    if(g[v] > 0){
                        bool ok = g[v] <= slack[src[v]];
                        for(ll p = v; ok && !T.inTree[p]; p = nxt[p]) if(base[p] + g[v] > MAX_H) ok = false;
                        if(ok){ found = v; break; }
                        continue;
                    }
                    que.pb(v);
                }
                if(found >= 0) break;
            }
        }
        if(found < 0) break;

        ll w = g[found], p = found;
        while(!T.inTree[p]){
            T.inTree[p] = 1;
            T.par[p] = nxt[p]; T.parDir[p] = ndir[p]; T.parLen[p] = nlen[p];
            T.load[p] += w;
            T.child[nxt[p]].pb(p);
            p = nxt[p];
        }
        for(; p != hub; p = T.par[p]) T.load[p] += w;
        T.load[hub] += w;
        g[found] = 0;
    }
}

//帰りがけ順に合流させる。uから親へ跳んだ匹数(根ではuに集まった匹数)を返す
ll execGather(State &s, const GTree &T, const vector<char> &isMem, ll u){
    ll cnt = isMem[u] ? 1 : 0;
    for(ll ch : T.child[u]) cnt += execGather(s, T, isMem, ch);
    if(u == T.root || cnt == 0) return cnt;
    ll h = s.t[u].h;
    if(h < cnt) return 0;
    if(!s.tryDo(Action(u / F.n, u % F.n, h - cnt, T.parDir[u], T.parLen[u]))) return 0;
    return cnt;
}

//------------------------------ (3) 配る ------------------------------
//塔(m匹)の移動: (マス, 手数の偶奇) のBFS。gsは他のスライムの高さ(塔の下に残す匹数)。
//塔に含まれる色(mask)の巣へは着けるが、そこから先へは進まない(途中で帰巣させない)。それ以外の色の巣は通ってよい
struct Walk { vector<int> dist, prv; vector<Action> act; };

void walkBFS(const vll &gs, ll src, ll m, ll mask, Walk &W){
    ll nn = F.n * F.n;
    W.dist.assign(2*nn, -1); W.prv.assign(2*nn, -1); W.act.assign(2*nn, Action());
    vll que;
    W.dist[src*2] = 0; que.pb(src*2);
    for(ll qh = 0; qh < (ll)que.size(); qh++){
        ll id = que[qh], u = id / 2, p = id % 2;
        ll nc = F.nestColor[u / F.n][u % F.n];
        if(u != src && nc >= 0 && (mask >> nc & 1)) continue;
        ll ui = u / F.n, uj = u % F.n, g = gs[u];
        rep(d,4) rep(l,1,g+2){
            if(l > F.runLen[d][ui][uj]) break;
            ll v = F.idx(ui + DI[d]*l, uj + DJ[d]*l);
            if(gs[v] + m > MAX_H) continue;
            ll vid = v*2 + (p^1);
            if(W.dist[vid] != -1) continue;
            W.dist[vid] = W.dist[id] + 1; W.prv[vid] = id; W.act[vid] = Action(ui,uj,g,d,l);
            que.pb(vid);
        }
    }
}

//ハブにm匹の塔があるとき、巣を巡って配る。全員配れたらtrue
bool deliver(State &s, ll hub, ll m){
    ll nn = F.n * F.n;
    const Tower &tw = s.t[hub];
    if(tw.h < m || m <= 0) return false;
    //連(下から): 色と匹数
    vll rc, rs;
    rep(z,tw.h - m, tw.h){
        ll c = tw.at(z);
        if(!rc.empty() && rc.back() == c) rs.back()++;
        else { rc.pb(c); rs.pb(1); }
    }
    ll R = rc.size();
    ll mask = 0;
    for(ll c : rc) mask |= 1LL << c;
    vll gs(nn);
    rep(x,nn) gs[x] = s.t[x].h;
    gs[hub] -= m;

    //BFSはマスと匹数ごとにメモする
    map<ll, Walk> memo;
    auto walk = [&](ll src, ll mm) -> Walk& {
        ll key = src * 16 + mm;
        auto it = memo.find(key);
        if(it != memo.end()) return it->second;
        Walk &W = memo[key];
        walkBFS(gs, src, mm, mask, W);
        return W;
    };
    vll pre(R+1, 0);
    rep(r,R) pre[r+1] = pre[r] + rs[r];

    //状態 (lo, hi, マス, 向き)。向き0: 連が下からrc[lo..hi)の順、1: 逆順(rc[lo]が一番上)
    auto key = [&](ll lo, ll hi, ll x, ll o){ return ((lo * (R+1) + hi) * nn + x) * 2 + o; };
    ll NS = (R+1) * (R+1) * nn * 2;
    vll dist(NS, inf);
    struct Step { ll prev, type, target, par, dir, len, sz; };   //type 0: 歩いて巣へ入る, 1: 投げ込む
    vector<Step> how(NS);
    priority_queue<pll, vpll, greater<pll>> pq;
    ll st = key(0, R, hub, 0);
    dist[st] = 0; pq.push({0, st});
    ll goal = -1;
    while(!pq.empty()){
        auto [dcur, id] = pq.top(); pq.pop();
        if(dcur > dist[id]) continue;
        ll o = id % 2, x = id / 2 % nn, lh = id / 2 / nn, lo = lh / (R+1), hi = lh % (R+1);
        if(lo == hi){ goal = id; break; }
        ll mm = pre[hi] - pre[lo];
        Walk &W = walk(x, mm);
        rep(e,2){
            //e=0: いま一番上の連、e=1: いま一番下の連(奇数手で着けば上に来る)
            bool topIsHi = (o == 0);
            bool takeHi = (e == 0) ? topIsHi : !topIsHi;
            ll r = takeHi ? hi-1 : lo;
            ll nlo = takeHi ? lo : lo+1, nhi = takeHi ? hi-1 : hi;
            ll c = rc[r], sz = rs[r], nest = nestIdx[c];
            auto relax = [&](ll nx, ll cost, Step stp){
                ll nid = key(nlo, nhi, nx, o ^ stp.par);
                if(chmin(dist[nid], dcur + cost)){ how[nid] = stp; pq.push({dist[nid], nid}); }
            };
            //歩いて巣へ入る
            ll dw = W.dist[nest*2 + e];
            if(dw > 0) relax(nest, dw, {id, 0, nest, e, -1, -1, sz});
            //巣から一直線上のマスqから、一番上の連だけを投げ込む
            ll ni = nest / F.n, nj = nest % F.n;
            rep(d,4) rep(t,1,MAX_H+1){
                ll qi = ni - DI[d]*t, qj = nj - DJ[d]*t;
                if(!F.ok(qi,qj)) break;
                ll q = F.idx(qi,qj);
                if(F.nestColor[qi][qj] >= 0 && (mask >> F.nestColor[qi][qj] & 1)) continue;
                if(t > gs[q] + (mm - sz) + 1) continue;
                ll dq = W.dist[q*2 + e];
                if(dq < 0) continue;
                relax(q, dq + 1, {id, 1, q, e, d, t, sz});
            }
        }
    }
    if(goal < 0) return false;

    vector<Step> steps;
    for(ll id = goal; id != st; id = how[id].prev) steps.pb(how[id]);
    reverse(all(steps));
    ll cur = hub, mm = m;
    for(const Step &stp : steps){
        //塔ごと target(投げるならq、歩いて入るなら巣) まで歩く
        Walk &W = walk(cur, mm);
        vector<Action> path;
        for(ll v = stp.target*2 + stp.par; W.prv[v] >= 0; v = W.prv[v]) path.pb(W.act[v]);
        reverse(all(path));
        ll before = s.remain;
        for(const Action &a : path) if(!s.tryDo(a)) return false;
        if(stp.type == 1){
            ll q = stp.target;
            if(!s.tryDo(Action(q / F.n, q % F.n, s.t[q].h - stp.sz, stp.dir, stp.len))) return false;
        }
        if(before - s.remain != stp.sz) return false;
        mm -= stp.sz;
        cur = stp.target;
    }
    return true;
}

//------------------------------ 1クラスタを運ぶ ------------------------------
void runCluster(State &s, const vll &mem){
    ll nn = F.n * F.n;
    vector<char> isMem(nn, 0);
    ll mask = 0;
    for(ll a : mem){ isMem[sCell[a]] = 1; mask |= 1LL << sCol[a]; }
    //ハブ: 配る手数の見込みが一番小さいマス
    ll hub = -1, hb = inf;
    for(ll a : mem){
        ll x = sCell[a];
        if(s.t[x].h == 0) continue;
        if(chmin(hb, delivCost(mem, x))) hub = x;
    }
    if(hub < 0) return;
    static GTree T;
    gatherTree(s, isMem, mask, hub, T);
    ll m = execGather(s, T, isMem, hub);
    ll r0 = s.remain;
    bool ok = deliver(s, hub, m);
#ifdef DBG
    string cs; for(ll a : mem) cs += colChar(sCol[a]);
    cerr << "size=" << mem.size() << " cols=" << cs << " gathered=" << m << " deliv=" << (r0 - s.remain) << " ok=" << ok << "\n";
#endif
}

//============================================================================

int main(){
    timer.start();
    cin.tie(nullptr);
    ios::sync_with_stdio(false);

    F.read(cin);
    calcDD();
    vector<vll> cl = annealClusters(CLUSTER_TL);

    State s;
    s.init();
    for(const vll &mem : cl){
        if((ll)s.ops.size() >= MAX_T || timer.now() > TL) break;
        runCluster(s, mem);
    }
#ifdef LOCAL
    ll afterCluster = s.ops.size(), leftCluster = s.remain;
#endif
    finishRest(s);

#ifdef LOCAL
    cerr << "N=" << F.n << " K=" << F.k << " slime=" << F.slimeNum << "\n";
    cerr << "CLUSTER_DONE T=" << afterCluster << " left=" << leftCluster << "\n";
    cerr << "time=" << timer.now() << "ms T=" << s.ops.size() << " E=" << s.remain << "\n";
#endif
    s.print(cout);
    return 0;
}
