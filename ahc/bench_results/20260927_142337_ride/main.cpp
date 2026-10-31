//============================================================================
// AHC072 A - Somersault Slimes  解法の方針
//----------------------------------------------------------------------------
// (1) 合流木方式(runTree)
//     色をランダムな順に1色ずつ処理する。処理中の色以外は動かさないので、各マスで
//     一団の下に残る匹数(base)は一定 → 飛距離の上限 base+1 と着地できる匹数 8-base が固定になる。
//     色cの巣を根、色cのスライムのマスを葉とする木を作る(buildTree): 木から逆向きBFSで
//     一番手数の少ない葉を探し、その道を木に加える(近似Steiner木、途中のマスでの合流もあり)。
//     各ノードに集まる匹数 load は base+load<=8 を守る(容量つき)。道は木のノードに着地しない。
//     帰りがけ順に、子の一団が全員集まってから親へ1手跳ぶ(execTree)。つなげなかった葉は
//     木を作り直して運ぶ。残りは (2) の仕上げで運ぶ。
//     便乗(planRides): 色cの木を作るたびに他の色bの木も作っておき、色cの一団の真下にいる色bを、
//     「跳ぶ先が色bの木でいまのマスの祖先」である間だけ一緒に運ぶ。ジャンプで上下が反転するので、
//     乗客が一団の端(奇数回跳んだ後は上、偶数回なら下)にいる形だけを使い、手数を増やさず降ろす:
//     下にいれば出発時に残す、上にいれば着地直後に乗客だけ色bの木の親へ跳ぶ。
//     乗客が一団の真ん中に挟まらないよう、途中のノードで乗客を運ぶ子の到着順(最初/最後)を指定する。
// (2) 仕上げ
//     取りこぼしを、左上から順に一番上のスライムから carryTop で巣まで運ぶ。
//     carryTop は近くの同色(collectNearby)・道中の同色(ルートDP)・横の同色(sidePick)を
//     回収しながら巣へ向かい、ルート上で回収するものが無いマスは routeJump で飛び越える。
// (3) 多試行
//     TRIAL_TL(=1000ms) まで (1)(2) を乱数(色の順・BFSの方向順)を変えて最初から独立に繰り返し、
//     スコアが同じか良ければ最良解を置き換える。
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

const double TRIAL_TL = 1000;   //試行を新しく始めるのをやめる時刻(ms)(調整ノブ)
//------------------------------ 合流木方式 ------------------------------
//色を1色ずつ処理する。処理中の色c以外のスライムは動かさないので、各マスの
//「色cの一団の下にいる匹数(base)」は色cを処理している間変わらない。
//色cの巣を根、色cのスライムがいるマスを葉とし、一団が着地するマスをノードとする木を作り、
//葉から順に親へ跳んで合流させながら巣へ運ぶ。1手の飛距離はそのマスのbase+1まで。

//色cを運ぶための木。ノードは一団が着地するマス(idx)
struct ColorTree {
    ll root = -1;
    vll par, parDir, parLen;  //親のidx, 親へ跳ぶ方向・飛距離
    vll load;                 //そのマスを通る(集まる)色cの匹数
    vll base;                 //木を作ったときの、一団の下に残る匹数
    vll term;                 //木を作ったときに、そのマスの一番上から続いていた色cの数(葉なら>0)
    vector<char> inTree;
    vvll child;
    vll tin, tout;            //帰りがけ順の番号(祖先判定用)

    //aがvの(v自身を除く)祖先か
    bool isAnc(ll a, ll v) const {
        return inTree[a] && inTree[v] && a != v && tin[a] < tin[v] && tout[v] < tout[a];
    }
    //根からDFSして tin/tout を振る
    void calcOrder(){
        ll nn = (ll)par.size(), cnt = 0;
        tin.assign(nn,-1); tout.assign(nn,-1);
        vpll st = {{root,0}};
        tin[root] = cnt++;
        while(!st.empty()){
            auto [u,it] = st.back();
            if(it < (ll)child[u].size()){
                st.back().snd++;
                ll v = child[u][it];
                tin[v] = cnt++;
                st.push_back({v,0});
            }else{
                tout[u] = cnt++;
                st.pop_back();
            }
        }
    }
};

//いまの盤面で色cの木を作る(近似Steiner木)。
//木から一番近い(手数の少ない)色cのマスを、木の途中のマスへつなぐことを繰り返す。
//どのノードでも base + load <= 8 を守る(一団が着地できる高さ)。
//木のノードには着地しない道だけを使う(待っている一団に意図せず合流しないように)。飛び越えるのはよい。
void buildTree(const State &s, ll c, ColorTree &T){
    ll nn = F.n * F.n;
    auto [gi,gj] = F.nestPos[c];
    ll root = F.idx(gi,gj);
    T.root = root;
    T.par.assign(nn,-1); T.parDir.assign(nn,-1); T.parLen.assign(nn,0);
    T.load.assign(nn,0); T.inTree.assign(nn,0); T.child.assign(nn,vll());
    T.inTree[root] = 1;

    vll g(nn,0), base(nn,0);   //g: 一番上から続く色cの数(0なら葉ではない)
    rep(x,nn){
        if(!F.isFloor[x / F.n][x % F.n]) continue;
        g[x] = topRun(s, x / F.n, x % F.n, c);
        base[x] = s.t[x].h - g[x];
    }
    g[root] = 0;
    T.base = base;
    T.term = g;

    static vll slack, dist, nxt, ndir, nlen, src, stk;
    while(true){
        //slack[x] = xへあと何匹集められるか(x から根の手前までの base+load の余裕の最小)
        slack.assign(nn,-1);
        slack[root] = inf;
        stk.assign(1,root);
        while(!stk.empty()){
            ll u = stk.back(); stk.pop_back();
            for(ll ch : T.child[u]){
                ll cap = MAX_H - base[ch] - T.load[ch];
                if(u == root) chmin(cap, MAX_H - base[root] - T.load[ch]);  //巣に着地するとき
                slack[ch] = min(slack[u], cap);
                stk.push_back(ch);
            }
        }
        //余裕のあるノードから逆向きにBFSして、一番近い葉を探す
        dist.assign(nn,-1); nxt.assign(nn,-1); ndir.assign(nn,-1); nlen.assign(nn,0); src.assign(nn,-1);
        queue<ll> que;
        rep(x,nn) if(T.inTree[x] && slack[x] >= 1){ dist[x] = 0; src[x] = x; que.push(x); }
        ll found = -1;
        while(!que.empty() && found < 0){
            ll u = que.front(); que.pop();
            ll ui = u / F.n, uj = u % F.n;
            ll d0 = rnd(4);
            rep(t,4){
                ll d = (d0 + t) % 4;   //vからd方向へl跳ぶとuに着く
                rep(l,1,MAX_H+1){
                    ll vi = ui - DI[d]*l, vj = uj - DJ[d]*l;
                    if(!F.ok(vi,vj)) break;
                    ll v = F.idx(vi,vj);
                    if(l > base[v] + 1) continue;          //飛距離が足りない
                    if(dist[v] != -1 || T.inTree[v]) continue;
                    if(g[v] == 0 && base[v] + 1 > MAX_H) continue;
                    dist[v] = dist[u] + 1; nxt[v] = u; ndir[v] = d; nlen[v] = l; src[v] = src[u];
                    if(g[v] > 0){
                        //葉が見つかった。余裕と道中の高さを確かめる
                        bool ok = g[v] <= slack[src[v]];
                        for(ll p = v; ok && !T.inTree[p]; p = nxt[p]){
                            if(base[p] + g[v] > MAX_H) ok = false;
                        }
                        if(ok){ found = v; break; }
                        continue;                          //葉からは先へ伸ばさない
                    }
                    que.push(v);
                }
                if(found >= 0) break;
            }
        }
        if(found < 0) break;

        //見つかった葉から木までの道をノードとして加える
        ll w = g[found];
        ll p = found;
        while(!T.inTree[p]){
            T.inTree[p] = 1;
            T.par[p] = nxt[p]; T.parDir[p] = ndir[p]; T.parLen[p] = nlen[p];
            T.load[p] += w;
            T.child[nxt[p]].push_back(p);
            p = nxt[p];
        }
        for(; p != root; p = T.par[p]) T.load[p] += w;
        g[found] = 0;
    }
    T.calcOrder();
}

//------------------------------ 便乗 ------------------------------
//色cの一団が木に沿って跳ぶとき、出発マスで一団の真下にいる別の色bのスライムを、
//「色bの木で、いまのマスから巣へ向かう道の上」を跳ぶ間だけ一緒に運ぶ(乗せる)。
//(跳ぶ先が色bの木で今のマスの祖先である間は乗せ続け、外れるマスで降ろす)
//ジャンプのたびに一団の上下が反転するので、乗ってからi回跳んだ時点で乗客は
//  i奇数: 一団の一番上 / i偶数: 一団の一番下
//にいる。降ろすのに手数を増やさないため、次の形だけを採用する:
//  途中のノード: i奇数なら乗客を運ぶ子が最後に着く、i偶数なら最初に着く(色cの葉でもない)
//               (乗客が一団の真ん中に挟まらないように)
//  降りるマスd : dが色bの巣でi奇数 → 着地で帰巣
//               i偶数 → dを出るとき一番下の乗客を残す(dの一番上に残る。dが色bの巣なら帰巣)
//               i奇数か、dが色cの巣 → 着地の直後に乗客だけ色bの木の親へ跳ぶ(色bに必要な1手。
//               下の一団が踏み台になる。色cの巣では、乗客が去ると色cが出発点で帰巣する)
//1つの一団が運ぶ乗客は同時に1組まで(乗り降りのマスを別の乗車と共有しない)。

struct RidePlan {
    vll boardCol, boardNum;   //uを出るとき、一団の真下から乗せる色と匹数(-1: なし)
    vll dropCol, dropNum;     //uを出るとき、一団の一番下に残す色と匹数
    vll offCol, offNum;       //uを出て着地した直後に、一番上から自分の木の親へ跳ばす色と匹数
    vll firstCh, lastCh;      //そのノードに最初/最後に着かせる子(-1: 指定なし)
};

//色cの木Tを動かすときの便乗を決める。OT[b]は色bの木(b != c)
void planRides(const State &s, ll c, const ColorTree &T, const vector<ColorTree> &OT, RidePlan &P){
    ll nn = F.n * F.n;
    P.boardCol.assign(nn,-1); P.boardNum.assign(nn,0);
    P.dropCol.assign(nn,-1);  P.dropNum.assign(nn,0);
    P.offCol.assign(nn,-1);   P.offNum.assign(nn,0);
    P.firstCh.assign(nn,-1);  P.lastCh.assign(nn,-1);
    static vector<char> used;
    used.assign(nn,0);

    vll cand;
    rep(x,nn) if(T.inTree[x] && x != T.root && T.base[x] >= 1 && T.term[x] == 0) cand.push_back(x);
    mrep(x,(ll)cand.size()-1,1) swap(cand[x], cand[rnd(x+1)]);

    for(ll u : cand){
        ll b = s.t[u].top();
        if(b == c) continue;
        const ColorTree &B = OT[b];
        if(!B.inTree[u]) continue;
        ll g = min(topRun(s, u / F.n, u % F.n, b), T.base[u]);

        //色bの道に沿っている間だけ乗る
        vll path = {u};
        while(path.back() != T.root && B.isAnc(T.par[path.back()], path.back())) path.push_back(T.par[path.back()]);
        ll t = (ll)path.size() - 1;
        if(t == 0) continue;
        ll d = path[t];

        bool ok = true;
        for(ll x : path) if(x != T.root && used[x]) ok = false;
        //乗客のぶん高くなっても着地できるか
        rep(x,1,t+1){
            ll v = path[x];
            if(v == T.root){
                if(T.load[path[x-1]] + g > MAX_H - T.base[v]) ok = false;
            }else if(T.base[v] + T.load[v] + g > MAX_H) ok = false;
        }
        if(!ok) continue;

        //降り方  0: dを出るとき一番下に残す / 1: 着地直後に跳ぶ / 2: 着地で帰巣
        ll mode;
        if(d == B.root)      mode = (t % 2 == 1) ? 2 : 0;
        else if(d == T.root) mode = 1;
        else                 mode = (t % 2 == 1) ? 1 : 0;
        if(mode == 1){
            ll q = B.par[d];
            if(q < 0 || T.inTree[q]) continue;   //跳んだ先に色cの一団が来ると高さの計画が狂う
        }

        //子の到着順の指定 (ノード, 子, 0:最初 1:最後)
        vector<tuple<ll,ll,ll>> cons;
        rep(x,1,t){
            if(x % 2 == 1) cons.push_back({path[x], path[x-1], 1});
            else           cons.push_back({path[x], path[x-1], 0});
        }
        if(mode == 0) cons.push_back({d, path[t-1], 0});
        for(auto [v,ch,w] : cons){
            ll cnt = (ll)T.child[v].size();
            if(w == 0){
                if(T.term[v] > 0) ok = false;                          //一番下に色cの葉がいる
                if(P.firstCh[v] >= 0 && P.firstCh[v] != ch) ok = false;
                if(P.lastCh[v] == ch && cnt > 1) ok = false;
            }else{
                if(P.lastCh[v] >= 0 && P.lastCh[v] != ch) ok = false;
                if(P.firstCh[v] == ch && cnt > 1) ok = false;
            }
        }
        if(!ok) continue;

        //採用
        P.boardCol[u] = b; P.boardNum[u] = g;
        if(mode == 0){ P.dropCol[d] = b; P.dropNum[d] = g; }
        if(mode == 1){ P.offCol[path[t-1]] = b; P.offNum[path[t-1]] = g; }
        for(auto [v,ch,w] : cons){
            if(w == 0) P.firstCh[v] = ch;
            else       P.lastCh[v] = ch;
        }
        for(ll x : path) if(x != T.root) used[x] = 1;
    }
}

//(i,j)の下からfrom番目からcnt匹がすべて色colか
bool runIs(const State &s, ll i, ll j, ll from, ll cnt, ll col){
    const Tower &w = s.at(i,j);
    if(from < 0 || from + cnt > w.h) return false;
    rep(x,cnt) if(w.at(from+x) != col) return false;
    return true;
}

//木を葉から順に(帰りがけ順に)動かす。uに子の一団が全員集まってから、uの一団を親へ跳ばす。
//一団は「木を作ったときのbaseより上の全部」。便乗の計画Pに従って乗せ降ろしする
//(盤面が計画どおりでなければ、その乗せ降ろしはしない)。
void execTree(State &s, ll c, const ColorTree &T, const RidePlan &P,
              const vector<ColorTree> &OT, ll u){
    //子の到着順: 指定された最初の子 → その他 → 指定された最後の子
    ll fc = P.firstCh[u], lc = P.lastCh[u];
    if(fc >= 0) execTree(s, c, T, P, OT, fc);
    for(ll ch : T.child[u]) if(ch != fc && ch != lc) execTree(s, c, T, P, OT, ch);
    if(lc >= 0 && lc != fc) execTree(s, c, T, P, OT, lc);

    if(u == T.root) return;
    if((ll)s.ops.size() >= MAX_T) return;
    ll i = u / F.n, j = u % F.n;
    ll h = s.height(i,j);
    ll k = T.base[u];
    if(h <= k) return;   //一団がいない
    if(P.boardCol[u] >= 0 && runIs(s, i, j, k - P.boardNum[u], P.boardNum[u], P.boardCol[u])){
        k -= P.boardNum[u];
    }
    if(P.dropCol[u] >= 0 && k + P.dropNum[u] < h && runIs(s, i, j, k, P.dropNum[u], P.dropCol[u])){
        k += P.dropNum[u];
    }
    if(!s.tryDo(Action(i,j,k,T.parDir[u],T.parLen[u]))) return;

    //着地直後に乗客だけ自分の木の親へ跳ぶ
    if(P.offCol[u] >= 0){
        ll p = T.par[u], pi = p / F.n, pj = p % F.n;
        ll b = P.offCol[u], g = P.offNum[u];
        const ColorTree &B = OT[b];
        ll hp = s.height(pi,pj);
        if(B.par[p] >= 0 && runIs(s, pi, pj, hp - g, g, b)){
            s.tryDo(Action(pi,pj,hp-g,B.parDir[p],B.parLen[p]));
        }
    }
}

//合流木方式: 色をランダムな順に1色ずつ、木を作って巣へ運ぶ。
//その色で木につなげなかったスライムは、木を作り直して運ぶ(動けなくなるまで)。
//木を作るたびに他の色の木も作り、その道に沿う区間だけ他の色を乗せて運ぶ(便乗)。
void runTree(State &s){
    vll order(F.k);
    iota(all(order), 0);
    mrep(x,F.k-1,1) swap(order[x], order[rnd(x+1)]);
    static ColorTree T;
    static vector<ColorTree> OT;
    static RidePlan P;
    OT.resize(F.k);
    bool progress = true;
    while(!s.done() && progress){
        progress = false;
        for(ll c : order){
            while(true){
                if((ll)s.ops.size() >= MAX_T || timer.now() > TL) return;
                ll before = (ll)s.ops.size();
                buildTree(s, c, T);
                rep(b,F.k) if(b != c) buildTree(s, b, OT[b]);
                planRides(s, c, T, OT, P);
                execTree(s, c, T, P, OT, T.root);
                if((ll)s.ops.size() == before) break;
                progress = true;
            }
        }
    }
}

//============================================================================

int main(){
    timer.start();
    cin.tie(nullptr);
    ios::sync_with_stdio(false);

    F.read(cin);

    //解法:
    //  (1) 合流木方式: 色を1色ずつ、巣を根とした木に沿って葉から合流させながら巣へ運ぶ
    //  (2) 取りこぼしを、左上から順に一番上のスライムから巣へ帰していく
    //  (1)(2)を乱数(色の順・BFSの方向順)を変えてTRIAL_TLまで独立に繰り返し、最も良い操作列を出力する
    State best;
    ll bestScore = inf, trials = 0;
    do{
        State s;
        s.init();
        runTree(s);
        finishRest(s);
        trials++;
        if(s.score() <= bestScore){ bestScore = s.score(); best = s; }
    }while(timer.now() < TRIAL_TL);

#ifdef LOCAL
    cerr << "N=" << F.n << " K=" << F.k << " slime=" << F.slimeNum
         << " colorNum=" << F.colorNum << "\n";
    cerr << best.dump();
    cerr << "time=" << timer.now() << "ms\n";
    //試行回数と最良の手数・残り
    cerr << "TRIALS trials=" << trials
         << " T=" << best.ops.size() << " E=" << best.remain << "\n";
#else
    (void)trials;
#endif

    best.print(cout);
    return 0;
}
