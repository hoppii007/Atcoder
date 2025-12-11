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

const ll mod = 998244353LL;
//const ll mod = 1000000007LL;
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


/////////////// 全体での決まり ////////////////
// ・木の場所は1, それ以外は0とする
// ・レールを敷く感じで、まっすぐ進ませるのを基本方針とする
// ・ラフマップ：3*3で空いている時は、真ん中を塞ぐ（視界妨害）
// ・最長ルートを作った後、穴あきにする（5%）
// ・本当はゴールに近いところは穴あきにしない、とかにしたほうがいい
// ・方針を大きく変えた。プレイヤーの視界を遮りつつ、花から遠ざけるようにした
// ・花から遠ざける要素あんまないけど、最終的に花を守る機構を作ったらめちゃ強くなった
// ・花から遠ざけるのはやっぱマストにしたら平均900超えた
// ・ランダム伸ばしはギザギザになるように、なめらかではなくrand()*2とか*3とかで伸ばすようにした
// ・距離1カットができなかった場合穴ができるから、それを防ぐようにした（距離1以上で最初に塞げるところを塞ぐようにした。なお、それ以前に木があれば何もしない）
// ・ゴールまでの距離がN/4以下の時、N/4より遠ざけることができるなら、遠ざける

struct Pos {
    ll i, j;
    Pos():i(0), j(0) {}
    Pos(ll i, ll j):i(i), j(j) {}
    bool operator==(const Pos& p) const {
        return tie(i,j) == tie(p.i, p.j);
    }
    bool operator!=(const Pos& p) const {
        return tie(i,j) != tie(p.i, p.j);
    }
    bool operator<(const Pos& p) const {
        return tie(i,j) < tie(p.i, p.j);
    }
};


//ミリ秒で経過時間を取得
class TimeKeeper {
public:
    chrono::system_clock::time_point  startPoint, endPoint;
    void start(){
        startPoint = chrono::system_clock::now();
    }
    double now(){
        endPoint = chrono::system_clock::now();
        double elapsed = chrono::duration_cast<std::chrono::milliseconds>(endPoint-startPoint).count();
        return elapsed;
    }
};


// グローバル変数
bool is_test = true;
ll N;
Pos T, P;
Pos sT; // 目の前の目標
vvb tentative_map; // 未確認の場所は全てfalseにしているマップ
vvb tree_map; // 木があるところは1
vvb checked_map; // 確認済みのところは1
const ll UP = 0;
const ll DOWN = 1;
const ll RIGHT = 2;
const ll LEFT = 3;
// 移動方向
vector<Pos> dirs = {{1,0},{-1,0},{0,1},{0,-1}};


//縦0以上h未満、横0以上w未満の(i,j)の4近傍をとる
vector<Pos> kinbo4(Pos p){
    vector<Pos> ret;
    ll i = p.i;
    ll j = p.j;
    if(j+1 < N) ret.push_back({i,j+1});
    if(i+1 < N) ret.push_back({i+1,j});
    if(j-1 >= 0) ret.push_back({i,j-1});
    if(i-1 >= 0) ret.push_back({i-1,j});
    return ret;
}


// a以上b未満の乱数を返す
std::mt19937_64 rndmt(std::random_device{}());
ll randll(ll a, ll b){
    std::uniform_int_distribution<ll> r(a, b-1);
    return r(rndmt);
}


// 座標変換関連
ll to_hash(Pos p){ return p.i*N + p.j; }
Pos to_pos(ll h){ return Pos{h/N, h%N}; }


ll manhat(Pos p1, Pos p2){ return abs(p1.i-p2.i) + abs(p1.j-p2.j); }


// 座標sからgまでの、暫定地図での最短経路計算（辿り着けない場合はinf）
vll min_dists(vector<Pos> starts, vvb &wall_map){
    vll dist(N*N, inf);
    deque<pll> q;
    for(Pos s: starts){
        dist[to_hash(s)] = 0;
        q.push_front({0, to_hash(s)});
    }
    
    while(!q.empty()){
        pll now = q.front();
        q.pop_front();
        Pos p = to_pos(now.snd);
        ll h = now.snd;
        ll c = now.fst;
        //行ける場所全てに対して
        for(Pos nxt: kinbo4(p)) if(!wall_map[nxt.i][nxt.j]){
            ll nc = dist[h] + 1;
            ll nh = to_hash(nxt);
            if(dist[nh] <= nc) continue;
            dist[nh] = nc;
            q.push_back({nc, nh});
        }
    }
    return dist;
}


// PとTは避けつつ、木をランダムで設置する
vvb make_random_tree_map(){
    vvb ret(N, vb(N));
    rep(i,N) rep(j,N) {
        if(randll(0,100) < 50 && Pos{i,j} != P && Pos{i,j} != T && !checked_map[i][j]) ret[i][j] = true;
        else ret[i][j] = tree_map[i][j];
    }
    return ret;
}


////////////// プレイヤー移動系 ///////////////
ll get_next_direction(){
    ll ret = UP;
    ll min_d = inf;
    vll dists = min_dists({sT}, tentative_map);
    if(P.i-1 >= 0 && dists[to_hash({P.i-1, P.j})] < min_d){
        ret = UP;
        min_d = dists[to_hash({P.i-1, P.j})];
    }
    if(P.i+1 < N && dists[to_hash({P.i+1, P.j})] < min_d){
        ret = DOWN;
        min_d = dists[to_hash({P.i+1, P.j})];
    }
    if(P.j-1 >= 0 && dists[to_hash({P.i, P.j-1})] < min_d){
        ret = LEFT;
        min_d = dists[to_hash({P.i, P.j-1})];
    }
    if(P.j+1 < N && dists[to_hash({P.i, P.j+1})] < min_d){
        ret = RIGHT;
        min_d = dists[to_hash({P.i, P.j+1})];
    }
    return ret;
}

void update_checkedmap(){
    // 上方向に確認マップを伸ばす
    ll i = P.i;
    ll j = P.j;
    checked_map[i][j] = true;
    while(i-1 >= 0 && !tree_map[i-1][j]) checked_map[--i][j] = true;
    if(i-1 >= 0) checked_map[i-1][j] = true;

    // 下方向に確認マップを伸ばす
    i = P.i;
    while(i+1 < N && !tree_map[i+1][j]) checked_map[++i][j] = true;
    if(i+1 < N) checked_map[i+1][j] = true;

    // 右方向に確認マップを伸ばす
    i = P.i;
    j = P.j;
    while(j+1 < N && !tree_map[i][j+1]) checked_map[i][++j] = true;
    if(j+1 < N) checked_map[i][j+1] = true;

    // 左方向に確認マップを伸ばす
    i = P.i;
    j = P.j;
    while(j-1 >= 0 && !tree_map[i][j-1]) checked_map[i][--j] = true;
    if(j-1 >= 0) checked_map[i][j-1] = true;

    rep(ii,N) rep(jj,N) tentative_map[ii][jj] = (checked_map[ii][jj] && tree_map[ii][jj]);
}
////////////////////////////////////////////////////


/////////////// 最長経路を求めるやつ //////////////////
void dfs_generate(Pos cur, Pos goal, vvb& wall_map, vvb& visited, vvb& used) {
    if (cur == goal) return; // ゴール到達

    // ランダム順に近傍を試す
    vector<Pos> neighbors = kinbo4(cur);
    shuffle(neighbors.begin(), neighbors.end(), rndmt);

    for (Pos& nxt : neighbors) {
        if(wall_map[nxt.i][nxt.j]) continue; // 通れない時はスキップ
        if(visited[nxt.i][nxt.j]) continue; // 訪問済みスキップ
        // used_trueに3以上摂生ていたらスキップ
        ll cnt = 0;
        for(Pos& nbr: kinbo4(nxt)) cnt += used[nbr.i][nbr.j];
        if(cnt >= 2) continue;

        used[nxt.i][nxt.j] = true;
        visited[nxt.i][nxt.j] = true;
        dfs_generate(nxt, goal, wall_map, visited, used);
        if(used[goal.i][goal.j]) return;
        used[nxt.i][nxt.j] = false;
    }

    // 行き止まりだった
    return;
}

// スタートからランダムDFSで初期解を作成する
vvb generate_random_path_map(Pos start, Pos goal, vvb& wall_map) {
    vvb used(N, vb(N, false));
    vvb visited(N, vb(N, false));
    used[start.i][start.j] = true;
    visited[start.i][start.j] = true;
    dfs_generate(start, goal, wall_map, visited, used);
    return used;
}
///////////////////////////////////////////////////


/////////////// 最短経路を求める /////////////////////
vvb generate_nearest_path_map(Pos start, Pos goal, vvb& wall_map){
    vll dists = min_dists({goal}, wall_map);
    vvb used(N, vb(N, false));
    Pos now_p = start;
    used[now_p.i][now_p.j] = true;
    while(now_p != goal){
        for(Pos nxt_p: kinbo4(now_p)) {
            if(dists[to_hash(now_p)] > dists[to_hash(nxt_p)]){
                used[nxt_p.i][nxt_p.j] = true;
                now_p = nxt_p;
                break;
            }
        }
    }
    return used;
}
//////////////////////////////////////////////////


// 視界を阻害するため、ラフマップを作成する。
vvb make_rough_map(vvb& use_map, vvb wall_map){
    rep(i,N-2) rep(j,N-2) if(!use_map[i][j] && !wall_map[i][j]) {
        if(wall_map[i][j]) continue;
        if(wall_map[i][j+1]) continue;
        if(wall_map[i][j+2]) continue;
        if(wall_map[i+1][j]) continue;
        if(wall_map[i+1][j+1]) continue;
        if(wall_map[i+1][j+2]) continue;
        if(wall_map[i+2][j]) continue;
        if(wall_map[i+2][j+1]) continue;
        if(wall_map[i+2][j+2]) continue;
        wall_map[i+1][j+1] = true;
    }
    return wall_map;
}

// i,jに置いても、まだゴールに辿り着けるか？
bool can_put(ll i, ll j){
    if(i < 0 || N <= i || j < 0 || N <= j) return false;
    if(tree_map[i][j]) return false;
    if(checked_map[i][j]) return false;
    if(Pos{i,j} == T) return false;
    tree_map[i][j] = true;
    ll d = 0;
    vll dists = min_dists({P}, tree_map);
    rep(i,N) rep(j,N) if(!tree_map[i][j]) chmax(d, dists[to_hash({i,j})]);
    tree_map[i][j] = false;
    return d != inf;
}


// 最短経路の一例を復元
vector<Pos> reconstruct_path(Pos P, Pos T, const vvb& tree_map) {
    int H = tree_map.size();
    int W = tree_map[0].size();
    vector<vector<Pos>> parent(H, vector<Pos>(W, {-1,-1}));
    vector<vector<bool>> vis(H, vector<bool>(W,false));
    queue<Pos> q;
    q.push(P);
    vis[P.i][P.j]=true;
    while(!q.empty()){
        Pos cur=q.front();q.pop();
        if(cur==T) break;
        for(auto dpos:dirs){
            ll ni=cur.i+dpos.i,nj=cur.j+dpos.j;
            if(ni<0||ni>=H||nj<0||nj>=W) continue;
            if(tree_map[ni][nj]) continue;
            if(vis[ni][nj]) continue;
            vis[ni][nj]=true;
            parent[ni][nj]=cur;
            q.push({ni,nj});
        }
    }
    vector<Pos> path;
    if(parent[T.i][T.j].i!=-1){
        for(Pos cur=T; !(cur==Pos{-1,-1}); cur=parent[cur.i][cur.j]){
            path.push_back(cur);
            if(cur==P) break;
        }
    }
    return path;
}

// 木を置く場所を探す
vector<Pos> find_tree_positions(vvb& tree_map, Pos P, Pos T) {
    // 元の最短距離
    vll dists = min_dists({T}, tree_map);
    ll d0 = dists[to_hash(P)];
    if (d0 > N/4) return {}; // 条件外

    int H = tree_map.size();
    int W = tree_map[0].size();

    // 候補は最短経路 + 隣接
    vector<Pos> path = reconstruct_path(P,T,tree_map);
    vector<Pos> candidates;
    auto add_candidate=[&](ll i,ll j){
        if(i<0||i>=H||j<0||j>=W) return;
        if(tree_map[i][j]) return;
        candidates.push_back({i,j});
    };
    for(auto p : path){
        add_candidate(p.i,p.j);
        for(auto dpos:dirs) add_candidate(p.i+dpos.i, p.j+dpos.j);
    }
    sort(candidates.begin(), candidates.end());
    candidates.erase(unique(candidates.begin(), candidates.end()), candidates.end());

    // 1本置き
    for(auto c : candidates){
        tree_map[c.i][c.j]=true;
        vll dists2=min_dists({T},tree_map);
        ll d1=dists2[to_hash(P)];
        bool is_ok = can_put(c.i, c.j);
        tree_map[c.i][c.j]=false;
        if(d1>=N/4+d0 && is_ok) return {c};
    }

    // 2本置き
    for(int i=0;i<(int)candidates.size();i++){
        for(int j=i+1;j<(int)candidates.size();j++){
            Pos c1=candidates[i], c2=candidates[j];
            tree_map[c1.i][c1.j]=true;
            tree_map[c2.i][c2.j]=true;
            vll dists2=min_dists({T},tree_map);
            ll d2=dists2[to_hash(P)];
            tree_map[c1.i][c1.j]=false;
            tree_map[c2.i][c2.j]=false;
            bool is_ok = false;
            if(can_put(c1.i, c1.j)){
                tree_map[c1.i][c1.j] = true;
                if(can_put(c2.i, c2.j)){
                    tree_map[c2.i][c2.j] = true;
                    is_ok = true;
                    tree_map[c2.i][c2.j] = false;
                }
                tree_map[c1.i][c1.j] = false;
            }
            if(d2>=N/4+d0 && is_ok) return {c1,c2};
        }
    }
    return {}; // 見つからない
}


Pos cut_r(vvb& tree_map, ll Pi, ll Pj, ll di, ll dj, ll r){
    bool blocked = false;
    for(int k = 0; k < r; k++){ // 距離1～r-1に木があるかチェック
        ll xi = Pi + di*k;
        ll xj = Pj + dj*k;
        if(xi < 0 || xi >= N || xj < 0 || xj >= N) return Pos{-1, -1};
        if(tree_map[xi][xj]) return Pos{-1, -1};
    }
    
    for(int k = r; ; k++){
        ll xi = Pi + di*k;
        ll xj = Pj + dj*k;
        if(xi < 0 || xi >= N || xj < 0 || xj >= N) break;
        if(tree_map[xi][xj]) break;
        if(can_put(xi,xj)) return Pos{xi, xj};
    }
    return Pos{-1, -1};
}

vpll block_and_cut_sight(vvb &tree_map, vvb &checked_map, ll Ti, ll Tj, ll Pi, ll Pj) {

    vll dist = min_dists({Pos{Ti,Tj}}, tree_map);
    vpll diffs;
    
    vector<Pos> longer_path = find_tree_positions(tree_map, P, T);
    for(Pos p: longer_path){
        tree_map[p.i][p.j] = true;
        diffs.push_back({p.i, p.j});
    }

    // --- 距離1カット & 距離rカット ---
    for(auto [di,dj] : dirs){
        ll ni = Pi + di;
        ll nj = Pj + dj;

        ll r = 1;
        if(randll(0, 100) < 20) r = 3;
        Pos p1 = cut_r(tree_map, Pi, Pj, di, dj, r);
        if(p1.i != -1){
            tree_map[p1.i][p1.j] = true;
            diffs.push_back({p1.i, p1.j});
        }
        

    }

    return diffs;
}


vpll enforce_turn_near_goal(vvb &tree_map, ll Ti, ll Tj) {
    vector<pair<ll,ll>> dirs = {{-1,0},{1,0},{0,-1},{0,1}};
    vpll diffs;
    //return diffs;

    // どの方向を「入口」とするか（例: ランダムで選ぶ）
    int entry_dir = randll(0, 4);
    auto [di, dj] = dirs[entry_dir];

    // 入口側の隣マス（ここは残す）
    ll ei = Ti + di;
    ll ej = Tj + dj;

    // 入口方向の2マス目は塞ぐ（直進防止）
    ll bi = Ti + di * 2;
    ll bj = Tj + dj * 2;
    if(0 <= bi && bi < N && 0 <= bj && bj < N && can_put(bi,bj)) {
        tree_map[bi][bj] = true;
        diffs.push_back({bi,bj});
    }

    // 曲がるルートを作る
    // 上下の場合は左右に、左右の場合は上下に逃げ道を用意
    if(di != 0){ // 縦方向入口 → 横に曲がる
        // 左右を候補にしてランダムに1つを開ける
        vector<pair<ll,ll>> turns = {{ei, ej-1}, {ei, ej+1}};
        int open_idx = randll(0,1);
        for(int k=0;k<2;k++){
            ll ti = turns[k].first;
            ll tj = turns[k].second;
            if(ti < 0 || ti >= N || tj < 0 || tj >= N) continue;
            if(tree_map[ti][tj]) continue;
            if(k == open_idx) {
                // ここは通路にするので残す
            } else {
                if(can_put(ti,tj)){
                    tree_map[ti][tj] = true;
                    diffs.push_back({ti,tj});
                }
            }
        }
    } else { // 横方向入口 → 縦に曲がる
        vector<pair<ll,ll>> turns = {{ei-1, ej}, {ei+1, ej}};
        int open_idx = randll(0,1);
        for(int k=0;k<2;k++){
            ll ti = turns[k].first;
            ll tj = turns[k].second;
            if(ti < 0 || ti >= N || tj < 0 || tj >= N) continue;
            if(tree_map[ti][tj]) continue;
            if(k == open_idx) {
                // 開ける
            } else {
                if(can_put(ti,tj)){
                    tree_map[ti][tj] = true;
                    diffs.push_back({ti,tj});
                }
            }
        }
    }

    // 残り3方向はゴール隣を全部塞ぐ
    for(int d=0; d<4; d++){
        if(d == entry_dir) continue;
        auto [odi, odj] = dirs[d];
        ll ni = Ti + odi;
        ll nj = Tj + odj;
        if(ni < 0 || ni >= N || nj < 0 || nj >= N) continue;
        if(tree_map[ni][nj]) continue;
        if(can_put(ni,nj)){
            tree_map[ni][nj] = true;
            diffs.push_back({ni,nj});
        }
    }

    return diffs;
}



int main(){
    // 入力
    {
        // 各変数の設定
        cin >> N >> T.i >> T.j;
        P = {0, N/2};
        if(is_test){
            sT = {-1,-1};
        }

        // 木マップの入力
        tree_map.resize(N, vb(N, false));
        rep(i,N){
            cinstr(s);
            rep(j,N) tree_map[i][j] = (s[j] == 'T');
        }
        // 暫定マップの初期化
        tentative_map.resize(N, vb(N, false));

        // 確認マップの初期化
        checked_map.resize(N, vb(N, false));
        checked_map[P.i][P.j] = true;
    }

    // DFSで、初期値を一つ生成する
    vvb random_path_map = generate_random_path_map(P, T, tree_map);

    // ゲームループ
    ll turn = 0;
    vll directions;
    while(true){

        // 本番は、プレイヤーの現在いる場所と、暫定マップのセットが入力される
        if(is_test == false){
            cin >> P.i >> P.j;
            cinll(n);
            rep(i,n){
                cinll(x,y);
                checked_map[x][y] = true;
                tentative_map[x][y] = tree_map[x][y];
            }
            // プレイヤーがAAにたどり着いていたら終わり
            if(P == T) break;
        }
        // テストでは移動処理をするため、何もしない

        // 木の配置を決定
        // プレイヤーが見える場所が2箇所以上ある時、一箇所に絞るようにする
        vpll diffs;
        if(turn == 0) diffs = enforce_turn_near_goal(tree_map, T.i, T.j);
        for(pll p: block_and_cut_sight(tree_map, checked_map, T.i, T.j, P.i, P.j)) diffs.push_back(p);
        // 出力
        cout << diffs.size() << " ";
        for(pll diff: diffs) cout << diff.fst <<" "<< diff.snd << " ";
        cout << endl;

        // プレイヤー移動処理（テスト用のみ）
        if(is_test == true){

            // 1.目的地についていたら終了（これは移動後にみる）

            // 2.上下左右それぞれの方向に対し、現在位置 からその方向に進んだ最初の「木」のマスまで（その木のマスを含む）のすべての未確認マスを 確認済みマス に追加する。
            update_checkedmap();

            // 3.伝説の花が 確認済みマス に含まれている場合、目的地 をそのマスに設定する。
            if(checked_map[T.i][T.j]){
                sT = T;
            }

            // 4.目的地 が未定でなく、暫定地図 において 目的地 が現在位置から空きマスのみを通って到達不能な場合、目的地 を未定に戻す。
            if(sT.i != -1 && min_dists({sT}, tentative_map)[to_hash(P)] == inf){
                sT = {-1, -1};
            }

            // 5.目的地 が未定であるか、伝説の花以外の 確認済みマス にある場合、暫定地図 において、現在位置から空きマスのみを通って到達可能な未確認マスの中から一様ランダムに1つ選び、それを 目的地 に設定する
            if(sT.i == -1 || (checked_map[sT.i][sT.j] && sT != T)){
                while(sT.i == -1 || checked_map[sT.i][sT.j] || min_dists({P}, tentative_map)[to_hash(sT)] == inf) cin >> sT.i >> sT.j;
            }
            
            // 6.暫定地図 において、各マスから空きマスのみを通って 目的地 へ到達する最短距離を計算する。現在位置に隣接する空きマスのうち、目的地 までの最短距離がより短くなるマスに移動する。そのようなマスが複数ある場合、上下左右の順に優先度をつけ、先に挙げられた方向を選択する。
            ll d = get_next_direction();
            directions.push_back(d);
            if(d == UP) P.i--;
            else if(d == DOWN) P.i++;
            else if(d == RIGHT) P.j++;
            else P.j--;

            // 目的地に到達したら終わり
            if(P == T) break;
        }

        // ターンを進める
        turn++;
    }
    if(is_test){
        cout <<turn << endl;
    }

    return 0;
}