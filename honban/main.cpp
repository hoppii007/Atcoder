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


//縦0以上h未満、横0以上w未満の(i,j)の4近傍をとる
vector<Pos> kinbo4(Pos p){
    vector<Pos> ret;
    ll i = p.i;
    ll j = p.j;
    if(i-1 >= 0) ret.push_back({i-1,j});
    if(j-1 >= 0) ret.push_back({i,j-1});
    if(i+1 < N) ret.push_back({i+1,j});
    if(j+1 < N) ret.push_back({i,j+1});
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
        if (visited[nxt.i][nxt.j]) continue; // 訪問済みスキップ
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
    rep(i,N-2) rep(j,N-2) if(!use_map[i][j]) {
        // vector<Pos> ps;
        // for(Pos p: kinbo4(Pos{i,j})) if(!wall_map[p.i][p.j]) ps.push_back(p);
        // if(ps.size() == 0) continue;
        // vll dists = min_dists({ps[0]}, wall_map);
        // bool is_ng = false;
        // for(Pos p: ps) if(dists[to_hash(p)] == inf) is_ng = true;
        // if(is_ng == false) wall_map[i][j] = true;
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

    // 山登りで、最長経路を計算する
    vvb random_path_map = generate_random_path_map(P, T, tree_map);
    ll score = 0;
    rep(i,N) rep(j,N) score += random_path_map[i][j];
    TimeKeeper tk;
    tk.start();
    ll take = 0;
    while(tk.now() <= 1700){
        take++;
        vvb nmap = generate_random_path_map(P, T, tree_map);
        ll nscore = 0;
        rep(i,N) rep(j,N) nscore += nmap[i][j];
        if(score < nscore){
            random_path_map = nmap;
            score = nscore;
        }
    }
    // random_path_map = generate_nearest_path_map(P, T, tree_map);

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
        if(turn == 0){
            vpll diffs;
            // 使う場所に関して、レールを作っていく（穴あきにすることで惑わせる）
            // レール以外の部分は基本的に開けておくことで、迷わせる
            rep(i,N) rep(j,N) {
                if(random_path_map[i][j]) continue; // 使う場所は無視
                if(tree_map[i][j]) continue; // 元々木が生えているところは無視
                // 使う場所に隣接していなければ無視
                ll cnt = 0;
                for(Pos& p: kinbo4(Pos{i,j})) cnt += random_path_map[p.i][p.j];
                if(cnt == 0) continue;
                // ゴールに隣接していないところは、たまに穴あきにする
                cnt = 0;
                for(Pos& p: kinbo4(Pos{i,j})) cnt += (p == T);
                if(cnt == 0 && randll(0, 100) < 5) continue;

                diffs.push_back({i,j});
                tree_map[i][j] = true;
            }
            // 空きすぎてる場所があれば、ラフマップで埋める（確認を妨害）
            vvb rough_map = make_rough_map(random_path_map, tree_map);
            rep(i,N) rep(j,N) if(rough_map[i][j] != tree_map[i][j]){
                diffs.push_back({i,j});
                tree_map[i][j] = true;
            }
            cout << diffs.size() << " ";
            for(pll p: diffs) cout << p.fst << " " << p.snd << " ";
            cout << endl;
        }
        else{
            cout << 0 << endl;
        }


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