//============================================================================
// AHC072 A - Somersault Slimes  解法の方針
//----------------------------------------------------------------------------
// (1) 磁石方式(runMagnet): 各色の巣を重心(磁石)とみなし、その色を巣へ流し込む。
//     色ごと(スライムが多い色から)に、巣から最も遠い「一番上が同色」のマスを出発点とし、
//     巣に着くまで次を繰り返す:
//       吸収(absorbNear): いまのマスから床上の距離 MAG_R(=2) 以内に一番上が同色のマスが
//         あれば、それと合流する。「相手を呼ぶ」「こちらから行く」のうち
//         (合流の手数 + 合流後のマスから巣までの距離) が小さい方。踏み台があれば飛距離2も使う。
//         合流後の一団で巣まで行けない(遠回り込みでも道が無い)吸収は取り消して行わない。
//       前進(stepToNest): 巣への距離が減る方向へ1手。下に残る仲間を踏み台にできるなら、
//         「一番上が同色でなく、まわり距離MAG_R以内にも同色がいないマス」を飛び越える。
//         着地で同色を拾える方向を優先。
//       渋滞(detourStep): 巣へ近づく隣がどれも着地できないときは、遠回りも許すBFSで
//         巣への最短手順を探す。一団全員で行けなければ下の仲間を残して上だけを運ぶ
//         (残した仲間が踏み台になり、渋滞を飛び越えられる)。運ぶのは巣まで行ける最大の匹数。
//         止まった地点より巣に近いマスに着くまで、その手順に沿って進む。
// (2) 仕上げ
//     取りこぼしを、左上から順に一番上のスライムから carryTop で巣まで運ぶ。
//     carryTop は近くの同色(collectNearby)・道中の同色(ルートDP)・横の同色(sidePick)を
//     回収しながら巣へ向かい、ルート上で回収するものが無いマスは routeJump で飛び越える。
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

//------------------------------ 床マス間の距離 ------------------------------

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

//------------------------------ 磁石方式 ------------------------------
//各色の巣を重心(磁石)とみなし、その色のスライムを巣へ流し込む。
//巣から遠い一団から出発し、1歩ずつ巣へ近づく。その途中、いまいるマスから距離MAG_R以内に
//一番上が同色のマスがあれば、先にそれを吸収して一団を大きくしてから進む。

const ll MAG_R = 2;   //吸収する半径(床上の距離)(調整ノブ)

vvll nearCells;       //nearCells[idx] = そのマスから床上の距離1..MAG_Rの床マス
void calcNearCells(){
    ll nn = F.n * F.n;
    nearCells.assign(nn, vll());
    rep(a,nn) rep(b,nn){
        if(a == b) continue;
        ll d = cellDist[a][b];
        if(1 <= d && d <= MAG_R) nearCells[a].push_back(b);
    }
}

//(ai,aj)の一番上にいる色cの一団(全部)を、距離MAG_R以内の(bi,bj)へ動かす最短の手順。
//手順は(方向,飛距離)の列。下に残るスライムを踏み台にした飛距離2も使う。
//途中で一番上が色cのマスに着地したら、そこの同色も一緒に運ぶ。無理なら空を返す。
vpll mergeSteps(const State &s, ll ai, ll aj, ll bi, ll bj, ll c){
    auto [gi,gj] = F.nestPos[c];
    ll ha = s.height(ai,aj), mm = topRun(s,ai,aj,c);
    ll hb = s.height(bi,bj);
    vpll best;
    rep(d1,4){
        rep(l1,1,3){
            if(l1 > ha - mm + 1 || l1 > F.runLen[d1][ai][aj]) break;
            ll xi = ai + DI[d1]*l1, xj = aj + DJ[d1]*l1;
            ll hx = s.height(xi,xj);
            if(hx + mm > MAX_H) continue;
            if(xi == bi && xj == bj) return {{d1,l1}};    //1手で着く
            if(!best.empty()) continue;                    //2手の候補はもうある
            if(xi == gi && xj == gj) continue;             //途中で巣に入ると帰巣してしまう
            //2手目: xからbへまっすぐ
            ll mx = mm + topRun(s,xi,xj,c);                //着地後に運ぶ匹数
            ll kx = hx + mm - mx;                          //着地後の踏み台
            rep(d2,4){
                ll di = bi - xi, dj = bj - xj;
                ll l2 = abs(di) + abs(dj);
                if(l2 == 0 || (di != 0 && dj != 0)) continue;
                if(di != DI[d2]*l2 || dj != DJ[d2]*l2) continue;
                if(l2 > kx + 1 || l2 > F.runLen[d2][xi][xj]) continue;
                if(hb + mx > MAX_H) continue;
                best = {{d1,l1},{d2,l2}};
            }
        }
    }
    return best;
}

//手順stepsを実行する。一団は毎回「一番上から続く色cの全部」を運ぶ。
//最後にいたマスを(ci,cj)に入れる。全部実行できたらtrue。
bool runSteps(State &s, ll &ci, ll &cj, ll c, const vpll &steps){
    for(auto [d,l] : steps){
        ll m = topRun(s,ci,cj,c);
        if(m == 0) return false;
        if(!s.tryDo(Action(ci,cj,s.height(ci,cj)-m,d,l))) return false;
        ci += DI[d]*l; cj += DJ[d]*l;
    }
    return true;
}

//(i,j)から距離MAG_R以内に、一番上が色cのマスがあるか
bool hasNearSame(const State &s, ll i, ll j, ll c){
    for(ll q : nearCells[F.idx(i,j)]) if(topRun(s,q/F.n,q%F.n,c) > 0) return true;
    return false;
}

//(ci,cj)の一団(色c)を巣の方向へ1手進める。巣への距離が1ずつ減る道を通る。
//下に残るスライムを踏み台にできるなら、まっすぐ進む限り
//「一番上が色cでなく、まわり距離MAG_R以内にも色cがいないマス」を飛び越える。
//着地で同色を拾える方向を優先し、次に遠くまで跳べる方向を選ぶ。動けたらtrue。
bool stepToNest(State &s, ll &ci, ll &cj, ll c){
    const vvll &nd = F.nestDist[c];
    auto [gi,gj] = F.nestPos[c];
    ll h = s.height(ci,cj), m = topRun(s,ci,cj,c);
    ll k = h - m;
    ll bd = -1, bl = 0, bg = -1;
    rep(d,4){
        ll lmax = min(k + 1, F.runLen[d][ci][cj]);
        ll lbest = 0;
        rep(l,1,lmax+1){
            ll pi = ci + DI[d]*l, pj = cj + DJ[d]*l;
            if(nd[pi][pj] != nd[ci][cj] - l) break;        //巣へまっすぐ近づく道だけ
            if(s.height(pi,pj) + m <= MAX_H) lbest = l;    //着地できる
            //このマスで止まるべきなら、これより先へは跳ばない
            if(pi == gi && pj == gj) break;
            if(topRun(s,pi,pj,c) > 0) break;
            if(hasNearSame(s,pi,pj,c)) break;
        }
        if(lbest == 0) continue;
        ll ti = ci + DI[d]*lbest, tj = cj + DJ[d]*lbest;
        ll g = (topRun(s,ti,tj,c) > 0 || (ti == gi && tj == gj)) ? 1 : 0;
        if(g > bg || (g == bg && lbest > bl)){ bg = g; bl = lbest; bd = d; }
    }
    if(bd < 0) return false;
    if(!s.tryDo(Action(ci,cj,k,bd,bl))) return false;
    ci += DI[bd]*bl; cj += DJ[bd]*bl;
    return true;
}

//(ci,cj)の一番上のmv匹を、色cの巣まで運ぶ最短手順をBFSで探す(遠回りも許す)。
//出発点では下に残る(高さ-mv)匹を、それ以降は着地したマスにいたスライムを踏み台にして、
//踏み台の匹数+1マスまで跳べる。着地できるのは「高さ+mv<=8」のマスだけ。
//途中のマスの高さは今のまま変わらないとみなす。手順(方向,飛距離)を返す。無理なら空。
vpll detourRoute(const State &s, ll ci, ll cj, ll mv, ll c){
    auto [gi,gj] = F.nestPos[c];
    ll nn = F.n * F.n;
    ll st = F.idx(ci,cj), goal = F.idx(gi,gj);
    static vll dist, prv, pdir, plen;
    dist.assign(nn,-1); prv.assign(nn,-1); pdir.assign(nn,-1); plen.assign(nn,0);
    dist[st] = 0;
    queue<ll> que;
    que.push(st);
    while(!que.empty()){
        ll u = que.front(); que.pop();
        if(u == goal) break;
        ll ui = u / F.n, uj = u % F.n;
        ll k = (u == st) ? s.height(ui,uj) - mv : s.height(ui,uj);   //踏み台の匹数
        rep(d,4){
            ll lmax = min(k + 1, F.runLen[d][ui][uj]);
            rep(l,1,lmax+1){
                ll vi = ui + DI[d]*l, vj = uj + DJ[d]*l;
                ll v = F.idx(vi,vj);
                if(dist[v] != -1) continue;
                if(s.height(vi,vj) + mv > MAX_H) continue;
                dist[v] = dist[u] + 1;
                prv[v] = u; pdir[v] = d; plen[v] = l;
                que.push(v);
            }
        }
    }
    if(dist[goal] < 0) return {};
    vpll steps;
    for(ll v = goal; v != st; v = prv[v]) steps.push_back({pdir[v], plen[v]});
    reverse(all(steps));
    return steps;
}

//一団(色c)が巣へ近づけない(渋滞している)ときに使う。
//上からmv匹だけを、detourRouteの手順で遠回り・踏み台ジャンプを使って運ぶ。
//mvは巣まで行ける最大の匹数(全員では行けなければ、下の仲間を残して上だけを動かす)。
//巣か、止まった地点より巣に近いマスに着くまで手順に沿って進む。動けたらtrue。
bool detourStep(State &s, ll &ci, ll &cj, ll c){
    const vvll &nd = F.nestDist[c];
    auto [gi,gj] = F.nestPos[c];
    ll m = topRun(s,ci,cj,c);
    ll nd0 = nd[ci][cj];
    ll mv = 0;
    vpll steps;
    mrep(x,m,1){
        steps = detourRoute(s, ci, cj, x, c);
        if(!steps.empty()){ mv = x; break; }
    }
    if(mv == 0) return false;
    ll before = (ll)s.ops.size();
    for(auto [d,l] : steps){
        if((ll)s.ops.size() >= MAX_T || timer.now() > TL) break;
        ll h = s.height(ci,cj);
        if(h < mv || !s.tryDo(Action(ci,cj,h-mv,d,l))) break;
        ci += DI[d]*l; cj += DJ[d]*l;
        if(ci == gi && cj == gj) break;
        if(nd[ci][cj] < nd0) break;                 //渋滞を抜けた
    }
    return (ll)s.ops.size() > before;
}

//(ci,cj)の一団(色c)のまわり距離MAG_R以内にいる同色を1つ吸収する。
//吸収は「相手をこちらへ呼ぶ」か「こちらから相手へ行く」の安い方で行う。
//評価 = 合流の手数 + 合流後のマスから巣までの距離(小さいほど良い)。
//評価の良い順に実際に合流してみて、合流後の一団が巣まで行けなければ(detourRouteが無い)
//取り消して次の候補を試す(大きくなりすぎて渋滞を抜けられなくなる吸収をしない)。吸収できたらtrue。
bool absorbNear(State &s, ll &ci, ll &cj, ll c){
    const vvll &nd = F.nestDist[c];
    ll m = topRun(s,ci,cj,c);
    if(m <= 0 || m >= MAX_H) return false;
    struct Cand {
        ll score, g;
        ll fi, fj;    //動かす側
        vpll steps;
    };
    vector<Cand> cands;
    for(ll q : nearCells[F.idx(ci,cj)]){
        ll qi = q / F.n, qj = q % F.n;
        ll g = topRun(s,qi,qj,c);
        if(g == 0 || m + g > MAX_H) continue;
        //相手をこちらへ呼ぶ
        vpll st = mergeSteps(s, qi, qj, ci, cj, c);
        if(!st.empty()) cands.push_back({(ll)st.size() + nd[ci][cj], g, qi, qj, st});
        //こちらから相手へ行く
        st = mergeSteps(s, ci, cj, qi, qj, c);
        if(!st.empty()) cands.push_back({(ll)st.size() + nd[qi][qj], g, ci, cj, st});
    }
    //評価が小さい順、同じなら吸収数が多い順(同順位は元の順のまま)
    stable_sort(all(cands), [](const Cand &a, const Cand &b){
        if(a.score != b.score) return a.score < b.score;
        return a.g > b.g;
    });
    for(const Cand &x : cands){
        ll before = (ll)s.ops.size();
        ll pi = x.fi, pj = x.fj;
        bool ok = runSteps(s, pi, pj, c, x.steps);
        //合流後の一団(一番上から続く色cの全部)で巣まで行けるか
        if(ok) ok = !detourRoute(s, pi, pj, topRun(s,pi,pj,c), c).empty();
        if(!ok){
            while((ll)s.ops.size() > before) s.undo();
            continue;
        }
        if(x.fi == ci && x.fj == cj){ ci = pi; cj = pj; }
        return true;
    }
    return false;
}

//色cのスライムを巣へ引き寄せる。巣から遠い一団から順に、吸収しながら巣へ運ぶ。
void magnetColor(State &s, ll c){
    const vvll &nd = F.nestDist[c];
    auto [gi,gj] = F.nestPos[c];
    vb skip(F.n*F.n, false);   //動かせなかった出発点
    while(true){
        if((ll)s.ops.size() >= MAX_T || timer.now() > TL) return;
        //一番上が色cのマスのうち、巣から最も遠いもの
        ll si = -1, sj = -1, sd = -1;
        rep(i,F.n) rep(j,F.n){
            if(skip[F.idx(i,j)] || nd[i][j] < 0) continue;
            if(topRun(s,i,j,c) == 0) continue;
            if(nd[i][j] > sd){ sd = nd[i][j]; si = i; sj = j; }
        }
        if(si < 0) return;

        ll ci = si, cj = sj;
        ll before = (ll)s.ops.size();
        while(ci != gi || cj != gj){
            if((ll)s.ops.size() >= MAX_T || timer.now() > TL) return;
            if(topRun(s,ci,cj,c) == 0) break;
            if(absorbNear(s, ci, cj, c)) continue;          //まわりの同色を先に吸収する
            if(stepToNest(s, ci, cj, c)) continue;
            if(!detourStep(s, ci, cj, c)) break;            //遠回りや分割でも巣へ行けない
        }
        if((ll)s.ops.size() == before) skip[F.idx(si,sj)] = true;
    }
}

//全色を磁石方式で帰巣させる。スライムが多い色から処理する。
void runMagnet(State &s){
    vpll order;   //(-匹数, 色)
    rep(c,F.k) order.push_back({-F.colorNum[c], c});
    sort(all(order));
    for(auto [negCnt, c] : order){
        if((ll)s.ops.size() >= MAX_T || timer.now() > TL) return;
        magnetColor(s, c);
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
    //  (1) 磁石方式: 色ごとに、巣から遠い一団から、まわりの同色を吸収しながら巣へ運ぶ
    //  (2) 取りこぼしを、左上から順に一番上のスライムから巣へ帰していく
    calcCellDist();
    calcNearCells();
    runMagnet(s);
    ll magnetT = s.ops.size();

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
    //磁石方式の手数と、仕上げ後の手数・残り
    cerr << "MAGNET magnet_T=" << magnetT
         << " T=" << s.ops.size() << " E=" << s.remain << "\n";
#else
    (void)magnetT;
#endif

    s.print(cout);
    return 0;
}
