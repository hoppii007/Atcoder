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
//     乗客(execTreeRide): 木を運ぶとき、後で処理する色のスライムを最大 RIDE_MAX(=4) 匹まで一緒に運ぶ。
//     乗客は乗り場(木のノード)まで自力で跳んで一団の上に乗り、巣までの経路上の降り場で降りる。
//     降り場で乗客が一番下ならXが残して出発し、一番上なら下の全員を踏み台にして投げる(飛距離は塔の高さまで)。
//     投げる先は届くマスのうち乗客の巣に一番近いマス(巣に届けばそのまま帰巣)。
//     乗客が最後に落ち着くマスは、下の全員が同色か乗客より後に処理する色のマスに限る(上から処理順に並んだ塔になるので、
//     各色は自分の番に一番上に出て、そのまま木の葉になる。先に処理する色を埋めて回収しにくくはしない)。
//     ジャンプで上下が反転するので、
//     降りるときに乗客が一団の端にいるかは盤面のコピーで実行して確かめ、成り立つものだけ採用する。
//     降り場の見込み = min(M(降り場), 1 + M(投げた先))。投げた先は降り場に着いたときの塔の高さの見込みから求める。
//     見込みの得 = M(今のマス) - 乗り場までの手数 - 降り場の見込み が RIDE_MINGAIN(=3) 以上の候補を得の大きい順に、
//     降り場を見込みの良い順に3つまで実際に運んでみて、実際の得(乗客が最後に落ち着いたマスで測る)が一番大きいものを採る。
//     M は乗客の色の巣か同色の他のスライムまでの床距離(木で合流すれば後はまとめて運べるので、巣への距離より実態に近い)。
//     焼きなました便では、後の便のノードに乗客を着地・降車させない。
//     一団の乗客(幹への合流): 一人ずつの乗客の前に、後の色のスライムを乗り場の近く(FEED_R(=4)手以内)で一団(2匹以上)に
//     まとめてから乗せる(buildFeeders: 木のノードから逆向きBFSでつなぐ、buildTree と同じ要領)。一団は色ごとに1つ。
//     乗り場と降り場(見込みの良い順に2つ)の組を盤面のコピーで実際に運び、
//     得 = (乗せた色の残りの見込み手数(estColor: 貪欲の木のノード数)の減り) - (増えた手数) が GROUP_MINGAIN(=2) 以上で最大のものを採る。
//     降り場では一団ごと、下に残るか投げる。
// (2) 仕上げ
//     取りこぼしを、左上から順に一番上のスライムから carryTop で巣まで運ぶ。
//     carryTop は近くの同色(collectNearby)・道中の同色(ルートDP)・横の同色(sidePick)を
//     回収しながら巣へ向かい、ルート上で回収するものが無いマスは routeJump で飛び越える。
// (3) 多試行 + 焼きなまし
//     GREEDY_TL(=200ms) まで (1)(2) を乱数(色の順・BFSの方向順)を変えて最初から独立に繰り返し、最良の色の順を選ぶ。
//     その色の順で各色を運ぶ。スライムのマスが DP_TMAX(=14) 以下の色は(13マス以上は時間が足りるときだけ)、まず厳密DP(solveColorDP)で解く:
//       dp[S][v] = 葉の集合Sをマスvに集める最小手数(Dreyfus-Wagner型、容量 base+|S|<=8 は S から分かる)、
//       巣では容量なしで合流できるので最後に葉全体を一団ごとに分割する。焼きなましと同じモデルの最適解になる。
//       着地の制約を緩めているので復元した手順を実際に指して確かめ、だめなら戻して焼きなましへ。
//     それ以外の色は TRIAL_TL(=1550ms) まで焼きなまして(annealPlan)から運ぶ。
//     時間は焼きなます色のスライム数に比例して配る。貪欲の最良と比べて良い方を出力する。
//     便: 各色を複数の木(便)に分け、便ごとに帰りがけ順で運ぶ(buildPlan で先の便の葉を除いて buildTree を繰り返す)。
//     先の便の葉は運び終わっているので、後の便はそのマスにも着地できる。便をずらせば同じマスを何度も
//     使えるので、8匹を超える同色も合流点や巣までの道を共有できる(1便の中の容量 8 は守る)。
//     焼きなまし: 色ごとの手数 = 全便のノード数の和 なので、これを直接減らす。部分木を切り離して上へ続く一本道を消し、
//     BFS でつなぎ直す(最短+ANNEAL_EXTRA 手以内の候補から選ぶ)のを部品に、次の4種の近傍を使う:
//       1. ランダムなノードをつなぎ直す
//       2. 合流点(その便の葉でない分岐ノード)を消し、その子をランダムな順につなぎ直す(合流点が移る)
//       3. 同じ便で根の直下の枝(巣へ一度に運ぶ8匹以下のまとまり)が異なる2つの葉を、互いに相手の枝へつなぎ直す
//       4. 子のない葉を別の便(空の便も含む)へ移す。移し先より前の便がそのマスを通り道にしていないときだけ
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

const double TRIAL_TL  = 1550;  //焼きなましを終える時刻(ms)(調整ノブ)
const double GREEDY_TL = 200;   //この時刻まで色の順をランダムに変えて貪欲の木で試す(ms)(調整ノブ)
//------------------------------ 合流木方式 ------------------------------
//色を1色ずつ処理する。処理中の色c以外のスライムは動かさないので、各マスの
//「色cの一団の下にいる匹数(base)」は色cを処理している間変わらない。
//色cの巣を根、色cのスライムがいるマスを葉とし、一団が着地するマスをノードとする木を作り、
//葉から順に親へ跳んで合流させながら巣へ運ぶ。1手の飛距離はそのマスのbase+1まで。

const ll TREE_CAP = 8;   //木の1つのノードに集める匹数の上限(調整ノブ)。8未満にすると大きな一団ができにくくなり、
                         //踏み台(高さ1)のマスにも着地できる一団が増える。着地の高さ base+load<=8 は別に守る

//色cを運ぶための木。ノードは一団が着地するマス(idx)
struct ColorTree {
    ll root = -1;
    vll par, parDir, parLen;  //親のidx, 親へ跳ぶ方向・飛距離
    vll load;                 //そのマスを通る(集まる)色cの匹数
    vll base;                 //木を作ったときの、一団の下に残る匹数
    vector<char> inTree;
    vvll child;
};

//いまの盤面で色cの木を作る(近似Steiner木)。
//木から一番近い(手数の少ない)色cのマスを、木の途中のマスへつなぐことを繰り返す。
//どのノードでも base + load <= 8 を守る(一団が着地できる高さ)。
//木のノードには着地しない道だけを使う(待っている一団に意図せず合流しないように)。飛び越えるのはよい。
//skipが与えられたら、skip[x]のマスの色cは先の便で運び終わったものとして葉にしない(着地はしてよい)。
void buildTree(const State &s, ll c, ColorTree &T, const vector<char> *skip = nullptr){
    ll nn = F.n * F.n;
    auto [gi,gj] = F.nestPos[c];
    ll root = F.idx(gi,gj);
    T.root = root;
    T.par.assign(nn,-1); T.parDir.assign(nn,-1); T.parLen.assign(nn,0);
    T.load.assign(nn,0); T.inTree.assign(nn,0);
    if((ll)T.child.size() != nn) T.child.assign(nn,vll());
    else for(vll &v : T.child) v.clear();   //確保済みの領域を使い回す
    T.inTree[root] = 1;

    static vll g, base;   //g: 一番上から続く色cの数(0なら葉ではない)
    g.assign(nn,0); base.assign(nn,0);
    rep(x,nn){
        if(!F.isFloor[x / F.n][x % F.n]) continue;
        g[x] = topRun(s, x / F.n, x % F.n, c);
        base[x] = s.t[x].h - g[x];
    }
    g[root] = 0;
    if(skip) rep(x,nn) if((*skip)[x]) g[x] = 0;
    T.base = base;

    static vll slack, dist, nxt, ndir, nlen, src, stk, que;
    nxt.resize(nn); ndir.resize(nn); nlen.resize(nn); src.resize(nn);
    while(true){
        //slack[x] = xへあと何匹集められるか(x から根の手前までの base+load の余裕の最小)
        slack.assign(nn,-1);
        slack[root] = inf;
        stk.assign(1,root);
        while(!stk.empty()){
            ll u = stk.back(); stk.pop_back();
            for(ll ch : T.child[u]){
                ll cap = min(MAX_H - base[ch], TREE_CAP) - T.load[ch];
                if(u == root) chmin(cap, min(MAX_H - base[root], TREE_CAP) - T.load[ch]);  //巣に着地するとき
                slack[ch] = min(slack[u], cap);
                stk.push_back(ch);
            }
        }
        //余裕のあるノードから逆向きにBFSして、一番近い葉を探す
        dist.assign(nn,-1);   //nxt などは dist を入れたマスでだけ読む
        que.clear();
        rep(x,nn) if(T.inTree[x] && slack[x] >= 1){ dist[x] = 0; src[x] = x; que.push_back(x); }
        ll found = -1;
        for(ll qh = 0; qh < (ll)que.size() && found < 0; qh++){
            ll u = que[qh];
            ll ui = u / F.n, uj = u % F.n;
            ll d0 = rnd(4);
            rep(t,4){
                ll d = (d0 + t) % 4;   //vからd方向へl跳ぶとuに着く
                ll lmax = min(MAX_H, F.runLen[d^1][ui][uj]), step = DI[d]*F.n + DJ[d];   //d^1 は逆向き
                rep(l,1,lmax+1){
                    ll v = u - step*l;
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
                    que.push_back(v);
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
}

//木を葉から順に(帰りがけ順に)動かす。uに子の一団が全員集まってから、uの一団を親へ跳ばす
void execTree(State &s, ll c, const ColorTree &T, ll u){
    for(ll ch : T.child[u]) execTree(s, c, T, ch);
    if(u == T.root) return;
    if((ll)s.ops.size() >= MAX_T) return;
    ll i = u / F.n, j = u % F.n;
    ll m = topRun(s,i,j,c);
    if(m == 0) return;
    s.tryDo(Action(i,j,s.height(i,j)-m,T.parDir[u],T.parLen[u]));
}

//------------------------------ 乗客(相乗り) ------------------------------
//色Xの木を運ぶとき、後で処理する色のスライムを乗客として一緒に運ぶ。
//乗客は乗り場v(木のノード)まで自力で跳び、Xの一団の上に乗る。vから巣までの経路上で、
//乗客の巣に一番近いノードwで降りる(一番上にいれば自力で巣へ近づく向きに跳び降り、一番下にいればXが残して出発する)。
//得 = M(今のマス) - (乗り場までの手数) - M(w)。M は乗客の色の巣か同色の他のスライムまでの床距離(木で合流すれば後はまとめて運べるため)。
//得の大きい順に採用し、採用するたびに盤面のコピーで木の実行を試して、成り立つときだけ残す。
const ll RIDE_MAX     = 4;  //1便あたりの乗客の上限(調整ノブ)
const ll RIDE_MINGAIN = 3;  //乗客にする最小の得(手)(調整ノブ)

struct Passenger {
    ll c, s, v, w;            //色, 今のマス(一団のときは負の番号), 乗り場, 降り場
    vector<Action> board;     //乗り場までの手
    ll m = 1;                 //一緒に乗る匹数(一団なら2以上)
};

//色cの1匹が各マスから巣へ行く最短手数(土台 = そのマスの高さ。一番上がcならそれ自身を除く)
vll jumpDist(const State &s, ll c){
    ll nn = F.n * F.n;
    auto [gi,gj] = F.nestPos[c];
    ll root = F.idx(gi,gj);
    vll dist(nn, inf);
    static vll que;
    que.assign(1, root);
    dist[root] = 0;
    for(ll qh = 0; qh < (ll)que.size(); qh++){
        ll u = que[qh];
        ll ui = u / F.n, uj = u % F.n;
        if(u != root && s.t[u].h + 1 > MAX_H) continue;   //uに着地できない
        rep(d,4){
            ll lmax = min(MAX_H, F.runLen[d^1][ui][uj]), step = DI[d]*F.n + DJ[d];   //d^1 は逆向き
            rep(l,1,lmax+1){
                ll v = u - step*l;   //vからd方向へl跳ぶとuに着く
                ll b = s.t[v].h - ((s.t[v].h > 0 && s.t[v].top() == c) ? 1 : 0);
                if(l > b + 1 || dist[v] < inf) continue;
                dist[v] = dist[u] + 1;
                que.push_back(v);
            }
        }
    }
    return dist;
}

//一番上の1匹を from から跳ばしていく最短経路(BFS)。forbidのマスには着地しない(行き先としてだけは着地してよい)。
//avoidのマスには着地しない。dist[x] = 手数(届かなければ-1)。pre/act で経路を復元する
void singleBFS(const State &s, ll from, const vector<char> &forbid, ll avoid, vll &dist, vll &pre, vector<Action> &act){
    ll nn = F.n * F.n;
    dist.assign(nn,-1); pre.assign(nn,-1); act.assign(nn,Action());
    queue<ll> que;
    dist[from] = 0; que.push(from);
    while(!que.empty()){
        ll u = que.front(); que.pop();
        if(u != from && forbid[u]) continue;   //禁止マスからは先へ進まない
        ll ui = u / F.n, uj = u % F.n;
        ll h = s.t[u].h + (u == from ? 0 : 1);  //この1匹がuにいるときの高さ
        rep(d,4) rep(l,1,h+1){
            if(l > F.runLen[d][ui][uj]) break;
            ll v = F.idx(ui + DI[d]*l, uj + DJ[d]*l);
            if(dist[v] != -1 || v == avoid) continue;
            if(s.t[v].h + 1 > MAX_H) continue;
            dist[v] = dist[u] + 1; pre[v] = u; act[v] = Action(ui,uj,h-1,d,l);
            que.push(v);
        }
    }
}

//乗客を運んだ結果: 乗客が最後に落ち着いたマス(帰巣したら巣)と、投げたか
struct RideResult { vll finalPos, thrown; };

//色Xの木Tを、乗客psを乗せて運ぶ。途中で成り立たなくなれば false(sは途中まで進んでいる)。
//reservedのマスには乗客を投げない。D[c] は色cの jumpDist。resがあれば各乗客の結果を書く
bool execWithRiders(State &s, ll X, const ColorTree &T, const vector<Passenger> &ps,
                    const vector<vll> &D, const vector<char> &reserved, const vll &later, RideResult *res = nullptr){
    //乗客cの下に置いてよい色か: 同色か、cより後に処理する色(その色の番にはcはもういないので、一番上に出て葉になる)
    auto okBelow = [&](ll col, ll c){ return col == c || later[col] > later[c]; };
    ll np = ps.size();
    if(res){ res->finalPos.assign(np,-1); res->thrown.assign(np,0); }
    vector<char> onboard(np,0), gone(np,0);
    vvll boardAt(F.n * F.n);
    rep(p,np) boardAt[ps[p].v].pb(p);
    //このマスから一緒に跳ぶ色か(Xと、ここで降りない乗客)
    auto flies = [&](ll col, ll u){
        if(col == X) return true;
        rep(p,np) if(onboard[p] && !gone[p] && ps[p].c == col && ps[p].w != u) return true;
        return false;
    };
    //降り場で一番上にいる乗客を、下の全員を踏み台にして投げる(飛距離は塔の高さhまで)。
    //届くマスのうち乗客の巣に一番近いマスへ(巣に届けばそのまま帰巣する)
    auto jumpOff = [&](ll p, ll at){
        ll c = ps[p].c, ai = at / F.n, aj = at % F.n, h = s.t[at].h, m = ps[p].m;
        ll nest = F.idx(F.nestPos[c].fst, F.nestPos[c].snd);
        ll best = -1; Action ba;
        if(topRun(s, ai, aj, c) != m) return false;
        rep(d,4) rep(l,1,h-m+2){   //下に h-m 匹残すので飛距離 h-m+1 まで
            if(l > F.runLen[d][ai][aj]) break;
            ll v = F.idx(ai + DI[d]*l, aj + DJ[d]*l);
            if(v != nest && (T.inTree[v] || reserved[v])) continue;
            if(s.t[v].h + m > MAX_H) continue;
            if(v != nest){   //先に処理する色を埋めない(下は同色か後の色だけ)
                bool okv = true;
                rep(z,s.t[v].h) if(!okBelow(s.t[v].at(z), c)) okv = false;
                if(!okv) continue;
            }
            if(best < 0 || D[c][v] < D[c][best]){ best = v; ba = Action(ai,aj,h-m,d,l); }
        }
        if(best < 0 || !s.tryDo(ba)) return false;
        if(res){ res->finalPos[p] = best; res->thrown[p] = 1; }
        return true;
    };
    bool ok = true;
    auto dfs = [&](auto &&self, ll u) -> void {
        for(ll ch : T.child[u]){ self(self, ch); if(!ok) return; }
        if(u == T.root) return;
        ll ui = u / F.n, uj = u % F.n;
        //乗車
        for(ll p : boardAt[u]){
            for(const Action &a : ps[p].board) if(!s.tryDo(a)){ ok = false; return; }
            if(s.t[u].h == 0 || s.t[u].top() != ps[p].c){ ok = false; return; }
            if(ps[p].m > 1 && topRun(s, ui, uj, ps[p].c) != ps[p].m){ ok = false; return; }
            onboard[p] = 1;
        }
        //出発: 上から続く「一緒に跳ぶ色」が一団。その下にXや乗ったままの乗客が残ってはいけない
        ll h = s.t[u].h, fly = 0;
        while(fly < h && flies(s.t[u].at(h-1-fly), u)) fly++;
        rep(z,h-fly) if(flies(s.t[u].at(z), u)){ ok = false; return; }
        if(fly == 0) return;
        if(!s.tryDo(Action(ui,uj,h-fly,T.parDir[u],T.parLen[u]))){ ok = false; return; }
        //降車: 着地したマスで降りる乗客
        ll pu = T.par[u];
        rep(p,np){
            if(!onboard[p] || gone[p] || ps[p].w != pu) continue;
            ll H = s.t[pu].h, idx = -1;
            rep(z,H) if(s.t[pu].at(z) == ps[p].c) idx = z;
            gone[p] = 1;
            if(res) res->finalPos[p] = pu;
            if(idx < 0) continue;                                    //巣で帰巣した
            if(idx == H-1){ if(!jumpOff(p,pu)){ ok = false; return; } continue; }  //一番上なら投げる
            rep(z,idx) if(s.t[pu].at(z) == X){ ok = false; return; }  //下にXがいるとXが残して出発できない
            rep(z,idx) if(!okBelow(s.t[pu].at(z), ps[p].c)){ ok = false; return; }  //先に処理する色を埋めない
        }
    };
    dfs(dfs, T.root);
    if(!ok) return false;
    rep(p,np) if(!gone[p]) return false;
    return true;
}


//------------------------------ 幹への合流(一団の乗客) ------------------------------
//色Xの木を幹にして、後で処理する色eのスライムを近くで一団(2匹以上)にまとめてから乗せる。
//まとめ方(フィーダー): Xの木のノード(乗り場)から逆向きBFSで、FEED_R手以内の一番近い色eのマスをつなぐことを繰り返す
//(buildTree と同じ要領。つないだ道のマスもフィーダーのノードになり、後の葉はそこへも合流できる)。
//フィーダーの道は、Xの木のノードと、一番上がXのマスには着地しない。乗り場vに集められる匹数は 8 - (vの土台 + Xの一団)。
//採るかどうかは盤面のコピーで実際に運んで決める:
//  得 = (乗せた色eの残りの見込み手数の減り) - (増えた手数)  が GROUP_MINGAIN 以上なら採る。
TUNABLE(ll, FEED_R, 4);          //一団を集める道の長さの上限(手)(調整ノブ)
TUNABLE(ll, GROUP_MINGAIN, 2);   //一団を乗せる最小の得(手)(調整ノブ)

//色eの残りの見込み手数: 今の盤面での貪欲の木のノード数 + つなげなかった匹数 * 3(乱数の状態は戻す)
ll estColor(const State &s, ll e){
    static ColorTree T;
    uint64_t save = rnd.x;
    buildTree(s, e, T);
    rnd.x = save;
    ll nn = F.n * F.n, cnt = 0, nodes = 0, carried = 0;
    rep(x,nn){
        rep(z,s.t[x].h) if(s.t[x].at(z) == e) cnt++;
        if(T.inTree[x] && x != T.root) nodes++;
    }
    for(ll ch : T.child[T.root]) carried += T.load[ch];
    return nodes + 3 * (cnt - carried);
}

struct Feeder { ll v, m; vector<Action> board; };

//色Xの木Tに、色eの一団を乗せる候補を乗り場ごとに作る(2匹以上のものだけ)
void buildFeeders(const State &s, ll X, const ColorTree &T, ll e, vector<Feeder> &out, const vector<char> *noPick = nullptr){
    out.clear();
    ll nn = F.n * F.n;
    ll nest = F.idx(F.nestPos[e].fst, F.nestPos[e].snd);
    static vll g, base, cap;
    static vector<char> noLand;   //フィーダーの道が着地できないマス
    g.assign(nn,0); base.assign(nn,0); cap.assign(nn,0); noLand.assign(nn,0);
    ll gsum = 0;
    rep(x,nn){
        const Tower &w = s.t[x];
        if(T.inTree[x] || x == nest) noLand[x] = 1;
        if(w.h == 0) continue;   //スライムのいないマスは g=0, base=0 のまま
        g[x] = topRun(s, x / F.n, x % F.n, e);
        base[x] = w.h - g[x];
        if(T.inTree[x] || x == nest || w.top() == X || (noPick && (*noPick)[x])){ g[x] = 0; noLand[x] = 1; }
        gsum += g[x];
    }
    if(gsum < 2) return;   //一団(2匹以上)を作れない
    //乗り場: Xの木のノード(根以外)。集められる匹数 = 8 - (土台 + Xの一団)
    rep(v,nn) if(T.inTree[v] && v != T.root && T.load[v] > 0) cap[v] = MAX_H - (T.base[v] + T.load[v]);
    static vll par, pdir, plen, load, dist, nxt, ndir, nlen, src, slack, que;
    static vector<char> inF;
    par.assign(nn,-1); pdir.assign(nn,-1); plen.assign(nn,0); load.assign(nn,0);
    dist.resize(nn); nxt.resize(nn); ndir.resize(nn); nlen.resize(nn); src.resize(nn); slack.resize(nn);
    inF.assign(nn,0);
    auto isBoard = [&](ll v){ return T.inTree[v] && cap[v] > 0; };
    //ノードxからあと何匹足せるか(乗り場までの経路上の余裕の最小)
    auto getSlack = [&](ll x){
        ll r = inf;
        for(ll u = x; ; u = par[u]){
            if(isBoard(u)){ chmin(r, cap[u] - load[u]); break; }
            chmin(r, MAX_H - base[u] - load[u]);
        }
        return r;
    };
    while(true){
        fill(all(dist), -1);
        que.clear();
        rep(x,nn) if((isBoard(x) || inF[x])){
            slack[x] = getSlack(x);
            if(slack[x] >= 1){ dist[x] = 0; src[x] = x; que.push_back(x); }
        }
        ll found = -1;
        for(ll qh = 0; qh < (ll)que.size() && found < 0; qh++){
            ll u = que[qh];
            if(dist[u] >= FEED_R) continue;
            ll ui = u / F.n, uj = u % F.n;
            rep(d,4){
                ll lmax = min(MAX_H, F.runLen[d^1][ui][uj]), step = DI[d]*F.n + DJ[d];   //d^1 は逆向き
                rep(l,1,lmax+1){
                    ll v = u - step*l;   //vからd方向へl跳ぶとuに着く
                    if(l > base[v] + 1) continue;
                    if(dist[v] != -1 || T.inTree[v] || inF[v]) continue;
                    if(g[v] == 0 && (noLand[v] || base[v] + 1 > MAX_H)) continue;
                    dist[v] = dist[u] + 1; nxt[v] = u; ndir[v] = d; nlen[v] = l; src[v] = src[u];
                    if(g[v] > 0){
                        bool okp = g[v] <= slack[src[v]];
                        for(ll p = v; okp && !T.inTree[p] && !inF[p]; p = nxt[p]) if(base[p] + g[v] > MAX_H) okp = false;
                        if(okp){ found = v; break; }
                        continue;
                    }
                    que.push_back(v);
                }
                if(found >= 0) break;
            }
        }
        if(found < 0) break;
        ll w = g[found], p = found;
        while(!T.inTree[p] && !inF[p]){
            inF[p] = 1;
            par[p] = nxt[p]; pdir[p] = ndir[p]; plen[p] = nlen[p];
            load[p] += w;
            p = nxt[p];
        }
        for(; ; p = par[p]){
            if(T.inTree[p]){ load[p] += w; break; }
            load[p] += w;
        }
        g[found] = 0;
    }
    //乗り場ごとに、フィーダーを帰りがけ順に盤面のコピーで動かして手順を記録する
    static vvll ch;
    ch.resize(nn);
    for(vll &v : ch) v.clear();
    rep(x,nn) if(inF[x]) ch[par[x]].push_back(x);
    rep(v,nn){
        if(!isBoard(v) || load[v] < 2) continue;
        State tmp = s.board();
        ll before = (ll)tmp.ops.size();
        bool okf = true;
        auto dfs = [&](auto &&self, ll u) -> void {
            for(ll c2 : ch[u]){ self(self, c2); if(!okf) return; }
            if(u == v) return;
            ll ui = u / F.n, uj = u % F.n, m = topRun(tmp, ui, uj, e);
            if(m != load[u] || !tmp.tryDo(Action(ui, uj, tmp.t[u].h - m, pdir[u], plen[u]))) okf = false;
        };
        dfs(dfs, v);
        if(!okf) continue;
        out.push_back({v, load[v], vector<Action>(tmp.ops.begin() + before, tmp.ops.end())});
    }
}

//色Xの木Tを運ぶ。later[c] = 色cがXより後に処理する色なら処理順の番号(大きいほど後)、そうでなければ -1(乗客にできない)。
//reserved のマスは後の便が使うので、乗客は着地しない・降りない。
//noPick のマスのスライムは乗客にしない(後の便の相乗りで運ぶ予定のもの)
void execTreeRide(State &s, ll X, const ColorTree &T, const vll &later, const vector<char> &reserved, const vector<char> *noPick = nullptr){
    ll nn = F.n * F.n;
    //D[c] = 色cの1匹が各マスから巣へ行く最短手数。乗客の候補が出た色だけ計算する
    vector<vll> D(F.k);
    auto needD = [&](ll c){ if(D[c].empty()) D[c] = jumpDist(s,c); };
    vector<char> forbid(nn,0);
    rep(x,nn) if(T.inTree[x] || reserved[x]) forbid[x] = 1;

    //乗客の候補: 一番上にいる後の色のスライムごとに、得の見込みが一番大きい乗り場を選ぶ
    struct Cand { ll gain, c, s, v; vll ws; vector<Action> board; };
    vector<Cand> cands;
    vll dist, pre;
    vector<Action> act;
    //M: 色cの巣と色cのスライムがいるマスからの床距離。乗客xについては x 以外からの距離を使うので、
    //一番近い発生源(s1, 距離d1)と、それとは別の2番目に近い発生源までの距離(d2)を持っておく(候補ごとにBFSしなくてよい)
    static vector<vll> d1, d2, s1;
    static vpll mq;   //(マス, 発生源)
    d1.resize(F.k); d2.resize(F.k); s1.resize(F.k);
    vector<char> mDone(F.k,0);
    auto calcM = [&](ll c){
        if(mDone[c]) return;
        mDone[c] = 1;
        d1[c].assign(nn,inf); d2[c].assign(nn,inf); s1[c].assign(nn,-1);
        mq.clear();
        auto addSrc = [&](ll y){ d1[c][y] = 0; s1[c][y] = y; mq.push_back({y,y}); };
        ll nest = F.idx(F.nestPos[c].fst, F.nestPos[c].snd);
        addSrc(nest);
        rep(y,nn){
            if(y == nest) continue;
            rep(z,s.t[y].h) if(s.t[y].at(z) == c){ addSrc(y); break; }
        }
        for(ll qh = 0; qh < (ll)mq.size(); qh++){
            auto [u,sr] = mq[qh];
            ll du = (s1[c][u] == sr) ? d1[c][u] : d2[c][u];
            ll ui = u / F.n, uj = u % F.n;
            rep(d,4){
                if(F.runLen[d][ui][uj] == 0) continue;
                ll v = u + DI[d]*F.n + DJ[d];
                if(s1[c][v] < 0){ d1[c][v] = du + 1; s1[c][v] = sr; mq.push_back({v,sr}); }
                else if(s1[c][v] != sr && d2[c][v] >= inf){ d2[c][v] = du + 1; mq.push_back({v,sr}); }
            }
        }
    };
    //乗客x(色c)から見た M(y)
    auto Mof = [&](ll c, ll x, ll y){ return s1[c][y] != x ? d1[c][y] : d2[c][y]; };
    //降り場wでの見込み: 下にいて降りる(M[w]) か、上にいて投げる(1手 + 投げた先のM)の良い方。
    //投げる先は、wに着いたときの塔の高さの見込み(今の高さ + 運ばれてくる一団 + 乗客)まで飛べるとして、巣に一番近いマス
    //乗客cをマスxの下からhb匹の上に置いてよいか: 下の全員が同色か、cより後に処理する色
    auto okStack = [&](ll c, ll x, ll hb){
        rep(z,hb){ ll col = s.t[x].at(z); if(col != c && later[col] <= later[c]) return false; }
        return true;
    };
    //見込みのうち M によらない部分: 下にいて降りてよいか(okLow)と、投げるときの着地点(land、なければ-1)
    auto dropParts = [&](ll c, ll w, ll prev, bool &okLow, ll &land){
        ll nest = F.idx(F.nestPos[c].fst, F.nestPos[c].snd);
        ll wi = w / F.n, wj = w % F.n;
        //下にいて残るのは、Xの一団を除いた下の全員が同色か後の色のときだけ(先に処理する色を埋めない)
        ll hb = s.t[w].h - topRun(s, wi, wj, X);
        okLow = okStack(c, w, hb);
        land = -1;
        ll h = min(MAX_H, s.t[w].h + T.load[prev] + 1);
        rep(d,4) rep(l,1,h+1){
            if(l > F.runLen[d][wi][wj]) break;
            ll y = F.idx(wi + DI[d]*l, wj + DJ[d]*l);
            if(y != nest && (T.inTree[y] || reserved[y])) continue;
            if(s.t[y].h + 1 > MAX_H) continue;
            if(y != nest && !okStack(c, y, s.t[y].h)) continue;   //先に処理する色を埋めない
            if(land < 0 || D[c][y] < D[c][land]) land = y;
        }
    };
    auto dropValue = [&](ll c, ll w, ll prev, const vll &Mc){
        ll nest = F.idx(F.nestPos[c].fst, F.nestPos[c].snd);
        if(w == nest) return 0LL;
        bool okLow; ll land;
        dropParts(c, w, prev, okLow, land);
        ll val = okLow ? Mc[w] : inf;
        if(land >= 0) chmin(val, 1 + Mc[land]);
        return val;
    };
    //木のノードを根から順に
    vll ord(1, T.root);
    rep(z,(ll)ord.size()) for(ll ch : T.child[ord[z]]) ord.push_back(ch);
    //色cの乗客について、辺(w=par(v), prev=v)ごとの見込みの材料(Mによらない)を前計算する。降り場にできない辺は eUse=0
    vector<vector<char>> eUse(F.k), eLow(F.k);
    vector<vll> eLand(F.k);
    auto calcEdges = [&](ll c){
        if(!eUse[c].empty()) return;
        eUse[c].assign(nn,0); eLow[c].assign(nn,0); eLand[c].assign(nn,-1);
        ll nest = F.idx(F.nestPos[c].fst, F.nestPos[c].snd);
        rep(z,1,(ll)ord.size()){
            ll v = ord[z], w = T.par[v];
            if(reserved[w] && w != T.root) continue;
            eUse[c][v] = 1;
            if(w == nest) continue;
            bool okLow; ll land;
            dropParts(c, w, v, okLow, land);
            eLow[c][v] = okLow; eLand[c][v] = land;
        }
    };
    vll pm(nn);
    rep(x,nn){
        if(s.t[x].h == 0 || T.inTree[x] || (noPick && (*noPick)[x])) continue;
        ll c = s.t[x].top();
        if(later[c] < 0) continue;
        calcM(c);
        if(Mof(c,x,x) - 1 < RIDE_MINGAIN) continue;   //乗り場まで1手・降り場の見込み0でも得が足りない
        needD(c);
        if(D[c][x] >= inf) continue;
        ll nest = F.idx(F.nestPos[c].fst, F.nestPos[c].snd);
        calcEdges(c);
        //辺(par(v), v)で降りるときの見込み
        auto edgeVal = [&](ll v) -> ll {
            ll w = T.par[v];
            if(w == nest) return 0;
            ll val = eLow[c][v] ? Mof(c,x,w) : inf;
            if(eLand[c][v] >= 0) chmin(val, 1 + Mof(c,x,eLand[c][v]));
            return val;
        };
        //pm[v] = vから巣までの経路上の降り場の見込みの最小
        pm[T.root] = inf;
        rep(z,1,(ll)ord.size()){
            ll v = ord[z];
            pm[v] = min(eUse[c][v] ? edgeVal(v) : inf, pm[T.par[v]]);
        }
        ll mx = Mof(c,x,x), bestG = -inf, bestV = -1, ub = -inf;
        //得の上限(乗り場まで1手とみなす)が足りなければ、乗り場までのBFSをしない
        rep(z,1,(ll)ord.size()) if(T.load[ord[z]] > 0) chmax(ub, mx - 1 - pm[ord[z]]);
        if(ub < RIDE_MINGAIN) continue;
        singleBFS(s, x, forbid, nest, dist, pre, act);
        rep(v,nn){
            if(!T.inTree[v] || v == T.root || dist[v] <= 0 || T.load[v] == 0) continue;
            ll g = mx - dist[v] - pm[v];
            if(g > bestG){ bestG = g; bestV = v; }
        }
        if(bestG < RIDE_MINGAIN) continue;
        //選んだ乗り場から巣までの経路上のノードを、降り場の見込みが良い順に(降り場の候補)
        Cand best{bestG, c, x, bestV, {}, {}};
        vll val(nn, inf);
        for(ll w = T.par[bestV], prev = bestV; ; prev = w, w = T.par[w]){
            if(eUse[c][prev]){ best.ws.pb(w); val[w] = edgeVal(prev); }
            if(w == T.root) break;
        }
        sort(all(best.ws), [&](ll a, ll b){ return val[a] < val[b]; });
        for(ll y = bestV; y != x; y = pre[y]) best.board.pb(act[y]);
        reverse(all(best.board));
        cands.pb(best);
    }
    sort(all(cands), [](const Cand &a, const Cand &b){ return a.gain > b.gain; });

    //見込みの大きい順に、実行が成り立つものだけ乗客にする(色・出発マス・乗り場は重ねない)。
    //降り場は見込みの良い順に3つまで実際に運んでみて、実際の得(乗客が最後に落ち着いたマスで測る)が一番大きいものにする
    vector<Passenger> ps;
    vector<char> usedColor(F.k,0);
    RideResult rr;
    //一団の乗客: 色ごとに、乗り場と降り場(見込みの良い順に2つ)を実際に運んでみて、得が一番大きいものを採る
    {
        State prev = s.board();
        execWithRiders(prev, X, T, ps, D, reserved, later);
        vector<Feeder> fs;
        rep(e,F.k){
            if((ll)ps.size() >= RIDE_MAX) break;
            if(e == X || later[e] < 0) continue;
            buildFeeders(s, X, T, e, fs, noPick);
            if(fs.empty()) continue;
            needD(e);
            //降り場の見込みに使う M(色eの巣か色eのスライムまでの床距離)
            calcM(e);
            const vll &Me = d1[e];
            ll ePrev = estColor(prev, e), opsPrev = (ll)prev.ops.size();
            ll bestGain = -inf;
            vector<Passenger> bestPs;
            State bestState;
            for(const Feeder &f : fs){
                bool clash = false;
                for(const Passenger &p : ps) if(p.v == f.v) clash = true;
                if(clash) continue;
                vll ws, val(nn, inf);
                for(ll w = T.par[f.v], pv = f.v; ; pv = w, w = T.par[w]){
                    if(!reserved[w] || w == T.root){ ws.pb(w); val[w] = dropValue(e, w, pv, Me); }
                    if(w == T.root) break;
                }
                sort(all(ws), [&](ll a, ll b){ return val[a] < val[b]; });
                rep(t, min<ll>(2, ws.size())){
                    vector<Passenger> trial = ps;
                    Passenger np{e, -1 - e, f.v, ws[t], f.board};
                    np.m = f.m;
                    trial.pb(np);
                    State tmp = s.board();
                    if(!execWithRiders(tmp, X, T, trial, D, reserved, later)) continue;
                    ll gain = (ePrev - estColor(tmp, e)) - ((ll)tmp.ops.size() - opsPrev);
                    if(gain > bestGain){ bestGain = gain; bestPs = trial; bestState = tmp; }
                }
            }
            if(bestGain >= GROUP_MINGAIN){ ps = bestPs; prev = bestState; usedColor[e] = 1; }
        }
    }
    for(const Cand &cd : cands){
        if((ll)ps.size() >= RIDE_MAX) break;
        if(usedColor[cd.c]) continue;
        bool clash = false;
        for(const Passenger &p : ps) if(p.v == cd.v || p.s == cd.s) clash = true;
        if(clash) continue;
        ll bestGain = -inf;
        vector<Passenger> bestPs;
        rep(t, min<ll>(3, cd.ws.size())){
            vector<Passenger> trial = ps;
            trial.pb({cd.c, cd.s, cd.v, cd.ws[t], cd.board});
            State tmp = s.board();
            if(!execWithRiders(tmp, X, T, trial, D, reserved, later, &rr)) continue;
            ll p = (ll)trial.size() - 1;
            ll g = Mof(cd.c, cd.s, cd.s) - (ll)cd.board.size() - rr.thrown[p] - Mof(cd.c, cd.s, rr.finalPos[p]);
            if(g > bestGain){ bestGain = g; bestPs = trial; }
        }
        if(bestGain >= RIDE_MINGAIN){ ps = bestPs; usedColor[cd.c] = 1; }
    }
    if(!ps.empty()){
        //その場で運び、成り立たなければ戻す
        ll before = (ll)s.ops.size();
        if(execWithRiders(s, X, T, ps, D, reserved, later)) return;
        while((ll)s.ops.size() > before) s.undo();
    }
    execTree(s, X, T, T.root);
}

//------------------------------ 木の焼きなまし ------------------------------
//色cの木では各ノードがちょうど1回跳ぶので、その色の手数 = 木のノード数 - 1(根)。
//色cを全員帰巣させれば後の盤面は木の形によらないので、色ごとに独立にノード数を最小化できる。
//部品: 切り離し(detach) … ノードxの部分木を切り離し、xから上へ続く一本道(色cの葉でも分岐でもないノード)を消す。
//      つなぎ直し(reattach) … xからBFSで、根につながっているノードへの道を探してつなぐ
//      (最短+ANNEAL_EXTRA手以内の候補から選ぶ。条件は buildTree と同じ: 着地は木の外か、余裕のあるノード。base+load<=8)。
//近傍1: ランダムなノードを切り離してつなぎ直す。
//近傍2(合流点をずらす): 色cの葉でない分岐ノード(Steiner点)の子を全部切り離し(分岐ノードとその上の一本道は消える)、
//       ランダムな順につなぎ直す。後の子は先の子の新しい道にも合流できるので、合流点が別の場所に移る。
//近傍3(葉の入れ替え): 根の直下の枝(巣へ一度に運ぶ8匹以下のまとまり)が異なる2つの葉を切り離し、
//       互いに相手の枝へつなぎ直す(枝が消えていたら制限なし)。
//近傍4(便の移動): 色cを複数の便(木)に分けて順に運ぶ。子のない葉を別の便へ移す。
//       便をずらせば同じマスを何度も使えるので、合流点や巣までの道を便どうしで共有できる。

TUNABLE(ll, ANNEAL_EXTRA, 2);        //つなぎ直す道の長さを最短から何手まで許すか(調整ノブ)
TUNABLE(double, ANNEAL_T0, 0.4953);  //焼きなましの初期温度(調整ノブ)
TUNABLE(double, ANNEAL_T1, 0.4256);  //焼きなましの最終温度(調整ノブ)
TUNABLE(double, P_STEINER, 0.01787); //近傍2を選ぶ確率(調整ノブ)
TUNABLE(double, P_SWAP, 0.1405);     //近傍3を選ぶ確率(調整ノブ)
TUNABLE(double, P_MOVE, 0.2714);     //近傍4を選ぶ確率(調整ノブ)

//色cの便(木)を順に作る。先の便に入った葉は除いて buildTree を繰り返す。
//先の便の葉は運び終わっているので、後の便はそのマスにも着地できる(同じマスを便をずらして何度も使える)。
void buildPlan(const State &s, ll c, vector<ColorTree> &Ts){
    ll nn = F.n * F.n;
    Ts.clear();
    vector<char> used(nn,0);
    while(true){
        ColorTree T;
        buildTree(s, c, T, &used);
        bool any = false;
        rep(x,nn){
            if(!T.inTree[x] || x == T.root) continue;
            any = true;
            if(!used[x] && topRun(s, x / F.n, x % F.n, c) > 0) used[x] = 1;   //この便の葉
        }
        if(!any) break;
        Ts.push_back(T);
    }
}

//色cの便の列Tsを、時刻deadline(ms)まで焼きなまして全便のノード数の和を減らす。
//便bの木は、葉の便番号 lb が b より小さいマス(運び終わった葉)には着地してよく、b 以上の葉には着地しない。
void annealPlan(const State &s, ll c, vector<ColorTree> &Ts, double deadline){
    if(Ts.empty()) return;
    ll nn = F.n * F.n, root = Ts[0].root;
    const vll base = Ts[0].base;
    vll g(nn,0);   //そのマスにいる色cの数(葉の重さ)
    rep(x,nn){
        if(x == root || !F.isFloor[x / F.n][x % F.n]) continue;
        g[x] = topRun(s, x / F.n, x % F.n, c);
    }

    //1便ぶんの状態: par(-1は親なし。根・木の外・切り離し中の部分木の頂点), 方向, 飛距離, load, 子の数, 木に入っているか
    struct Bt { vll par, pdir, plen, load, ccnt; vector<char> in; };
    vector<Bt> bs;
    for(auto &T : Ts){
        Bt b;
        b.par = T.par; b.pdir = T.parDir; b.plen = T.parLen; b.load = T.load; b.in = T.inTree;
        b.ccnt.assign(nn,0);
        rep(x,nn) if(b.par[x] >= 0) b.ccnt[b.par[x]]++;
        bs.push_back(b);
    }
    Bt emptyB;   //空の便(葉を移す先)
    emptyB.par.assign(nn,-1); emptyB.pdir.assign(nn,-1); emptyB.plen.assign(nn,0);
    emptyB.load.assign(nn,0); emptyB.ccnt.assign(nn,0); emptyB.in.assign(nn,0);
    emptyB.in[root] = 1;
    bs.push_back(emptyB);

    vll lb(nn, inf);   //葉がどの便で運ばれるか(infはどの便にも入っていない)
    rep(b,(ll)bs.size()) rep(x,nn) if(bs[b].in[x] && g[x] > 0 && lb[x] == inf) lb[x] = b;

    ll cost = 0;
    for(auto &b : bs) rep(x,nn) if(b.in[x] && x != root) cost++;
    vector<Bt> best = bs;
    ll bestCost = cost;

    Bt *cur = &bs[0];
    ll curB = 0;
    auto isLeaf = [&](ll p){ return g[p] > 0 && lb[p] == curB; };   //いまの便の葉か

    Bt sA, sB;   //却下したとき戻す用
    vll slb;
    vll conn(nn), branch(nn), slack(nn), dist(nn), prv(nn), pd(nn), pl(nn), stk, lst;
    vpll nodes;
    struct Cand { ll u, y, d, l, len; };
    vector<Cand> cands, shortest;

    //xの部分木を切り離し、上へ続く一本道を消す。消したノード数を返す
    auto detach = [&](ll x) -> ll {
        auto &B = *cur;
        ll L = B.load[x];
        for(ll q = B.par[x]; q != root; q = B.par[q]) B.load[q] -= L;
        ll p = B.par[x];
        B.ccnt[p]--; B.par[x] = -1;
        ll removed = 0;
        while(p != root && !isLeaf(p) && B.ccnt[p] == 0){
            ll q = B.par[p];
            B.in[p] = 0; B.par[p] = -1; B.load[p] = 0;
            B.ccnt[q]--; removed++;
            p = q;
        }
        return removed;
    };
    //メモを消す(木の形が変わったら・便を変えたら呼ぶ)
    auto resetMemo = [&](){ fill(all(conn), -1); fill(all(slack), -inf); };
    //wが根につながっているか。つながっていればbranch[w]に根の直下の祖先を入れる(メモ化)
    auto connected = [&](ll w) -> bool {
        auto &B = *cur;
        if(w == root) return true;
        stk.clear();
        ll v = w, res = 0, br = -1;
        while(true){
            if(v == root){ res = 1; break; }
            if(conn[v] != -1){ res = conn[v]; br = branch[v]; break; }
            if(B.par[v] < 0){ conn[v] = 0; res = 0; break; }
            stk.push_back(v); v = B.par[v];
        }
        for(ll z = (ll)stk.size()-1; z >= 0; z--){
            ll u = stk[z];
            conn[u] = res;
            if(res){ if(br < 0) br = u; branch[u] = br; }
        }
        return conn[w] == 1;
    };
    //根につながっているノードwへ(根以外)足せる匹数の余裕(メモ化)
    auto getSlack = [&](ll w){
        auto &B = *cur;
        stk.clear();
        ll v = w;
        while(v != root && slack[v] == -inf){ stk.push_back(v); v = B.par[v]; }
        ll up = (v == root) ? inf : slack[v];
        for(ll z = (ll)stk.size()-1; z >= 0; z--){
            ll u = stk[z];
            ll cap = min(MAX_H - base[u], TREE_CAP) - B.load[u];
            if(B.par[u] == root) chmin(cap, min(MAX_H - base[root], TREE_CAP) - B.load[u]);
            up = min(up, cap);
            slack[u] = up;
        }
        return slack[w];
    };
    //切り離したxを、根につながっているノードへつなぎ直す。加えたノード数を返す(つなげなければ-1)。
    //restrictが0以上なら、根の直下がrestrictの枝にだけつなぐ
    auto reattach = [&](ll x, ll restrict) -> ll {
        auto &B = *cur;
        ll L = B.load[x];
        resetMemo();
        auto canAttach = [&](ll y){
            if(!connected(y)) return false;
            if(y == root) return restrict < 0 && L <= MAX_H - base[root];
            if(restrict >= 0 && branch[y] != restrict) return false;
            return getSlack(y) >= L;
        };
        fill(all(dist), -1);
        cands.clear();
        ll bestD = inf;
        queue<ll> que;
        dist[x] = 0; que.push(x);
        while(!que.empty()){
            ll u = que.front(); que.pop();
            if(dist[u] + 1 > bestD + ANNEAL_EXTRA) break;
            ll ui = u / F.n, uj = u % F.n;
            rep(d,4){
                ll lmax = min(base[u] + 1, F.runLen[d][ui][uj]);
                rep(l,1,lmax+1){
                    ll w = F.idx(ui + DI[d]*l, uj + DJ[d]*l);
                    if(B.in[w]){
                        //切り離し中の部分木(自分も含む)には着地しない
                        if(canAttach(w)){
                            cands.push_back({u, w, d, l, dist[u] + 1});
                            chmin(bestD, dist[u] + 1);
                        }
                        continue;
                    }
                    if(g[w] > 0 && lb[w] >= curB) continue;   //まだ運ばれていない葉には着地しない
                    if(base[w] + L > MAX_H) continue;
                    if(dist[w] != -1) continue;
                    dist[w] = dist[u] + 1; prv[w] = u; pd[w] = d; pl[w] = l;
                    que.push(w);
                }
            }
        }
        if(cands.empty()) return -1;
        //半分は最短の候補から、残りは全候補から選ぶ
        shortest.clear();
        for(auto &cd : cands) if(cd.len == bestD) shortest.push_back(cd);
        Cand ch = (rnd(2) == 0) ? shortest[rnd((ll)shortest.size())] : cands[rnd((ll)cands.size())];
        //x → … → u → y の道を木に加える
        B.par[ch.u] = ch.y; B.pdir[ch.u] = ch.d; B.plen[ch.u] = ch.l;
        B.ccnt[ch.y]++;
        for(ll w = ch.u; w != x; w = prv[w]){
            B.in[w] = 1; B.load[w] = L; B.ccnt[w] = 1;
            ll pw = prv[w];
            B.par[pw] = w; B.pdir[pw] = pd[w]; B.plen[pw] = pl[w];
        }
        for(ll q = ch.y; q != root; q = B.par[q]) B.load[q] += L;
        return ch.len - 1;
    };
    auto setCur = [&](ll b){ curB = b; cur = &bs[b]; };

    double startT = timer.now(), temp = ANNEAL_T0;
    for(ll iter = 0; ; iter++){
        if((iter & 63) == 0){
            double now = timer.now();
            if(now >= deadline) break;
            double prog = (now - startT) / max(1e-9, deadline - startT);
            temp = ANNEAL_T0 * pow(ANNEAL_T1 / ANNEAL_T0, prog);
        }
        nodes.clear();
        rep(b,(ll)bs.size()) rep(y,nn) if(bs[b].in[y] && y != root) nodes.push_back({b,y});
        if(nodes.empty()) break;

        ll delta = 0, ba = -1, bb = -1;   //変えた便(戻す用)
        bool ok = true, done = false;
        slb = lb;
        double r = rnd.prob();
        if(r < P_STEINER){
            //近傍2: 分岐ノード(いまの便の葉でない)の子を全部切り離してつなぎ直す
            auto [b0, y0] = nodes[rnd((ll)nodes.size())];
            setCur(b0); ba = b0; sA = bs[b0];
            lst.clear();
            rep(y,nn) if(cur->in[y] && y != root && !isLeaf(y) && cur->ccnt[y] >= 2) lst.push_back(y);
            if(!lst.empty()){
                ll sp = lst[rnd((ll)lst.size())];
                vll chs;
                rep(y,nn) if(cur->par[y] == sp) chs.push_back(y);
                for(ll ch : chs) delta -= detach(ch);
                mrep(z,(ll)chs.size()-1,1) swap(chs[z], chs[rnd(z+1)]);
                for(ll ch : chs){
                    ll a = reattach(ch, -1);
                    if(a < 0){ ok = false; break; }
                    delta += a;
                }
                done = true;
            }
        }else if(r < P_STEINER + P_SWAP){
            //近傍3: 同じ便の中で、根の直下の枝が異なる2つの葉を入れ替える
            auto [b0, y0] = nodes[rnd((ll)nodes.size())];
            setCur(b0); ba = b0; sA = bs[b0];
            resetMemo();
            lst.clear();
            rep(y,nn) if(cur->in[y] && y != root && isLeaf(y)){ connected(y); lst.push_back(y); }
            if(lst.size() >= 2){
                ll a = lst[rnd((ll)lst.size())];
                ll bra = branch[a];
                vll others;
                for(ll y : lst) if(branch[y] != bra) others.push_back(y);
                if(!others.empty()){
                    ll b = others[rnd((ll)others.size())];
                    ll brb = branch[b];
                    delta -= detach(a);
                    delta -= detach(b);
                    //入れ替え先の枝がまだ根の直下に残っていれば、そこへつなぐ
                    ll ra = (cur->in[brb] && cur->par[brb] == root) ? brb : -1;
                    ll rb = (cur->in[bra] && cur->par[bra] == root) ? bra : -1;
                    ll x1 = reattach(a, ra);
                    if(x1 < 0) ok = false;
                    else{
                        delta += x1;
                        ll x2 = reattach(b, rb);
                        if(x2 < 0) ok = false;
                        else delta += x2;
                    }
                    done = true;
                }
            }
        }else if(r < P_STEINER + P_SWAP + P_MOVE){
            //近傍4(便の移動): 子のない葉を別の便へ移す。
            //移し先の便より前の便が、そのマスを通り道に使っていないときだけ(まだ葉が残っていて着地できないため)
            lst.clear();
            rep(y,nn) if(lb[y] != inf && bs[lb[y]].ccnt[y] == 0) lst.push_back(y);
            if(!lst.empty() && bs.size() >= 2){
                ll x = lst[rnd((ll)lst.size())];
                ll a = lb[x];
                ll b = rnd((ll)bs.size() - 1);
                if(b >= a) b++;
                bool valid = true;
                rep(j,(ll)bs.size()) if(j != a && bs[j].in[x] && j <= b) valid = false;
                if(valid){
                    ba = a; bb = b; sA = bs[a]; sB = bs[b];
                    setCur(a);
                    delta -= detach(x);
                    cur->in[x] = 0; cur->load[x] = 0;
                    delta -= 1;
                    lb[x] = b;
                    setCur(b);
                    cur->in[x] = 1; cur->load[x] = g[x]; cur->ccnt[x] = 0; cur->par[x] = -1;
                    delta += 1;
                    ll add = reattach(x, -1);
                    if(add < 0) ok = false;
                    else delta += add;
                    done = true;
                }
            }
        }
        if(!done){
            //近傍1: ランダムなノードを切り離してつなぎ直す
            auto [b0, x] = nodes[rnd((ll)nodes.size())];
            setCur(b0); ba = b0; bb = -1; sA = bs[b0];
            delta -= detach(x);
            ll a = reattach(x, -1);
            if(a < 0) ok = false;
            else delta += a;
        }

        bool accept = ok && (delta <= 0 || rnd.prob() < exp(-(double)delta / temp));
        if(!accept){
            if(ba >= 0) bs[ba] = sA;
            if(bb >= 0) bs[bb] = sB;
            lb = slb;
            continue;
        }
        cost += delta;
        //最後の便が使われたら、空の便を足しておく
        bool lastUsed = false;
        rep(y,nn) if(bs.back().in[y] && y != root){ lastUsed = true; break; }
        if(lastUsed) bs.push_back(emptyB);
        if(cost < bestCost){
            bestCost = cost;
            best = bs;
        }
    }

    //最良の便の列をTsに戻す(空の便は除く)
    ColorTree tmpl = Ts[0];
    Ts.clear();
    for(auto &b : best){
        bool any = false;
        rep(y,nn) if(b.in[y] && y != root){ any = true; break; }
        if(!any) continue;
        ColorTree T = tmpl;
        T.par = b.par; T.parDir = b.pdir; T.parLen = b.plen; T.load = b.load; T.inTree = b.in;
        T.child.assign(nn, vll());
        rep(x,nn) if(T.par[x] >= 0) T.child[T.par[x]].push_back(x);
        Ts.push_back(T);
    }
}

//------------------------------ 次の色との相乗りつき焼きなまし ------------------------------
//色Xの便と、次に処理する色Bの便をいっしょに焼きなます。Bの一団はXの木のノードu(乗り場)でXの一団の上に乗り、
//巣へ向かう経路上のノードw(降り場)まで運んでもらう(相乗り)。その区間はBの手数がかからない。
//  Bを乗り場へ集める木(pre)はXの便を運んでいる途中に動かす。根がuの小さな木で、Xの木のノード・まだ運んでいないXの葉・
//  Bの巣・他の相乗りの pre のマスには着地しない。
//  一緒に跳ぶたびに一団全体の上下が入れ替わるので、乗った直後(uから0手)はBが一番上、奇数手目のノードではBが一番下になる。
//  Bが端にいるように、奇数手目のノードでは相乗りの子を最初に、偶数手目のノードでは最後に着地させる。
//  奇数手目のノードにXの(この便の)葉があると、その上にBが来て端でなくなるので通れない。
//  降り場: 奇数手目なら、Xが下のBを残して出発する(wがBの巣ならそのまま帰巣)。
//          偶数手目ならBが一番上で着地するので、wがBの巣のときだけ(着地で帰巣)。途中の偶数手目のノードにBの巣があってはいけない。
//  容量: 乗り場から降り場までの各ノードで Xの土台 + Xのload + Bの匹数 <= 8。
//  降りたBは、Xが去った後のBの便(post)の葉になる。降り場は後のXの便では使わない。
//コスト = Xの全便のノード数 + Bのpostの全便のノード数 + preのノード数(根uを除く)。
//Bのpostは「Xを全員帰巣させた後の盤面」で作る(Xを処理した後、Bは改めてDPか焼きなましで運ぶので、ここでは見込みとして使う)。
TUNABLE(ll, JOINT_ON, 1);       //相乗りつき焼きなましを使うか
TUNABLE(double, P_JB, 0.25);    //Bのpostの近傍を選ぶ確率(調整ノブ)
TUNABLE(double, P_JR, 0.15);    //相乗りの近傍を選ぶ確率(調整ノブ)
TUNABLE(ll, JFEED_R, 4);        //新しい相乗りで、Bの葉から乗り場を探す手数の上限(調整ノブ)

struct Batch { vll par, pdir, plen, load, ccnt; vector<char> in; };
ll DBG_PLAN = 0, DBG_OK = 0, DBG_NG = 0;   //デバッグ: 計画した相乗りの便, 実行できた便, できなかった便
struct JRide {
    ll bx, u, w;   //Xの便, 乗り場, 降り場
    Batch pre;     //Bの一団をuへ集める木(根u)。pre.load[u] = 乗る匹数の合計(0なら使われていない)
};

void annealJoint(const State &s, ll X, ll B, vector<ColorTree> &Ts, vector<JRide> &outRides, const vll &later, double deadline){
    outRides.clear();
    if(Ts.empty()) return;
    ll nn = F.n * F.n, rootX = Ts[0].root, rootB = F.idx(F.nestPos[B].fst, F.nestPos[B].snd);
    const vll baseX = Ts[0].base;
    vll gX(nn,0);
    rep(x,nn){
        if(x == rootX || !F.isFloor[x / F.n][x % F.n]) continue;
        gX[x] = topRun(s, x / F.n, x % F.n, X);
    }
    //Xを全員帰巣させた後の盤面
    State pb = s.board();
    rep(x,nn) rep(_,gX[x]) pb.t[x].pop();
    vll baseB(nn,0), gB(nn,0);
    vector<char> preOk(nn,0), okDrop(nn,0);
    rep(x,nn){
        if(!F.isFloor[x / F.n][x % F.n]) continue;
        if(x != rootB) gB[x] = topRun(pb, x / F.n, x % F.n, B);
        baseB[x] = pb.t[x].h - gB[x];
        preOk[x] = gB[x] > 0 && gX[x] == 0;   //いま一番上がBのマス(Xを運んでいる途中に乗り場へ集められる)
        bool okb = true;   //降りたBの下が、Bか後で処理する色だけか
        rep(z,pb.t[x].h){ ll col = pb.t[x].at(z); if(col != B && later[col] <= later[B]) okb = false; }
        okDrop[x] = okb;
    }
    vector<ColorTree> TsB;
    buildPlan(pb, B, TsB);
    if(TsB.empty()){ annealPlan(s, X, Ts, deadline); return; }

    auto toBatch = [&](const ColorTree &T){
        Batch b;
        b.par = T.par; b.pdir = T.parDir; b.plen = T.parLen; b.load = T.load; b.in = T.inTree;
        b.ccnt.assign(nn,0);
        rep(x,nn) if(b.par[x] >= 0) b.ccnt[b.par[x]]++;
        return b;
    };
    auto mkEmpty = [&](ll root){
        Batch b;
        b.par.assign(nn,-1); b.pdir.assign(nn,-1); b.plen.assign(nn,0);
        b.load.assign(nn,0); b.ccnt.assign(nn,0); b.in.assign(nn,0);
        b.in[root] = 1;
        return b;
    };
    vector<Batch> BX, BB;
    for(auto &T : Ts) BX.push_back(toBatch(T));
    BX.push_back(mkEmpty(rootX));
    for(auto &T : TsB) BB.push_back(toBatch(T));
    BB.push_back(mkEmpty(rootB));
    //葉の便番号(infはどの便にも入っていない。Bのpreの葉は -1 で、lrB = 相乗りの番号)
    vll lbX(nn,inf), lbB(nn,inf), lrB(nn,-1), gw = gB;   //gw: Bのpostの葉の重さ(降りたBを含む)
    rep(b,(ll)BX.size()) rep(x,nn) if(BX[b].in[x] && gX[x] > 0 && lbX[x] == inf) lbX[x] = b;
    rep(b,(ll)BB.size()) rep(x,nn) if(BB[b].in[x] && gw[x] > 0 && lbB[x] == inf) lbB[x] = b;
    vector<JRide> R;

    ll cost = 0;
    for(auto &b : BX) rep(x,nn) if(b.in[x] && x != rootX) cost++;
    for(auto &b : BB) rep(x,nn) if(b.in[x] && x != rootB) cost++;
    ll bestCost = cost;
    vector<Batch> bestBX = BX;
    vector<JRide> bestR;

    //どの木を触るか。kind 0: Xの便, 1: Bのpostの便, 2: 相乗りid番のpre
    struct Ctx { Batch *bt; ll root; const vll *base; int kind; ll id; };
    auto ctxX = [&](ll b){ return Ctx{&BX[b], rootX, &baseX, 0, b}; };
    auto ctxB = [&](ll b){ return Ctx{&BB[b], rootB, &baseB, 1, b}; };
    auto ctxP = [&](ll r){ return Ctx{&R[r].pre, R[r].u, &baseB, 2, r}; };
    auto isLeaf = [&](const Ctx &k, ll p) -> bool {
        if(k.kind == 0) return gX[p] > 0 && lbX[p] == k.id;
        if(k.kind == 1) return gw[p] > 0 && lbB[p] == k.id;
        return lbB[p] == -1 && lrB[p] == k.id;
    };
    //相乗りrの乗り場から降り場までの容量の余裕(経路が壊れていれば -1)
    auto rideCap = [&](ll r) -> ll {
        const JRide &J = R[r];
        const Batch &Xb = BX[J.bx];
        if(!Xb.in[J.u] || J.u == rootX) return -1;
        ll cap = MAX_H - baseX[J.u] - Xb.load[J.u];
        for(ll p = Xb.par[J.u]; ; p = Xb.par[p]){
            if(p < 0 || p == rootX) return -1;
            chmin(cap, MAX_H - baseX[p] - Xb.load[p]);
            if(p == J.w) break;
        }
        return cap;
    };

    //xの部分木を切り離し、上へ続く一本道を消す。消したノード数を返す
    auto detach = [&](const Ctx &k, ll x) -> ll {
        Batch &Bt = *k.bt;
        ll L = Bt.load[x];
        for(ll q = Bt.par[x]; q != k.root; q = Bt.par[q]) Bt.load[q] -= L;
        if(k.kind == 2) Bt.load[k.root] -= L;
        ll p = Bt.par[x];
        Bt.ccnt[p]--; Bt.par[x] = -1;
        ll removed = 0;
        while(p != k.root && !isLeaf(k,p) && Bt.ccnt[p] == 0){
            ll q = Bt.par[p];
            Bt.in[p] = 0; Bt.par[p] = -1; Bt.load[p] = 0;
            Bt.ccnt[q]--; removed++;
            p = q;
        }
        return removed;
    };
    vll conn(nn), branch(nn), slk(nn), dist(nn), prv(nn), pd(nn), pl(nn), stk, lst;
    ll preRootCap = 0;   //kind 2: 乗り場にあと何匹乗れるか
    auto resetMemo = [&](){ fill(all(conn), -1); fill(all(slk), -inf); };
    auto connected = [&](const Ctx &k, ll w) -> bool {
        Batch &Bt = *k.bt;
        if(w == k.root) return true;
        stk.clear();
        ll v = w, res = 0, br = -1;
        while(true){
            if(v == k.root){ res = 1; break; }
            if(conn[v] != -1){ res = conn[v]; br = branch[v]; break; }
            if(Bt.par[v] < 0){ conn[v] = 0; res = 0; break; }
            stk.push_back(v); v = Bt.par[v];
        }
        for(ll z = (ll)stk.size()-1; z >= 0; z--){
            ll u = stk[z];
            conn[u] = res;
            if(res){ if(br < 0) br = u; branch[u] = br; }
        }
        return conn[w] == 1;
    };
    auto getSlack = [&](const Ctx &k, ll w) -> ll {
        Batch &Bt = *k.bt;
        const vll &base = *k.base;
        stk.clear();
        ll v = w;
        while(v != k.root && slk[v] == -inf){ stk.push_back(v); v = Bt.par[v]; }
        ll up = (v == k.root) ? inf : slk[v];
        for(ll z = (ll)stk.size()-1; z >= 0; z--){
            ll u = stk[z];
            ll cap = min(MAX_H - base[u], TREE_CAP) - Bt.load[u];
            if(Bt.par[u] == k.root){
                if(k.kind == 2) chmin(cap, preRootCap);
                else chmin(cap, min(MAX_H - base[k.root], TREE_CAP) - Bt.load[u]);
            }
            up = min(up, cap);
            slk[u] = up;
        }
        return slk[w];
    };
    //kind 2 の着地禁止: Xの便bx以降のノード・まだ運んでいないXの葉(xForbid)、他の相乗りのpreのマス(preUse)
    vector<char> xForbid(nn,0), preUse(nn,0);
    auto prepareP = [&](ll r){
        fill(all(xForbid), 0);
        ll b0 = R[r].bx;
        rep(b,b0,(ll)BX.size()) rep(q,nn) if(BX[b].in[q]) xForbid[q] = 1;
        rep(q,nn) if(gX[q] > 0 && lbX[q] >= b0) xForbid[q] = 1;
        fill(all(preUse), 0);
        rep(r2,(ll)R.size()) if(r2 != r) rep(q,nn) if(R[r2].pre.in[q] && q != R[r2].u) preUse[q] = 1;
    };
    auto canLand = [&](const Ctx &k, ll q, ll L) -> bool {
        if((*k.base)[q] + L > MAX_H) return false;
        if(k.kind == 0) return !(gX[q] > 0 && lbX[q] >= k.id);
        if(k.kind == 1) return !(gw[q] > 0 && lbB[q] >= k.id);
        return q != rootB && gw[q] == 0 && !xForbid[q] && !preUse[q];
    };
    struct Cand { ll u, y, d, l, len; };
    vector<Cand> cands, shortest;
    //切り離したxを、根につながっているノードへつなぎ直す。加えたノード数を返す(つなげなければ-1)
    auto reattach = [&](const Ctx &k, ll x, ll restrict) -> ll {
        Batch &Bt = *k.bt;
        const vll &base = *k.base;
        ll L = Bt.load[x];
        resetMemo();
        if(k.kind == 2){
            ll rc = rideCap(k.id);
            if(rc < 0) return -1;
            preRootCap = rc - Bt.load[k.root];
        }
        auto canAttach = [&](ll y){
            if(!connected(k,y)) return false;
            if(y == k.root){
                if(restrict >= 0) return false;
                if(k.kind == 2) return L <= preRootCap;
                return L <= MAX_H - base[k.root];
            }
            if(restrict >= 0 && branch[y] != restrict) return false;
            return getSlack(k,y) >= L;
        };
        fill(all(dist), -1);
        cands.clear();
        ll bestD = inf;
        static vll que;
        que.assign(1, x);
        dist[x] = 0;
        for(ll qh = 0; qh < (ll)que.size(); qh++){
            ll u = que[qh];
            if(dist[u] + 1 > bestD + ANNEAL_EXTRA) break;
            ll ui = u / F.n, uj = u % F.n;
            rep(d,4){
                ll lmax = min(base[u] + 1, F.runLen[d][ui][uj]);
                rep(l,1,lmax+1){
                    ll w = F.idx(ui + DI[d]*l, uj + DJ[d]*l);
                    if(Bt.in[w]){
                        if(canAttach(w)){
                            cands.push_back({u, w, d, l, dist[u] + 1});
                            chmin(bestD, dist[u] + 1);
                        }
                        continue;
                    }
                    if(dist[w] != -1 || !canLand(k,w,L)) continue;
                    dist[w] = dist[u] + 1; prv[w] = u; pd[w] = d; pl[w] = l;
                    que.push_back(w);
                }
            }
        }
        if(cands.empty()) return -1;
        shortest.clear();
        for(auto &cd : cands) if(cd.len == bestD) shortest.push_back(cd);
        Cand ch = (rnd(2) == 0) ? shortest[rnd((ll)shortest.size())] : cands[rnd((ll)cands.size())];
        Bt.par[ch.u] = ch.y; Bt.pdir[ch.u] = ch.d; Bt.plen[ch.u] = ch.l;
        Bt.ccnt[ch.y]++;
        for(ll w = ch.u; w != x; w = prv[w]){
            Bt.in[w] = 1; Bt.load[w] = L; Bt.ccnt[w] = 1;
            ll pw = prv[w];
            Bt.par[pw] = w; Bt.pdir[pw] = pd[w]; Bt.plen[pw] = pl[w];
        }
        for(ll q = ch.y; q != k.root; q = Bt.par[q]) Bt.load[q] += L;
        if(k.kind == 2) Bt.load[k.root] += L;
        return ch.len - 1;
    };

    //全ての相乗りが成り立つか
    vll markP(nn,-1), markQ(nn,-1);
    ll stampV = 0;
    auto validate = [&]() -> bool {
        stampV++;
        rep(r,(ll)R.size()){
            JRide &J = R[r];
            Batch &P = J.pre;
            ll L = P.load[J.u];
            if(L == 0) continue;   //使われていない
            if(J.u == rootB) return false;
            ll cap = rideCap(r);
            if(cap < L) return false;
            const Batch &Xb = BX[J.bx];
            ll key = stampV * 1024 + J.bx, d = 0;
            for(ll p = J.u; ; p = Xb.par[p]){
                if(markP[p] == key) return false;   //同じ便の相乗りどうしで経路を共有しない
                markP[p] = key;
                if(d > 0){
                    bool odd = d & 1;
                    if(odd && gX[p] > 0 && lbX[p] == J.bx) return false;   //Xの葉の上にBが来て端でなくなる
                    if(p == J.w) break;
                    if(!odd && p == rootB) return false;                   //途中で帰巣してしまう
                }
                d++;
            }
            if(d % 2 == 0 && J.w != rootB) return false;
            if(J.w != rootB){
                if(!okDrop[J.w]) return false;
                rep(b,J.bx+1,(ll)BX.size()) if(BX[b].in[J.w]) return false;   //後のXの便が降り場を使う
            }
            rep(q,nn){
                if(!P.in[q] || q == J.u) continue;
                if(markQ[q] == stampV) return false;   //相乗りどうしでpreのマスを共有しない
                markQ[q] = stampV;
                if(q == rootB) return false;
                if(gX[q] > 0 && lbX[q] >= J.bx) return false;
                rep(b,J.bx,(ll)BX.size()) if(BX[b].in[q]) return false;
            }
        }
        return true;
    };

    //Bのpostで、降り場wの葉の重さを dlt 変える。ノード数の変化を delta に足す。容量を超えたら false
    vector<pair<ll,Batch>> savedBB;   //相乗りの近傍で書き換えたBのpostの便(戻す用)
    auto touchB = [&](ll b){
        for(auto &pr : savedBB) if(pr.first == b) return;
        savedBB.push_back({b, BB[b]});
    };
    auto pathLoad = [&](ll b, ll w, ll dlt) -> bool {
        Batch &Bt = BB[b];
        bool okc = true;
        for(ll q = w; q != rootB; q = Bt.par[q]){
            if(q < 0) return false;
            Bt.load[q] += dlt;
            if(Bt.load[q] > min(MAX_H - baseB[q], TREE_CAP)) okc = false;
            if(Bt.par[q] == rootB && Bt.load[q] > MAX_H - baseB[rootB]) okc = false;
        }
        return dlt < 0 || okc;
    };
    auto addWeight = [&](ll w, ll dlt, ll &delta) -> bool {
        if(w == rootB || dlt == 0) return true;
        if(lbB[w] == -1) return false;
        if(gw[w] > 0 && lbB[w] != inf){
            ll b = lbB[w];
            touchB(b);
            gw[w] += dlt;
            if(!pathLoad(b, w, dlt)) return false;
            if(gw[w] == 0){
                lbB[w] = inf;
                Batch &Bt = BB[b];
                if(Bt.ccnt[w] == 0){
                    delta -= detach(ctxB(b), w) + 1;
                    Bt.in[w] = 0; Bt.load[w] = 0;
                }
            }
            return true;
        }
        if(dlt < 0) return false;
        gw[w] += dlt;
        ll m = -1;
        rep(b,(ll)BB.size()) if(BB[b].in[w]){ m = b; break; }
        if(m >= 0){
            //すでに通り道になっている便のうち一番前の便の葉にする(それより前の便はwに着地しない)
            touchB(m);
            lbB[w] = m;
            return pathLoad(m, w, gw[w]);
        }
        ll b = rnd((ll)BB.size());
        touchB(b);
        lbB[w] = b;
        Batch &Bt = BB[b];
        Bt.in[w] = 1; Bt.load[w] = gw[w]; Bt.ccnt[w] = 0; Bt.par[w] = -1;
        delta += 1;
        ll a = reattach(ctxB(b), w, -1);
        if(a < 0) return false;
        delta += a;
        return true;
    };
    //Xの便bxの乗り場uから、L匹のBを降ろせる降り場の候補
    auto validWs = [&](ll bx, ll u, ll L){
        vll res;
        const Batch &Xb = BX[bx];
        if(!Xb.in[u] || u == rootX || u == rootB) return res;
        ll cap = MAX_H - baseX[u] - Xb.load[u];
        if(cap < L) return res;
        ll d = 0;
        for(ll p = Xb.par[u]; p >= 0 && p != rootX; p = Xb.par[p]){
            d++;
            chmin(cap, MAX_H - baseX[p] - Xb.load[p]);
            if(cap < L) break;
            if(d & 1){
                if(gX[p] > 0 && lbX[p] == bx) break;
                if(p == rootB){ res.push_back(p); continue; }
                if(!okDrop[p]) continue;
                bool used = false;
                rep(b,bx+1,(ll)BX.size()) if(BX[b].in[p]) used = true;
                if(!used) res.push_back(p);
            }else if(p == rootB){ res.push_back(p); break; }
        }
        return res;
    };
    //乗り場へ集められるBの葉: いま一番上がBで、Bのpostの子のない葉(降りたBを含まない)
    auto preCands = [&](vll &out){
        out.clear();
        rep(x,nn){
            if(!preOk[x] || lbB[x] < 0 || lbB[x] == inf || gw[x] != gB[x]) continue;
            if(BB[lbB[x]].ccnt[x] != 0) continue;
            out.push_back(x);
        }
    };
    auto aliveRides = [&](vll &out){
        out.clear();
        rep(r,(ll)R.size()) if(R[r].pre.load[R[r].u] > 0) out.push_back(r);
    };
    //Bのpostの葉xを取り除く
    auto takeFromPost = [&](ll x, ll &delta){
        ll b0 = lbB[x];
        touchB(b0);
        delta -= detach(ctxB(b0), x) + 1;
        BB[b0].in[x] = 0; BB[b0].load[x] = 0;
    };

    //annealPlan と同じ4種の近傍を、Xの便(kind 0)かBのpostの便(kind 1)に1回試す
    Batch sA, sB;
    vll slb;
    vpll nodes;
    ll ba = -1, bb = -1;
    auto stdMove = [&](int kind, ll &delta, bool &ok) -> bool {
        vector<Batch> &BS = kind == 0 ? BX : BB;
        vll &LB = kind == 0 ? lbX : lbB;
        const vll &G = kind == 0 ? gX : gw;
        ll root = kind == 0 ? rootX : rootB;
        auto ctx = [&](ll b){ return kind == 0 ? ctxX(b) : ctxB(b); };
        nodes.clear();
        rep(b,(ll)BS.size()) rep(y,nn) if(BS[b].in[y] && y != root) nodes.push_back({b,y});
        if(nodes.empty()) return false;
        slb = LB; ba = bb = -1;
        double r = rnd.prob();
        bool done = false;
        if(r < P_STEINER){
            auto [b0, y0] = nodes[rnd((ll)nodes.size())];
            ba = b0; sA = BS[b0];
            Ctx k = ctx(b0);
            lst.clear();
            rep(y,nn) if(BS[b0].in[y] && y != root && !isLeaf(k,y) && BS[b0].ccnt[y] >= 2) lst.push_back(y);
            if(!lst.empty()){
                ll sp = lst[rnd((ll)lst.size())];
                vll chs;
                rep(y,nn) if(BS[b0].par[y] == sp) chs.push_back(y);
                for(ll ch : chs) delta -= detach(k,ch);
                mrep(z,(ll)chs.size()-1,1) swap(chs[z], chs[rnd(z+1)]);
                for(ll ch : chs){
                    ll a = reattach(k,ch,-1);
                    if(a < 0){ ok = false; break; }
                    delta += a;
                }
                done = true;
            }
        }else if(r < P_STEINER + P_SWAP){
            auto [b0, y0] = nodes[rnd((ll)nodes.size())];
            ba = b0; sA = BS[b0];
            Ctx k = ctx(b0);
            resetMemo();
            lst.clear();
            rep(y,nn) if(BS[b0].in[y] && y != root && isLeaf(k,y)){ connected(k,y); lst.push_back(y); }
            if(lst.size() >= 2){
                ll a = lst[rnd((ll)lst.size())];
                ll bra = branch[a];
                vll others;
                for(ll y : lst) if(branch[y] != bra) others.push_back(y);
                if(!others.empty()){
                    ll b = others[rnd((ll)others.size())];
                    ll brb = branch[b];
                    delta -= detach(k,a);
                    delta -= detach(k,b);
                    ll ra = (BS[b0].in[brb] && BS[b0].par[brb] == root) ? brb : -1;
                    ll rb = (BS[b0].in[bra] && BS[b0].par[bra] == root) ? bra : -1;
                    ll x1 = reattach(k,a,ra);
                    if(x1 < 0) ok = false;
                    else{
                        delta += x1;
                        ll x2 = reattach(k,b,rb);
                        if(x2 < 0) ok = false;
                        else delta += x2;
                    }
                    done = true;
                }
            }
        }else if(r < P_STEINER + P_SWAP + P_MOVE){
            lst.clear();
            rep(y,nn) if(LB[y] != inf && LB[y] >= 0 && BS[LB[y]].ccnt[y] == 0) lst.push_back(y);
            if(!lst.empty() && BS.size() >= 2){
                ll x = lst[rnd((ll)lst.size())];
                ll a = LB[x];
                ll b = rnd((ll)BS.size() - 1);
                if(b >= a) b++;
                bool valid = true;
                rep(j,(ll)BS.size()) if(j != a && BS[j].in[x] && j <= b) valid = false;
                if(valid){
                    ba = a; bb = b; sA = BS[a]; sB = BS[b];
                    delta -= detach(ctx(a), x);
                    BS[a].in[x] = 0; BS[a].load[x] = 0;
                    delta -= 1;
                    LB[x] = b;
                    BS[b].in[x] = 1; BS[b].load[x] = G[x]; BS[b].ccnt[x] = 0; BS[b].par[x] = -1;
                    delta += 1;
                    ll add = reattach(ctx(b), x, -1);
                    if(add < 0) ok = false;
                    else delta += add;
                    done = true;
                }
            }
        }
        if(!done){
            auto [b0, x] = nodes[rnd((ll)nodes.size())];
            ba = b0; bb = -1; sA = BS[b0];
            delta -= detach(ctx(b0), x);
            ll a = reattach(ctx(b0), x, -1);
            if(a < 0) ok = false;
            else delta += a;
        }
        return true;
    };

    //相乗りの近傍。書き換えた相乗りを戻す用に rideSnap に残す(新しく作ったら pushed)
    ll rideSnapIdx = -1;
    JRide rideSnap;
    bool pushed = false;
    vll sLbB, sLrB, sGw, rl, anyX(nn);
    auto rideMove = [&](ll &delta, bool &ok) -> bool {
        double r = rnd.prob();
        aliveRides(rl);
        if(rl.empty() || r < 0.35){
            //新しい相乗り: Bの葉xから近くのXのノードを乗り場にする
            preCands(lst);
            if(lst.empty()) return false;
            ll x = lst[rnd((ll)lst.size())], L = gB[x];
            fill(all(anyX), 0);
            rep(b,(ll)BX.size()) rep(q,nn) if(BX[b].in[q]) anyX[q] = 1;
            vector<char> used(nn,0);
            rep(r2,(ll)R.size()) rep(q,nn) if(R[r2].pre.in[q] && q != R[r2].u) used[q] = 1;
            fill(all(dist), -1);
            static vll que, tg;
            que.assign(1,x); tg.clear();
            dist[x] = 0;
            for(ll qh = 0; qh < (ll)que.size(); qh++){
                ll u = que[qh];
                if(dist[u] >= JFEED_R) continue;
                ll ui = u / F.n, uj = u % F.n;
                rep(d,4){
                    ll lmax = min(baseB[u] + 1, F.runLen[d][ui][uj]);
                    rep(l,1,lmax+1){
                        ll w = F.idx(ui + DI[d]*l, uj + DJ[d]*l);
                        if(dist[w] != -1) continue;
                        dist[w] = dist[u] + 1;
                        if(anyX[w]){ if(w != rootX && w != rootB) tg.push_back(w); continue; }
                        if(w == rootB || gw[w] > 0 || gX[w] > 0 || used[w] || baseB[w] + L > MAX_H) continue;
                        que.push_back(w);
                    }
                }
            }
            if(tg.empty()) return false;
            ll u = tg[rnd((ll)tg.size())];
            vll bl;
            rep(b,(ll)BX.size()) if(BX[b].in[u]) bl.push_back(b);
            ll bx = bl[rnd((ll)bl.size())];
            vll ws = validWs(bx, u, L);
            if(ws.empty()) return false;
            ll w = ws[rnd((ll)ws.size())];
            takeFromPost(x, delta);
            JRide J;
            J.bx = bx; J.u = u; J.w = w; J.pre = mkEmpty(u);
            R.push_back(J);
            pushed = true;
            ll rid = (ll)R.size() - 1;
            lbB[x] = -1; lrB[x] = rid;
            Batch &P = R[rid].pre;
            P.in[x] = 1; P.load[x] = L; P.ccnt[x] = 0; P.par[x] = -1;
            delta += 1;
            prepareP(rid);
            ll a = reattach(ctxP(rid), x, -1);
            if(a < 0){ ok = false; return true; }
            delta += a;
            if(!addWeight(w, L, delta)) ok = false;
            return true;
        }
        ll rid = rl[rnd((ll)rl.size())];
        rideSnapIdx = rid; rideSnap = R[rid];
        JRide &J = R[rid];
        if(r < 0.55){
            //相乗りにBの葉を足す(preのノードの近くの葉)
            lst.clear();
            rep(q,nn) if(J.pre.in[q]) lst.push_back(q);
            ll q0 = lst[rnd((ll)lst.size())], qi = q0 / F.n, qj = q0 % F.n;
            vll cs;
            preCands(cs);
            vll near;
            for(ll x : cs) if(abs(x / F.n - qi) + abs(x % F.n - qj) <= JFEED_R) near.push_back(x);
            if(near.empty()) return false;
            ll x = near[rnd((ll)near.size())], L = gB[x];
            takeFromPost(x, delta);
            lbB[x] = -1; lrB[x] = rid;
            Batch &P = J.pre;
            P.in[x] = 1; P.load[x] = L; P.ccnt[x] = 0; P.par[x] = -1;
            delta += 1;
            prepareP(rid);
            ll a = reattach(ctxP(rid), x, -1);
            if(a < 0){ ok = false; return true; }
            delta += a;
            if(!addWeight(J.w, L, delta)) ok = false;
            return true;
        }
        if(r < 0.7){
            //相乗りからBの葉を外して、Bのpostの便へ戻す
            lst.clear();
            rep(q,nn) if(lbB[q] == -1 && lrB[q] == rid && J.pre.ccnt[q] == 0) lst.push_back(q);
            if(lst.empty()) return false;
            ll x = lst[rnd((ll)lst.size())], L = gB[x];
            ll b = rnd((ll)BB.size());
            rep(j,b+1) if(BB[j].in[x]) return false;
            delta -= detach(ctxP(rid), x) + 1;
            J.pre.in[x] = 0; J.pre.load[x] = 0;
            touchB(b);
            lbB[x] = b; lrB[x] = -1;
            BB[b].in[x] = 1; BB[b].load[x] = L; BB[b].ccnt[x] = 0; BB[b].par[x] = -1;
            delta += 1;
            ll a = reattach(ctxB(b), x, -1);
            if(a < 0){ ok = false; return true; }
            delta += a;
            if(!addWeight(J.w, -L, delta)) ok = false;
            return true;
        }
        if(r < 0.85){
            //降り場を変える
            ll L = J.pre.load[J.u];
            vll ws = validWs(J.bx, J.u, L);
            vll ws2;
            for(ll w : ws) if(w != J.w) ws2.push_back(w);
            if(ws2.empty()) return false;
            ll w2 = ws2[rnd((ll)ws2.size())], w1 = J.w;
            J.w = w2;
            if(!addWeight(w1, -L, delta) || !addWeight(w2, L, delta)) ok = false;
            return true;
        }
        //preのノードをつなぎ直す
        lst.clear();
        rep(q,nn) if(J.pre.in[q] && q != J.u) lst.push_back(q);
        if(lst.empty()) return false;
        ll q = lst[rnd((ll)lst.size())];
        prepareP(rid);
        delta -= detach(ctxP(rid), q);
        ll a = reattach(ctxP(rid), q, -1);
        if(a < 0) ok = false;
        else delta += a;
        return true;
    };

    double startT = timer.now(), temp = ANNEAL_T0;
    for(ll iter = 0; ; iter++){
        if((iter & 63) == 0){
            double now = timer.now();
            if(now >= deadline) break;
            double prog = (now - startT) / max(1e-9, deadline - startT);
            temp = ANNEAL_T0 * pow(ANNEAL_T1 / ANNEAL_T0, prog);
        }
        ll delta = 0;
        bool ok = true;
        double r = rnd.prob();
        int kind = r < P_JR ? 2 : r < P_JR + P_JB ? 1 : 0;
        if(kind == 2){
            savedBB.clear(); rideSnapIdx = -1; pushed = false;
            sLbB = lbB; sLrB = lrB; sGw = gw;
            if(!rideMove(delta, ok)){
                //何も変えていない(途中で抜けた場合も書き換え前)
                continue;
            }
            if(ok) ok = validate();
        }else{
            if(!stdMove(kind, delta, ok)) continue;
            if(ok && kind == 0 && !R.empty()) ok = validate();
        }
        bool accept = ok && (delta <= 0 || rnd.prob() < exp(-(double)delta / temp));
        if(!accept){
            if(kind == 2){
                for(auto &pr : savedBB) BB[pr.first] = pr.second;
                lbB = sLbB; lrB = sLrB; gw = sGw;
                if(pushed) R.pop_back();
                if(rideSnapIdx >= 0) R[rideSnapIdx] = rideSnap;
            }else{
                vector<Batch> &BS = kind == 0 ? BX : BB;
                if(ba >= 0) BS[ba] = sA;
                if(bb >= 0) BS[bb] = sB;
                (kind == 0 ? lbX : lbB) = slb;
            }
            continue;
        }
        cost += delta;
        //使われなくなった相乗りを消す(番号を詰める)
        {
            bool dead = false;
            for(auto &J : R) if(J.pre.load[J.u] == 0) dead = true;
            if(dead){
                vll nid(R.size(), -1);
                vector<JRide> R2;
                rep(r2,(ll)R.size()) if(R[r2].pre.load[R[r2].u] > 0){ nid[r2] = R2.size(); R2.push_back(R[r2]); }
                R.swap(R2);
                rep(x,nn) if(lrB[x] >= 0) lrB[x] = nid[lrB[x]];
            }
        }
        //最後の便が使われたら、空の便を足しておく
        auto pushEmpty = [&](vector<Batch> &BS, ll root){
            bool lastUsed = false;
            rep(y,nn) if(BS.back().in[y] && y != root){ lastUsed = true; break; }
            if(lastUsed) BS.push_back(mkEmpty(root));
        };
        pushEmpty(BX, rootX);
        pushEmpty(BB, rootB);
        if(cost < bestCost){
            bestCost = cost;
            bestBX = BX;
            bestR = R;
        }
    }

    //最良のXの便をTsに戻す(空の便は除き、相乗りの便番号を付け替える)
    ColorTree tmpl = Ts[0];
    Ts.clear();
    vll remap(bestBX.size(), -1);
    rep(b,(ll)bestBX.size()){
        auto &bt = bestBX[b];
        bool any = false;
        rep(y,nn) if(bt.in[y] && y != rootX){ any = true; break; }
        if(!any) continue;
        remap[b] = Ts.size();
        ColorTree T = tmpl;
        T.par = bt.par; T.parDir = bt.pdir; T.parLen = bt.plen; T.load = bt.load; T.inTree = bt.in;
        T.child.assign(nn, vll());
        rep(x,nn) if(T.par[x] >= 0) T.child[T.par[x]].push_back(x);
        Ts.push_back(T);
    }
    for(auto &J : bestR){
        if(J.pre.load[J.u] == 0 || remap[J.bx] < 0) continue;
        J.bx = remap[J.bx];
        outRides.push_back(J);
    }
}

//Xの便b(木T)を、相乗り rides(bx == b のもの)をつけて運ぶ。成り立たなければ盤面を戻して false
bool execJointBatch(State &s, ll X, ll B, const ColorTree &T, const vector<JRide> &rides, ll b){
    ll nn = F.n * F.n, before = (ll)s.ops.size();
    ll rootB = F.idx(F.nestPos[B].fst, F.nestPos[B].snd);
    vll keepExtra(nn,0), afterExtra(nn,0), convoy(nn,-1), boardR(nn,-1);
    vector<char> first(nn,0);
    bool any = false;
    rep(r,(ll)rides.size()){
        const JRide &J = rides[r];
        if(J.bx != b) continue;
        any = true;
        ll L = J.pre.load[J.u], d = 0, prev = J.u;
        boardR[J.u] = r;
        for(ll p = T.par[J.u]; ; prev = p, p = T.par[p]){
            if(p < 0) return false;
            d++;
            convoy[p] = prev; first[p] = (d & 1);
            if(p == J.w) break;
        }
        if(d & 1){ keepExtra[J.w] = L; if(J.w != rootB) afterExtra[J.w] = L; }
    }
    if(!any) return false;
    bool ok = true;
    auto undoAll = [&](){ while((ll)s.ops.size() > before) s.undo(); };
    //preを帰りがけ順に動かして、Bの一団をuへ集める
    auto runPre = [&](const JRide &J){
        vvll ch(nn);
        rep(q,nn) if(J.pre.in[q] && q != J.u && J.pre.par[q] >= 0) ch[J.pre.par[q]].push_back(q);
        auto dfsP = [&](auto &&self, ll q) -> void {
            for(ll c2 : ch[q]){ self(self, c2); if(!ok) return; }
            if(q == J.u) return;
            ll qi = q / F.n, qj = q % F.n, m = topRun(s, qi, qj, B);
            if(m != J.pre.load[q] || !s.tryDo(Action(qi, qj, s.t[q].h - m, J.pre.pdir[q], J.pre.plen[q]))) ok = false;
        };
        dfsP(dfsP, J.u);
    };
    auto dfs = [&](auto &&self, ll u) -> void {
        vll chs = T.child[u];
        if(convoy[u] >= 0){
            //相乗りの子を、奇数手目なら最初に、偶数手目なら最後に着地させる
            chs.erase(find(all(chs), convoy[u]));
            if(first[u]) chs.insert(chs.begin(), convoy[u]);
            else chs.push_back(convoy[u]);
        }
        for(ll ch : chs){ self(self, ch); if(!ok) return; }
        if(boardR[u] >= 0){ runPre(rides[boardR[u]]); if(!ok) return; }
        if(u == T.root) return;
        ll ui = u / F.n, uj = u % F.n;
        ll keep = T.base[u] + keepExtra[u];
        if(s.t[u].h <= keep || !s.tryDo(Action(ui, uj, keep, T.parDir[u], T.parLen[u]))) ok = false;
    };
    dfs(dfs, T.root);
    //Xの一団が全員出発し、降りたBが残っているか
    if(ok){
        rep(u,nn){
            if(!T.inTree[u] || u == T.root) continue;
            if(s.t[u].h != T.base[u] + afterExtra[u]){ ok = false; break; }
            if(afterExtra[u] > 0 && topRun(s, u / F.n, u % F.n, B) < afterExtra[u]){ ok = false; break; }
        }
    }
    if(!ok){ undoAll(); return false; }
    return true;
}

//------------------------------ 色ごとの厳密DP ------------------------------
//焼きなましと同じモデル(base固定・飛距離base+1・base+load<=8・手数=巣以外のノード数)を、
//色cのスライムのマスが DP_TMAX 個以下のとき Dreyfus-Wagner 型のDPで最適に解く。
//  dp[S][v] = 葉の集合Sの全員をマスvに集める最小手数(vに集まれるのは base[v]+|S|<=8 のときだけ)
//  合流: dp[S][v] = dp[A][v] + dp[S\A][v]、 移動: dp[S][w] = dp[S][v] + 1 (vからwへ1手で跳べる)
//  巣では容量を気にせず合流できるので、最後に葉全体を巣へ着く一団に分割する。
//着地の制約(まだ出発していない葉のマスに着地しない)は緩めているので、復元した手順を実際に指して確かめる。

const ll DP_TMAX = 14;   //DPで解く色のマス数の上限(調整ノブ)
const ll DP_FAST = 12;   //このマス数以下なら時間を気にせずDPする(12マスで最大75ms程度)
const double DP_MS_AT_FAST = 80;   //DP_FASTマスのDPにかかる時間の目安(ms)。1マス増えるごとに約2.2倍

//色cをDPの手順で巣へ運ぶ。全員帰巣できたらtrue。できなければ盤面を元に戻してfalse
bool solveColorDP(State &s, ll c, double deadline){
    ll nn = F.n * F.n;
    auto [gi,gj] = F.nestPos[c];
    ll root = F.idx(gi,gj);
    vll base(nn,0), g(nn,0), terms;
    rep(x,nn){
        if(!F.isFloor[x / F.n][x % F.n]) continue;
        g[x] = topRun(s, x / F.n, x % F.n, c);
        base[x] = s.t[x].h - g[x];
        if(x != root && g[x] > 0) terms.push_back(x);
    }
    ll t = terms.size();
    if(t == 0 || t > DP_TMAX) return false;
    if(t > DP_FAST && timer.now() + DP_MS_AT_FAST * pow(2.2, t - DP_FAST) > deadline) return false;   //時間が足りない

    //1手の移動 v -> w (巣への移動は toRoot に分ける)
    vvll adj(nn);
    vector<char> toRoot(nn,0);
    rep(v,nn){
        ll vi = v / F.n, vj = v % F.n;
        if(!F.isFloor[vi][vj] || v == root) continue;
        rep(d,4){
            ll lmax = min(base[v] + 1, F.runLen[d][vi][vj]);
            rep(l,1,lmax+1){
                ll w = F.idx(vi + DI[d]*l, vj + DJ[d]*l);
                if(w == root) toRoot[v] = 1;
                else adj[v].push_back(w);
            }
        }
    }

    const int INF = 1e9;
    ll full = (1LL << t) - 1;
    vll W(full+1,0);   //集合の匹数
    rep(S,1,full+1) W[S] = W[S & (S-1)] + g[terms[__builtin_ctzll(S)]];
    //from[S][v]: -1=葉そのもの, 0..nn-1=そのマスから跳んできた, nn+A=AとS\Aがvで合流
    vector<vector<int>> dp(full+1), from(full+1);
    vll arrive(full+1, INF), arrFrom(full+1, -1);   //Sが一団で巣に着く最小手数と、跳ぶ前のマス
    vector<vector<int>> bucket;
    rep(S,1,full+1){
        if(W[S] > MAX_H) continue;
        auto &cur = dp[S];
        auto &fr = from[S];
        cur.assign(nn, INF);
        fr.assign(nn, -2);
        if((S & (S-1)) == 0){ ll x = terms[__builtin_ctzll(S)]; cur[x] = 0; fr[x] = -1; }
        ll low = S & -S;
        for(ll A = (S-1) & S; A > 0; A = (A-1) & S){
            if(!(A & low)) continue;
            ll B = S ^ A;
            if(dp[A].empty() || dp[B].empty()) continue;
            const auto &da = dp[A], &db = dp[B];
            rep(v,nn) if(da[v] < INF && db[v] < INF && da[v] + db[v] < cur[v]){ cur[v] = da[v] + db[v]; fr[v] = (int)(nn + A); }
        }
        rep(v,nn) if(base[v] + W[S] > MAX_H) cur[v] = INF;
        //初期値つきの多始点BFS(距離ごとのバケット)
        int mx = 0;
        rep(v,nn) if(cur[v] < INF) chmax(mx, cur[v]);
        bucket.assign(mx + nn + 2, {});
        rep(v,nn) if(cur[v] < INF) bucket[cur[v]].push_back(v);
        rep(dd,(ll)bucket.size()){
            for(size_t z = 0; z < bucket[dd].size(); z++){
                int v = bucket[dd][z];
                if(cur[v] != dd) continue;
                for(ll w : adj[v]){
                    if(base[w] + W[S] > MAX_H || cur[w] <= dd + 1) continue;
                    cur[w] = dd + 1; fr[w] = v;
                    bucket[dd+1].push_back(w);
                }
            }
        }
        if(base[root] + W[S] <= MAX_H){
            rep(v,nn) if(toRoot[v] && cur[v] < INF && cur[v] + 1 < arrive[S]){ arrive[S] = cur[v] + 1; arrFrom[S] = v; }
        }
    }
    //葉全体を、巣へ着く一団に分割する
    vll R(full+1, INF), Rch(full+1, -1);
    R[0] = 0;
    rep(S,1,full+1){
        ll low = S & -S;
        for(ll A = S; A > 0; A = (A-1) & S){
            if(!(A & low) || arrive[A] >= INF || R[S^A] >= INF) continue;
            if(arrive[A] + R[S^A] < R[S]){ R[S] = arrive[A] + R[S^A]; Rch[S] = A; }
        }
    }
    if(R[full] >= INF) return false;

    //復元: 集合Sをマスvに集める手順を帰りがけ順に並べる
    vpll jumps;   //(出発マス, 着地マス)
    auto gen = [&](auto &&self, ll S, ll v) -> void {
        int f = from[S][v];
        if(f == -1) return;
        if(f >= nn){ ll A = f - nn; self(self, A, v); self(self, S ^ A, v); return; }
        self(self, S, f);
        jumps.push_back({f, v});
    };
    for(ll S = full; S > 0; S ^= Rch[S]){
        ll A = Rch[S];
        gen(gen, A, arrFrom[A]);
        jumps.push_back({arrFrom[A], root});
    }

    //実際に指して確かめる。だめなら元に戻す
    ll before = s.ops.size();
    bool ok = true;
    for(auto [u,v] : jumps){
        ll ui = u / F.n, uj = u % F.n, vi = v / F.n, vj = v % F.n;
        ll d = vi < ui ? 0 : vi > ui ? 1 : vj < uj ? 2 : 3;
        ll l = abs(vi - ui) + abs(vj - uj);
        ll m = topRun(s, ui, uj, c);
        if(m == 0) continue;   //一団がもういない(先に別の一団に拾われた)
        if(!s.tryDo(Action(ui,uj,s.height(ui,uj)-m,d,l))){ ok = false; break; }
    }
    if(ok) rep(x,nn) rep(y,s.t[x].h) if(s.t[x].at(y) == c){ ok = false; break; }
    if(!ok) while((ll)s.ops.size() > before) s.undo();
    return ok;
}

//合流木方式: 色をorderの順に1色ずつ、木を作って巣へ運ぶ。
//その色で木につなげなかったスライムは、木を作り直して運ぶ(動けなくなるまで)。
//annealEndが正なら、各色の最初は便の列を作り(buildPlan)、その色のスライム数に比例した時間だけ
//焼きなましてから(annealEnd(ms)まで)、便を順に運ぶ。
void runTree(State &s, const vll &order, double annealEnd){
    static ColorTree T;
    static vector<ColorTree> Ts;
    static vector<JRide> rides;
    ll nn = F.n * F.n;
    vll later(F.k);
    vector<char> noReserve(nn,0), reserved(nn);
    bool progress = true;
    while(!s.done() && progress){
        progress = false;
        rep(z,(ll)order.size()){
            ll c = order[z];
            //乗客にできるのは、この周回でcより後に処理する色
            rep(e,F.k) later[e] = -1;
            rep(y,z+1,(ll)order.size()) later[order[y]] = y;
            bool first = true;
            while(true){
                if((ll)s.ops.size() >= MAX_T || timer.now() > TL) return;
                ll before = (ll)s.ops.size();
                if(first && annealEnd > 0){
                    //まずDPで解く。解けなければ焼きなます
                    if(!solveColorDP(s, c, annealEnd)){
                        buildPlan(s, c, Ts);
                        //焼きなましの時間は、焼きなます色(DP_TMAXマスより多い色と、この色)の匹数に比例して配る
                        vll cnt(F.k,0), cells(F.k,0);
                        rep(x,F.n*F.n){
                            rep(y,s.t[x].h) cnt[s.t[x].at(y)]++;
                            if(s.t[x].h > 0) cells[s.t[x].top()]++;
                        }
                        ll rest = 0;
                        rep(e,F.k) if(e == c || cells[e] > DP_TMAX) rest += cnt[e];
                        double now = timer.now();
                        double share = (rest > 0) ? (double)cnt[c] / rest : 0;
                        double dl = now + max(0.0, annealEnd - now) * share;
                        //次の色がいれば、その色との相乗りつきで焼きなます
                        ll nb = (z + 1 < (ll)order.size()) ? order[z+1] : -1;
                        rides.clear();
                        if(JOINT_ON && nb >= 0) annealJoint(s, c, nb, Ts, rides, later, dl);
                        else annealPlan(s, c, Ts, dl);
                        //後の便のノードと、後の便の相乗りのpreのマスは、乗客が着地・降車しないように予約しておく。
                        //後の便の相乗りで運ぶBは乗客にしない
                        vector<char> noPick(nn);
                        rep(b,(ll)Ts.size()){
                            rep(x,nn) reserved[x] = 0, noPick[x] = 0;
                            rep(b2,b+1,(ll)Ts.size()) rep(x,nn) if(Ts[b2].inTree[x]) reserved[x] = 1;
                            for(auto &J : rides) if(J.bx > b) rep(x,nn) if(J.pre.in[x] && x != J.u) reserved[x] = noPick[x] = 1;
                            bool hasR = false;
                            for(auto &J : rides) if(J.bx == b) hasR = true;
                            if(hasR) DBG_PLAN++;
                            if(execJointBatch(s, c, nb, Ts[b], rides, b)){ DBG_OK++; continue; }
                            if(hasR) DBG_NG++;
                            execTreeRide(s, c, Ts[b], later, reserved, &noPick);
                        }
                    }
                }else{
                    buildTree(s, c, T);
                    execTreeRide(s, c, T, later, noReserve);
                }
                first = false;
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
    //  GREEDY_TLまでは(1)(2)を乱数(色の順・BFSの方向順)を変えて独立に繰り返し、最良の色の順を選ぶ。
    //  その順で、各色の木を焼きなましてから(1)(2)を行い、良い方の操作列を出力する
    State best;
    ll bestScore = inf, trials = 0;
    vll order(F.k), bestOrder;
    iota(all(order), 0);
    do{
        mrep(x,F.k-1,1) swap(order[x], order[rnd(x+1)]);
        State s;
        s.init();
        runTree(s, order, -1);
        finishRest(s);
        trials++;
        if(s.score() <= bestScore){ bestScore = s.score(); best = s; bestOrder = order; }
    }while(timer.now() < GREEDY_TL);

    //最良の色の順で、各色の木を焼きなましてから運ぶ
    ll greedyScore = bestScore;
    {
        State s;
        s.init();
        runTree(s, bestOrder, TRIAL_TL);
        finishRest(s);
        if(s.score() <= bestScore){ bestScore = s.score(); best = s; }
    }

#ifdef LOCAL
    cerr << "N=" << F.n << " K=" << F.k << " slime=" << F.slimeNum
         << " colorNum=" << F.colorNum << "\n";
    cerr << best.dump();
    cerr << "time=" << timer.now() << "ms\n";
    //試行回数と最良の手数・残り
    cerr << "TRIALS trials=" << trials << " greedy=" << greedyScore
         << " T=" << best.ops.size() << " E=" << best.remain << "\n";
    cerr << "JOINT plan=" << DBG_PLAN << " ok=" << DBG_OK << " ng=" << DBG_NG << endl;
#else
    (void)trials; (void)greedyScore;
#endif

    best.print(cout);
    return 0;
}
