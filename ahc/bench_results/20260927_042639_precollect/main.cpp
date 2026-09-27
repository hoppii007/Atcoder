//============================================================================
// AHC072 A - Somersault Slimes  解法の方針 (precollect)
//----------------------------------------------------------------------------
// 左上から順に、一番上のスライムを自分の巣まで運ぶ(carryTop)。
//   前半(collectNearby): 巣へ向かう前に、COLLECT_R=3 マス以内の一番近い同色へ
//     寄っていき、8匹になるか近くに同色がいなくなるまで回収する。
//   後半: ルートDP(巣に1マスずつ近づく道で回収数最大)に沿って巣へ運ぶ。
// 運ぶ操作は常に飛距離1。
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

const ll NG = -1;        //そのルートでは巣まで行けない
const ll COLLECT_R = 3;  //出発前に同色を回収しに行く距離の上限(調整ノブ)

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

    //各マスで回収できる数 = 一番上から連続する色cの数
    static vvll gain;
    gain.assign(F.n, vll(F.n, 0));
    rep(i,F.n) rep(j,F.n) if(F.isFloor[i][j]) gain[i][j] = topRun(s,i,j,c);

    //val[m][i][j] = m匹運んでマス(i,j)にいるとき、以降で追加回収できる最大数
    //nxt[m][i][j] = そのとき進む方向
    //着地点の高さ制限があるので運んでいる匹数mを状態に持つ。
    //巣からの距離が小さい順に決めればよい(1マスずつ近づく道だけを使うため)。
    static vector<vvll> val, nxt;
    val.assign(MAX_H+1, vvll(F.n, vll(F.n, NG)));
    nxt.assign(MAX_H+1, vvll(F.n, vll(F.n, -1)));

    vector<vpll> byDist(F.n*F.n + 1);
    rep(i,F.n) rep(j,F.n){
        if(F.isFloor[i][j] && dist[i][j] >= 0) byDist[dist[i][j]].push_back({i,j});
    }
    rep(m,1,MAX_H+1) val[m][gi][gj] = 0;  //巣に着いたら終わり
    rep(D,1,F.n*F.n+1){
        for(auto [i,j] : byDist[D]){
            rep(m,1,MAX_H+1){
                ll best = NG, bd = -1;
                rep(d,4){
                    ll ni = i + DI[d], nj = j + DJ[d];
                    if(!F.ok(ni,nj) || dist[ni][nj] != D-1) continue;
                    if(s.height(ni,nj) + m > MAX_H) continue;   //着地できない
                    ll mu = m + gain[ni][nj];                   //着地後に持つ匹数
                    if(val[mu][ni][nj] == NG) continue;
                    if(chmax(best, gain[ni][nj] + val[mu][ni][nj])) bd = d;
                }
                val[m][i][j] = best;
                nxt[m][i][j] = bd;
            }
        }
    }

    //持ち上げる匹数を決める。回収総数が最大のものを選び、同数なら多く持つ方。
    //(一番上の連続分を全部持つと高さ制限で進めない場合があるので少なくする余地を残す)
    ll m0 = 0, bestTotal = -1;
    rep(m,1,gain[ci][cj]+1){
        if(val[m][ci][cj] == NG) continue;
        ll total = m + val[m][ci][cj];
        if(total > bestTotal || (total == bestTotal && m > m0)){ bestTotal = total; m0 = m; }
    }
    if(m0 == 0) return moved;  //巣まで行けるルートが無い

    //決めたルートに沿って運ぶ
    ll m = m0;
    while(ci != gi || cj != gj){
        if((ll)s.ops.size() >= MAX_T) break;
        if(timer.now() > TL) break;
        ll d = nxt[m][ci][cj];
        if(d < 0) break;
        ll h = s.height(ci,cj);
        if(h < m) break;
        if(!s.tryDo(Action(ci,cj,h-m,d,1))) break;
        moved = true;
        ci += DI[d]; cj += DJ[d];
        m = topRun(s,ci,cj,c);          //着地点の同色を回収した分だけ増える
        if(m <= 0 || m > MAX_H) break;
    }
    return moved;
}

//============================================================================

int main(){
    timer.start();
    cin.tie(nullptr);
    ios::sync_with_stdio(false);

    F.read(cin);
    State s;
    s.init();

    //解法: 左上から順に、一番上にいるスライムを自分の巣まで帰していく
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
#endif

    s.print(cout);
    return 0;
}
