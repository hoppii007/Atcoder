//============================================================================
// AHC072 A - Somersault Slimes  解法の方針
//----------------------------------------------------------------------------
// (1) 集合計画(焼きなまし)
//     各スライムを「同色の集合地点(クラスタ, 8匹以下)」に割り当てる。
//     近似コスト = Σ(メンバー∪集合地点のMST長 + 集合地点から巣までの距離)
//     近傍: move(1匹を別クラスタへ) / swap(同色2匹を交換) /
//           shift(集合地点を動かす) / merge(2クラスタを統合)
// (2) デコーダ(クラスタごとに「寄せる → 巣へ運ぶ」を続けて行う。大きいクラスタから)
//     寄せる: 遠いメンバーから集合地点へ運び、集合地点から GATHER_R 以内の
//             「空き or 一番上が同色」のマスに着いたら止める(1本の塔にはしない)。
//             運ぶ道はルートDPで選び、道中の同色も拾う。
//     巣へ運ぶ: 寄せた一団を巣から遠いものから carryTop で運ぶ。近くの同色(collectNearby)・
//             道中の同色(ルートDP)・横の同色(sidePick)を回収しながら巣へ向かう。
// (3) 仕上げ
//     取りこぼしを、左上から順に一番上のスライムから carryTop で巣まで運ぶ。
//     ※焼きなましのコストモデルは「集合地点に1本の塔として、飛距離1で集める」前提のまま。
// 移動の工夫(routeJump): ルートに沿って運ぶとき、下に残る仲間k匹を踏み台にすれば
//     飛距離をk+1まで伸ばせる。ルートがまっすぐ続き、途中のマスで回収するもの
//     (一番上が同色 / sidePickで拾える横の同色)が無ければ、そのマスを飛び越える。
//     collectNearby と sidePick の移動は飛距離1のまま。
// ※旧方式のブロック集約(gatherBlocks)は比較用に残してある。
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
const ll BLOCK     = 5;  //事前集約で使う粗い正方形の一辺(調整ノブ)

//(i,j)の一番上から連続している色cのスライムの数
ll topRun(const State &s, ll i, ll j, ll c){
    const Tower &w = s.at(i,j);
    ll cnt = 0;
    while(cnt < w.h && w.at(w.h-1-cnt) == c) cnt++;
    return cnt;
}

//allowのマスだけを通る床上BFS距離(到達不可は-1)
vvll bfsMask(ll si, ll sj, const vvb &allow){
    vvll dist(F.n, vll(F.n,-1));
    if(!F.ok(si,sj) || !allow[si][sj]) return dist;
    dist[si][sj] = 0;
    queue<pll> que;
    que.push({si,sj});
    while(!que.empty()){
        auto [i,j] = que.front(); que.pop();
        rep(d,4){
            ll ni = i + DI[d], nj = j + DJ[d];
            if(!F.ok(ni,nj) || !allow[ni][nj] || dist[ni][nj] != -1) continue;
            dist[ni][nj] = dist[i][j] + 1;
            que.push({ni,nj});
        }
    }
    return dist;
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
//  useSideのとき横に拾える同色がいるマス / stopRが0以上のとき目標から距離stopR以内の空きマス
//ルートDPの経路をそのままなぞるので、着地点の高さ制限はDPで確認済み。
ll routeJump(const State &s, ll ci, ll cj, ll m, ll c, ll d,
             const vvll &dist, const vector<vvll> &nxt, bool useSide, ll stopR){
    ll k = s.height(ci,cj) - m;                      //踏み台になる匹数
    ll lmax = min(k + 1, F.runLen[d][ci][cj]);
    ll l = 1;
    ll pi = ci + DI[d], pj = cj + DJ[d];             //いま飛び越えるか検討しているマス
    while(l < lmax){
        if(dist[pi][pj] == 0) break;
        if(topRun(s,pi,pj,c) > 0) break;
        if(nxt[m][pi][pj] != d) break;
        if(useSide && hasSideTarget(s,pi,pj,c,dist)) break;
        if(stopR >= 0 && dist[pi][pj] <= stopR && s.height(pi,pj) == 0) break;
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
        ll l = routeJump(s, ci, cj, m, c, d, dist, nxt, useSide, -1);
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

//------------------------------ 事前集約(粗いブロック) ------------------------------

//allowで示したブロック内にいる色cを1か所に寄せる。
//寄せ先は「集約コスト + 巣までの距離」が最小のマス。遠いものから順に寄せ、
//道中の同色も拾っていく(ルートDPが拾える道を選ぶ)。
void gatherColorInBlock(State &s, const vvb &allow, ll c){
    //ブロック内で色cを一番上に持つマス
    vpll cells;
    rep(i,F.n) rep(j,F.n) if(allow[i][j] && topRun(s,i,j,c) > 0) cells.push_back({i,j});
    if(cells.size() < 2) return;

    //寄せ先の候補: 色cを持つマスと、ブロック内にあるなら色cの巣
    vpll cand = cells;
    auto [pi_, pj_] = F.nestPos[c];
    if(F.ok(pi_,pj_) && allow[pi_][pj_]) cand.push_back({pi_,pj_});

    ll bestScore = inf, gi = -1, gj = -1;
    vvll bestDist;
    for(auto [ai,aj] : cand){
        ll nd = F.nestDist[c][ai][aj];
        if(nd < 0) continue;                       //巣へ行けないマスに寄せても無駄
        vvll d = bfsMask(ai, aj, allow);
        ll sum = 0;
        for(auto [i,j] : cells){
            if(i == ai && j == aj) continue;
            //ブロック内で行けないマスは寄せられないので、その分は罰則として数える
            sum += (d[i][j] < 0 ? 2*F.n : d[i][j]);
        }
        ll score = sum + nd;                       //集約コスト + そこから巣までの距離
        if(score < bestScore){ bestScore = score; gi = ai; gj = aj; bestDist = d; }
    }
    if(gi < 0) return;

    static vvll gain;
    static vector<vvll> val, nxt;
    vvb skip(F.n, vb(F.n,false));                  //寄せられなかったマス
    while(true){
        if((ll)s.ops.size() >= MAX_T || timer.now() > TL) return;

        //寄せ先以外で色cを持つ、寄せ先から最も遠いマスを選ぶ
        ll si = -1, sj = -1, sd = -1;
        rep(i,F.n) rep(j,F.n){
            if(!allow[i][j] || skip[i][j] || bestDist[i][j] <= 0) continue;
            if(topRun(s,i,j,c) == 0) continue;
            if(bestDist[i][j] > sd){ sd = bestDist[i][j]; si = i; sj = j; }
        }
        if(si < 0) return;

        buildRoute(s, c, gi, gj, bestDist, allow, gain, val, nxt);
        ll m0 = pickLift(gain[si][sj], si, sj, val);
        if(m0 == 0){ skip[si][sj] = true; continue; }   //寄せ先まで行けない

        ll ci = si, cj = sj, m = m0;
        if(!carryAlong(s, ci, cj, m, c, gi, gj, bestDist, nxt, false)) skip[si][sj] = true;
    }
}

//グリッドを一辺BLOCKの粗い正方形に分割し、各ブロック内で同色をできるだけくっつけておく。
//ここで塊を作っておくと、巣へ運ぶ段階で1手あたりに運べる匹数が増える。
void gatherBlocks(State &s){
    rep(bi,0,F.n,BLOCK) rep(bj,0,F.n,BLOCK){
        if(timer.now() > TL) return;
        ll ei = min(F.n, bi+BLOCK), ej = min(F.n, bj+BLOCK);
        vvb allow(F.n, vb(F.n,false));
        rep(i,bi,ei) rep(j,bj,ej) if(F.isFloor[i][j]) allow[i][j] = true;

        //ブロック内の匹数が多い色から寄せる(良い寄せ先を先に取れるようにする)
        vpll order;   //(-匹数, 色)
        rep(c,F.k){
            ll cnt = 0;
            rep(i,bi,ei) rep(j,bj,ej) if(allow[i][j]) cnt += topRun(s,i,j,c);
            if(cnt >= 2) order.push_back({-cnt, c});
        }
        sort(all(order));
        for(auto [negCnt, c] : order) gatherColorInBlock(s, allow, c);
    }
}

//------------------------------ 集合計画(焼きなまし) ------------------------------
//各スライムを「同色の集合地点(クラスタ)」へ割り当てる計画を焼きなましで作り、
//デコーダ(gatherCluster)で操作列にする。
//近似コスト(1クラスタ) = メンバー∪集合地点のMST長(集める手数) + 集合地点から巣までの距離
//  ※デコーダが常に飛距離1で運ぶことを前提にしたモデル

const double PLAN_TL = 900;    //計画の焼きなましを打ち切る時刻(ms)(調整ノブ)
const double SA_T0   = 3.0;    //焼きなましの開始温度(調整ノブ)
const double SA_T1   = 0.1;    //焼きなましの終了温度(調整ノブ)
const ll NEAR_CAND   = 8;      //付け替え相手として見る近い同色スライムの数(調整ノブ)
const ll CL_CAP      = MAX_H;  //1クラスタの匹数上限(集合地点に1本の塔として積むため)

//床マス間の距離(全ペア)。cellDist[a][b] (a,bはidx、どちらかが壁なら-1)
vector<vector<int>> cellDist;
void calcCellDist(){
    ll nn = F.n * F.n;
    cellDist.assign(nn, vector<int>(nn, -1));
    rep(i,F.n) rep(j,F.n){
        if(!F.isFloor[i][j]) continue;
        vvll d = F.bfs({{i,j}});
        ll a = F.idx(i,j);
        rep(x,F.n) rep(y,F.n) cellDist[a][F.idx(x,y)] = (int)d[x][y];
    }
}

struct Cluster {
    ll c = -1;    //色
    ll p = -1;    //集合地点(idx)
    vll mem;      //メンバー(スライムの初期位置idx)
    ll cost = 0;  //近似コスト
};

struct Plan {
    vector<Cluster> cl;
    vll owner;        //owner[idx] = そのマスにいたスライムの所属クラスタ(スライムがいないマスは-1)
    vll pointOwner;   //pointOwner[idx] = そのマスを集合地点にしているクラスタ(巣は管理しない)
    vll freeIds;      //メンバーが0になった空きクラスタ
    ll total = 0;     //近似コストの総和
};

bool isNestCell(ll p){ return F.nestColor[p / F.n][p % F.n] >= 0; }

//メンバーmem ∪ {p} のMST長 + pから色cの巣までの距離
ll clusterCost(ll c, ll p, const vll &mem){
    if(mem.empty()) return 0;
    static ll nodes[CL_CAP + 1], best[CL_CAP + 1];
    static bool used[CL_CAP + 1];
    ll cnt = 0;
    nodes[cnt++] = p;
    for(ll x : mem) if(x != p) nodes[cnt++] = x;
    //Prim法(高々9点なので O(点数^2) で十分)
    rep(a,cnt){ best[a] = inf; used[a] = false; }
    best[0] = 0;
    ll mst = 0;
    rep(_,cnt){
        ll u = -1;
        rep(a,cnt) if(!used[a] && (u < 0 || best[a] < best[u])) u = a;
        used[u] = true;
        mst += best[u];
        rep(a,cnt) if(!used[a]) chmin(best[a], (ll)cellDist[nodes[u]][nodes[a]]);
    }
    return mst + F.nestDist[c][p / F.n][p % F.n];
}

//クラスタid(色c、メンバーmem)の集合地点をpにできるか
//  自分の巣 / 空き床 / 自分のメンバーのマス のいずれかで、他クラスタの集合地点と重ならないこと
//  (他色のスライムがいるマスに集めると、そのスライムを埋めてしまう)
bool pointOk(const Plan &pl, ll id, ll c, ll p, const vll &mem){
    ll i = p / F.n, j = p % F.n;
    if(!F.isFloor[i][j]) return false;
    ll nc = F.nestColor[i][j];
    if(nc == c) return true;
    if(nc >= 0) return false;
    if(pl.pointOwner[p] >= 0 && pl.pointOwner[p] != id) return false;
    if(F.initTower[p].h == 0) return true;
    return find(all(mem), p) != mem.end();
}

//クラスタidの集合地点を付け替える(pointOwnerも更新)
void setPoint(Plan &pl, ll id, ll p){
    Cluster &q = pl.cl[id];
    if(q.p >= 0 && !isNestCell(q.p) && pl.pointOwner[q.p] == id) pl.pointOwner[q.p] = -1;
    q.p = p;
    if(p >= 0 && !isNestCell(p)) pl.pointOwner[p] = id;
}

//メンバーが0になったクラスタを空きにする
void freeCluster(Plan &pl, ll id){
    setPoint(pl, id, -1);
    pl.cl[id].mem.clear();
    pl.cl[id].cost = 0;
    pl.freeIds.push_back(id);
}

//色c・集合地点p・メンバーmemのクラスタを新しく作る
ll newCluster(Plan &pl, ll c, ll p, const vll &mem, ll cost){
    ll id;
    if(!pl.freeIds.empty()){ id = pl.freeIds.back(); pl.freeIds.pop_back(); }
    else{ id = pl.cl.size(); pl.cl.push_back(Cluster()); }
    Cluster &q = pl.cl[id];
    q.c = c; q.p = -1; q.mem = mem; q.cost = cost;
    setPoint(pl, id, p);
    for(ll x : mem) pl.owner[x] = id;
    return id;
}

//vからxを1つ取り除いたものをoutに入れる
void eraseTo(const vll &v, ll x, vll &out){
    out.clear();
    for(ll y : v) if(y != x) out.push_back(y);
}

//焼きなましの受理判定
bool accept(ll delta, double temp){
    if(delta <= 0) return true;
    return rnd.prob() < exp(-(double)delta / temp);
}

//初期計画: 全スライムが単独クラスタで、集合地点は自分の位置
void initPlan(Plan &pl){
    ll nn = F.n * F.n;
    pl.cl.clear();
    pl.freeIds.clear();
    pl.owner.assign(nn, -1);
    pl.pointOwner.assign(nn, -1);
    pl.total = 0;
    rep(x,nn){
        if(F.initTower[x].h == 0) continue;
        ll c = F.initTower[x].top();
        vll mem = {x};
        ll cost = clusterCost(c, x, mem);
        newCluster(pl, c, x, mem, cost);
        pl.total += cost;
    }
}

//計画を焼きなましで改善する。近傍は4種類:
//  move  : 1匹を同色の別クラスタ(または新しい単独クラスタ)へ移す
//  swap  : 別クラスタにいる同色2匹を入れ替える
//  shift : 集合地点を 隣のマス / メンバーのマス / 巣 へ動かす
//  merge : 同色の2クラスタを1つにまとめる
void optimizePlan(Plan &pl){
    //スライム一覧と、各スライムに近い同色スライム
    vll slimes;
    rep(x,F.n*F.n) if(F.initTower[x].h > 0) slimes.push_back(x);
    vvll nearSame(F.n * F.n);
    for(ll x : slimes){
        vpll cand;   //(距離, idx)
        for(ll y : slimes){
            if(y == x || F.initTower[y].top() != F.initTower[x].top()) continue;
            cand.push_back({cellDist[x][y], y});
        }
        sort(all(cand));
        rep(a, min((ll)cand.size(), NEAR_CAND)) nearSame[x].push_back(cand[a].snd);
    }

    vll ma, mb;   //近傍で使う作業用のメンバー列
    double start = timer.now();
    double span = max(1.0, PLAN_TL - start);
    double temp = SA_T0;
    ll iter = 0;
    while(true){
        if((iter & 255) == 0){
            double now = timer.now();
            if(now > PLAN_TL) break;
            temp = SA_T0 * pow(SA_T1 / SA_T0, (now - start) / span);
        }
        iter++;

        ll x = slimes[rnd(slimes.size())];
        ll A = pl.owner[x];
        Cluster &a = pl.cl[A];
        ll kind = rnd(10);

        if(kind < 5){
            //--- move ---
            if(x == a.p && a.mem.size() > 1) continue;   //集合地点のメンバーは抜けられない
            if(rnd(4) == 0){
                //新しい単独クラスタとして独立させる(集合地点は自分のマス)
                if(a.mem.size() <= 1) continue;
                eraseTo(a.mem, x, ma);
                ll ca = clusterCost(a.c, a.p, ma);
                ll cn = clusterCost(a.c, x, {x});
                ll delta = ca + cn - a.cost;
                if(!accept(delta, temp)) continue;
                a.mem = ma; a.cost = ca;
                newCluster(pl, a.c, x, {x}, cn);
                pl.total += delta;
            }else{
                if(nearSame[x].empty()) continue;
                ll y = nearSame[x][rnd(nearSame[x].size())];
                ll B = pl.owner[y];
                if(B == A) continue;
                Cluster &b = pl.cl[B];
                if((ll)b.mem.size() >= CL_CAP) continue;
                eraseTo(a.mem, x, ma);
                mb = b.mem; mb.push_back(x);
                ll ca = clusterCost(a.c, a.p, ma);
                ll cb = clusterCost(b.c, b.p, mb);
                ll delta = ca + cb - a.cost - b.cost;
                if(!accept(delta, temp)) continue;
                b.mem = mb; b.cost = cb;
                pl.owner[x] = B;
                if(ma.empty()) freeCluster(pl, A);
                else{ a.mem = ma; a.cost = ca; }
                pl.total += delta;
            }
        }else if(kind < 7){
            //--- swap ---
            if(nearSame[x].empty()) continue;
            ll y = nearSame[x][rnd(nearSame[x].size())];
            ll B = pl.owner[y];
            if(B == A) continue;
            Cluster &b = pl.cl[B];
            if(x == a.p || y == b.p) continue;           //集合地点のメンバーは動かさない
            eraseTo(a.mem, x, ma); ma.push_back(y);
            eraseTo(b.mem, y, mb); mb.push_back(x);
            ll ca = clusterCost(a.c, a.p, ma);
            ll cb = clusterCost(b.c, b.p, mb);
            ll delta = ca + cb - a.cost - b.cost;
            if(!accept(delta, temp)) continue;
            a.mem = ma; a.cost = ca;
            b.mem = mb; b.cost = cb;
            pl.owner[x] = B; pl.owner[y] = A;
            pl.total += delta;
        }else if(kind < 9){
            //--- shift ---
            ll np = -1;
            ll r = rnd(3);
            if(r == 0){
                ll d = rnd(4);
                ll ni = a.p / F.n + DI[d], nj = a.p % F.n + DJ[d];
                if(!F.ok(ni,nj)) continue;
                np = F.idx(ni,nj);
            }else if(r == 1){
                np = a.mem[rnd(a.mem.size())];
            }else{
                np = F.idx(F.nestPos[a.c].fst, F.nestPos[a.c].snd);
            }
            if(np == a.p || !pointOk(pl, A, a.c, np, a.mem)) continue;
            ll ca = clusterCost(a.c, np, a.mem);
            ll delta = ca - a.cost;
            if(!accept(delta, temp)) continue;
            setPoint(pl, A, np);
            a.cost = ca;
            pl.total += delta;
        }else{
            //--- merge ---
            if(nearSame[x].empty()) continue;
            ll y = nearSame[x][rnd(nearSame[x].size())];
            ll B = pl.owner[y];
            if(B == A) continue;
            Cluster &b = pl.cl[B];
            if((ll)(a.mem.size() + b.mem.size()) > CL_CAP) continue;
            //Aの集合地点のままBを吸収する
            ma = a.mem; ma.insert(ma.end(), all(b.mem));
            ll ca = clusterCost(a.c, a.p, ma);
            ll delta = ca - a.cost - b.cost;
            if(!accept(delta, temp)) continue;
            for(ll z : b.mem) pl.owner[z] = A;
            a.mem = ma; a.cost = ca;
            freeCluster(pl, B);
            pl.total += delta;
        }
    }
#ifdef LOCAL
    cerr << "plan_iter=" << iter << "\n";
#endif
}

//------------------------------ デコーダ(計画 → 操作列) ------------------------------
//クラスタごとに「集合地点のまわりへゆるく寄せる → 巣へ向かいながら回収する」を続けて行う。

const ll GATHER_R = 2;   //集合地点からこの距離以内まで寄せたら止める(調整ノブ。0なら1本の塔に積む)

//m匹の色cを、ルート(nxt)に沿って目標(distの0のマス)へ運ぶ。
//回収するものが無いマスは、下の仲間を踏み台にして飛び越える(routeJump)。
//目標からの距離がR以下で「空き or 一番上が色c」のマスに着地したら止まる
//(他色のスライムの上で止まると、そのスライムを埋めてしまうため)。
//止まったマスを(ci,cj)に入れる。1手でも動かせたらtrue。
bool carryNear(State &s, ll &ci, ll &cj, ll m, ll c, ll R,
               const vvll &dist, const vector<vvll> &nxt){
    bool moved = false;
    while(dist[ci][cj] > 0){
        if((ll)s.ops.size() >= MAX_T || timer.now() > TL) break;
        ll d = nxt[m][ci][cj];
        if(d < 0) break;
        ll h = s.height(ci,cj);
        if(h < m) break;
        ll l = routeJump(s, ci, cj, m, c, d, dist, nxt, false, R);
        ll ni = ci + DI[d]*l, nj = cj + DJ[d]*l;
        bool canStop = dist[ni][nj] <= R && (s.height(ni,nj) == 0 || s.at(ni,nj).top() == c);
        if(!s.tryDo(Action(ci,cj,h-m,d,l))) break;
        moved = true;
        ci = ni; cj = nj;
        m = topRun(s,ci,cj,c);          //着地点の同色を回収した分だけ増える
        if(canStop) break;
        if(m <= 0 || m > MAX_H) break;
    }
    return moved;
}

//クラスタのメンバーを、集合地点から距離GATHER_R以内へゆるく寄せる。
//集合地点から遠いメンバーから順に、道中の同色も拾いながら運ぶ(ルートDPが拾える道を選ぶ)。
//寄せ終えた一団がいるマスをzoneに入れる。
void gatherCluster(State &s, const Cluster &q, vpll &zone){
    zone.clear();
    if(q.mem.empty()) return;
    ll c = q.c, gi = q.p / F.n, gj = q.p % F.n;
    vvll dist = F.bfs({{gi,gj}});
    static vvll gain;
    static vector<vvll> val, nxt;

    //最初から範囲内にいるメンバーは動かさない
    for(ll x : q.mem){
        ll i = x / F.n, j = x % F.n;
        if(dist[i][j] <= GATHER_R && topRun(s,i,j,c) > 0) zone.push_back({i,j});
    }

    vb skip(q.mem.size(), false);   //寄せられなかったメンバー
    while(true){
        if((ll)s.ops.size() >= MAX_T || timer.now() > TL) return;

        //まだ一番上に色cが残っている範囲外のメンバーのうち、集合地点から最も遠いもの
        ll bx = -1, bd = -1;
        rep(x, q.mem.size()){
            if(skip[x]) continue;
            ll i = q.mem[x] / F.n, j = q.mem[x] % F.n;
            if(dist[i][j] <= GATHER_R) continue;
            if(topRun(s,i,j,c) == 0) continue;
            if(dist[i][j] > bd){ bd = dist[i][j]; bx = x; }
        }
        if(bx < 0) return;
        ll si = q.mem[bx] / F.n, sj = q.mem[bx] % F.n;

        buildRoute(s, c, gi, gj, dist, F.isFloor, gain, val, nxt);
        ll m0 = pickLift(gain[si][sj], si, sj, val);
        if(m0 == 0){ skip[bx] = true; continue; }   //集合地点まで行けない

        ll ci = si, cj = sj;
        if(!carryNear(s, ci, cj, m0, c, GATHER_R, dist, nxt)){ skip[bx] = true; continue; }
        //途中で止まった場合も、一団がいるマスとして巣へ運ぶ対象にする
        if(topRun(s,ci,cj,c) > 0) zone.push_back({ci,cj});
    }
}

//zoneにいる色cの一団を、巣から遠いマスから順に巣へ運ぶ。
//carryTopが近くの同色(collectNearby)・道中の同色(ルートDP)・横の同色(sidePick)を
//回収しながら運ぶので、集合地点のまわりに寄せた同色はここでまとめて拾われる。
void deliverZone(State &s, ll c, vpll zone){
    sort(all(zone));
    zone.erase(unique(all(zone)), zone.end());
    sort(all(zone), [&](const pll &a, const pll &b){
        return F.nestDist[c][a.fst][a.snd] > F.nestDist[c][b.fst][b.snd];
    });
    for(auto [i,j] : zone){
        while(topRun(s,i,j,c) > 0 && carryTop(s,i,j)){
            if((ll)s.ops.size() >= MAX_T || timer.now() > TL) return;
        }
    }
}

//計画に従って、クラスタごとに「寄せる → 巣へ運ぶ」を行う。大きいクラスタから処理する。
void runPlan(State &s, const Plan &pl){
    vpll order;   //(-匹数, クラスタid)
    rep(id, pl.cl.size()) if(!pl.cl[id].mem.empty()) order.push_back({-(ll)pl.cl[id].mem.size(), id});
    sort(all(order));
    vpll zone;
    for(auto [negSize, id] : order){
        if((ll)s.ops.size() >= MAX_T || timer.now() > TL) return;
        gatherCluster(s, pl.cl[id], zone);
        deliverZone(s, pl.cl[id].c, zone);
    }
}

//============================================================================

int main(){
    timer.start();
    cin.tie(nullptr);
    ios::sync_with_stdio(false);

    F.read(cin);
    State s;
    s.init();

    //解法:
    //  (1) 各スライムの集合地点(クラスタ)を焼きなましで決める
    //  (2) クラスタごとに、集合地点のまわりへゆるく寄せ、巣へ向かいながら回収する
    //  (3) 取りこぼしを、左上から順に一番上のスライムから巣へ帰していく
    calcCellDist();
    Plan plan;
    initPlan(plan);
    ll initEst = plan.total;
    optimizePlan(plan);
    runPlan(s, plan);
    ll planT = s.ops.size();

    while(!s.done() && (ll)s.ops.size() < MAX_T && timer.now() <= TL){
        bool moved = false;
        rep(i,F.n) rep(j,F.n){
            while(s.height(i,j) > 0 && carryTop(s,i,j)) moved = true;
        }
        if(!moved) break;  //どのスライムも動かせない
    }
    

#ifdef LOCAL
    cerr << "N=" << F.n << " K=" << F.k << " slime=" << F.slimeNum
         << " colorNum=" << F.colorNum << "\n";
    cerr << s.dump();
    cerr << "actions=" << s.enumActions().size() << "\n";
    cerr << "time=" << timer.now() << "ms\n";
    //近似コストと実際の手数の対応を見るための行
    ll clusterNum = 0;
    for(const Cluster &q : plan.cl) if(!q.mem.empty()) clusterNum++;
    cerr << "PLAN init_est=" << initEst << " est=" << plan.total
         << " clusters=" << clusterNum << " plan_T=" << planT
         << " T=" << s.ops.size() << " E=" << s.remain << "\n";
#else
    (void)initEst; (void)planT;
#endif

    s.print(cout);
    return 0;
}
