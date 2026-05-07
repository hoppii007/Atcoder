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
#define cvvll1(name,a,b) vvll name(a, vll(b))
#define cvvll2(name,a,b,c) vvll name(a, vll(b,c))
#define cvvlloverload2(name,a,b,c,d,...) d
#define make_vvll(...) cvvlloverload2(__VA_ARGS__,cvvll2,cvvll1)(__VA_ARGS__)
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
#define vmll vector<mll>
#define vvmll vector<vector<mll>>

// const ll mod = 998244353LL;
// const ll mod = 1000000007LL;
const ll mod = 10007;
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

// pair 表示用
template <typename T, typename S>
std::ostream& operator<<(std::ostream& os, const std::pair<T,S>& p) { 
    os << p.fst << " " << p.snd;
    return os;
}


// =====================================================
// AHC063: Watchman Route on Grid
// Step 1: 入力パース + セグメント分解
// Step 2: DFS で初期巡回路を構築
// Step 3: カバー差分管理 (seg_cnt_h, seg_cnt_v, unseen_count)
// Step 4: 縮約ループ（同マス再訪潰し）
// Step 5: 任意点ペア最短路置換（全点対 Dijkstra）
// Step 6: 焼きなまし（shortcut + detour）
// =====================================================

ll N, si, sj;
vector<string> grid;

// セグメント情報
vvll h_id, v_id;            // (i,j) が属する水平/垂直セグメントのID（障害物は -1）
ll num_h, num_v;
vvpll h_cells, v_cells;     // 各セグメントに属するマス一覧

// 4方向: 0=U, 1=D, 2=L, 3=R （d^1 で逆方向）
const int dr[] = {-1, 1, 0, 0};
const int dc[] = {0, 0, -1, 1};
const char dch[] = {'U', 'D', 'L', 'R'};

// 巡回路: walk[0] = walk[L] = (si,sj)、隣接 walk[k]→walk[k+1] は1マス移動
vector<pll> walk;
vvb visited;

// Step 3: カバー差分管理
vi seg_cnt_h, seg_cnt_v;    // 各セグメントの walk 中の出現回数
ll unseen_count;            // 未だ視界に入っていない道路マス数
ll total_road;              // 道路マス総数

// Step 5: 道路マス連番ID + 全点対最短路
vector<pll> road_cells;     // road_cells[id] = (r, c)
vvll cell_id;               // cell_id[r][c] = road_id（障害物は -1）
vector<vector<int>> all_dist;     // all_dist[src_id][dst_id]
vector<vector<int>> all_parent;   // all_parent[src_id][dst_id] = 親の road_id
const int INF_DIST = 1e9;

/**
 * @brief 道路マスを水平・垂直の最大セグメントに分解し、各マスにIDを振る
 */
void decompose_segments(){
    h_id.assign(N, vll(N, -1));
    v_id.assign(N, vll(N, -1));
    num_h = 0;
    rep(i, N){
        ll j = 0;
        while(j < N){
            if(grid[i][j] == '#'){ j++; continue; }
            vpll cells;
            while(j < N && grid[i][j] != '#'){
                h_id[i][j] = num_h;
                cells.pb({i, j});
                j++;
            }
            h_cells.pb(cells);
            num_h++;
        }
    }
    num_v = 0;
    rep(j, N){
        ll i = 0;
        while(i < N){
            if(grid[i][j] == '#'){ i++; continue; }
            vpll cells;
            while(i < N && grid[i][j] != '#'){
                v_id[i][j] = num_v;
                cells.pb({i, j});
                i++;
            }
            v_cells.pb(cells);
            num_v++;
        }
    }
}

/**
 * @brief 始点に戻る閉じたDFS巡回路を構築する。各木辺を2回ずつ通る。walk に push
 *        @param dir_order 4方向の優先順（[0..3] の置換）。木の形を変えるために使う
 */
void dfs(ll r, ll c, const int* dir_order){
    visited[r][c] = true;
    rep(i, 4){
        int d = dir_order[i];
        ll nr = r + dr[d];
        ll nc = c + dc[d];
        if(nr < 0 || nr >= N || nc < 0 || nc >= N) continue;
        if(grid[nr][nc] == '#') continue;
        if(visited[nr][nc]) continue;
        walk.pb({nr, nc});
        dfs(nr, nc, dir_order);
        walk.pb({r, c});
    }
}

/**
 * @brief 巡回路に1マス分追加。セグメントカウンタと unseen_count を差分更新
 */
void add_cell(ll r, ll c){
    ll h = h_id[r][c], v = v_id[r][c];
    // h が 0→1 遷移: h_cells[h] 上の v 未 touched なマスが seen 化
    if(seg_cnt_h[h] == 0){
        for(auto& p : h_cells[h]){
            if(seg_cnt_v[v_id[p.fst][p.snd]] == 0) unseen_count--;
        }
    }
    seg_cnt_h[h]++;
    // v が 0→1 遷移: v_cells[v] 上の h 未 touched なマスが seen 化
    // ※ (r,c) 自身は seg_cnt_h[h] が既に >0 なので二重カウントされない
    if(seg_cnt_v[v] == 0){
        for(auto& p : v_cells[v]){
            if(seg_cnt_h[h_id[p.fst][p.snd]] == 0) unseen_count--;
        }
    }
    seg_cnt_v[v]++;
}

/**
 * @brief 巡回路から1マス分削除。add_cell の逆操作
 */
void remove_cell(ll r, ll c){
    ll h = h_id[r][c], v = v_id[r][c];
    seg_cnt_v[v]--;
    if(seg_cnt_v[v] == 0){
        for(auto& p : v_cells[v]){
            if(seg_cnt_h[h_id[p.fst][p.snd]] == 0) unseen_count++;
        }
    }
    seg_cnt_h[h]--;
    if(seg_cnt_h[h] == 0){
        for(auto& p : h_cells[h]){
            if(seg_cnt_v[v_id[p.fst][p.snd]] == 0) unseen_count++;
        }
    }
}

/**
 * @brief 同マス再訪を潰す縮約。各 pass で位置リスト pos を構築し、
 *        各 k について walk[k] の最大 idx を O(1) で参照、削除は lazy に deleted[] で記録
 * @return 削減された総コスト
 */
ll contract_loops(){
    ll total_saved = 0;
    bool any = true;
    int passes = 0;
    unordered_map<ll, vector<ll>> pos;  // 各 pass で再構築、再利用バッファ
    vector<pll> new_walk;
    while(any && passes < 20){
        any = false;
        passes++;
        // 位置リスト構築: cell -> その cell が現れる walk 内 index 列（昇順）
        pos.clear();
        ll W = walk.size();
        for(ll k = 0; k < W; k++){
            ll key = walk[k].fst * (N + 1) + walk[k].snd;
            pos[key].pb(k);
        }
        vector<bool> deleted(W, false);
        ll k = 0;
        while(k < W){
            if(deleted[k]){ k++; continue; }
            ll key = walk[k].fst * (N + 1) + walk[k].snd;
            auto& idxs = pos[key];
            // 後ろから走査して、deleted でない最大 idx > k を探す
            ll best_j = -1;
            for(int ii = (int)idxs.size() - 1; ii >= 0; ii--){
                if(idxs[ii] <= k) break;
                if(!deleted[idxs[ii]]){ best_j = idxs[ii]; break; }
            }
            if(best_j == -1){ k++; continue; }
            // walk[k+1..best_j] の削除を試す（既に deleted のものはスキップ）
            ll saved = 0;
            for(ll idx = k+1; idx <= best_j; idx++){
                if(deleted[idx]) continue;
                saved += grid[walk[idx].fst][walk[idx].snd] - '0';
                remove_cell(walk[idx].fst, walk[idx].snd);
            }
            if(unseen_count == 0){
                for(ll idx = k+1; idx <= best_j; idx++){
                    deleted[idx] = true;
                }
                total_saved += saved;
                any = true;
                // k はそのまま（再評価して連続削除可能性あり）
            } else {
                for(ll idx = k+1; idx <= best_j; idx++){
                    if(deleted[idx]) continue;
                    add_cell(walk[idx].fst, walk[idx].snd);
                }
                k++;
            }
        }
        // 削除フラグに基づいて walk を圧縮
        if(any){
            new_walk.clear();
            new_walk.reserve(W);
            for(ll idx = 0; idx < W; idx++){
                if(!deleted[idx]) new_walk.pb(walk[idx]);
            }
            walk.swap(new_walk);
        }
    }
    return total_saved;
}

/**
 * @brief 道路マスに連番IDを振る
 */
void enumerate_roads(){
    cell_id.assign(N, vll(N, -1));
    rep(i, N) rep(j, N){
        if(grid[i][j] != '#'){
            cell_id[i][j] = road_cells.size();
            road_cells.pb({i, j});
        }
    }
}

/**
 * @brief 全点対最短路を計算（各道路マス始点で Dijkstra）。
 *        all_dist[s][v] と all_parent[s][v] を埋める
 */
void compute_all_pairs(){
    ll R = road_cells.size();
    all_dist.assign(R, vector<int>(R, INF_DIST));
    all_parent.assign(R, vector<int>(R, -1));
    rep(s, R){
        all_dist[s][s] = 0;
        priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;
        pq.push({0, (int)s});
        while(!pq.empty()){
            auto [d, u] = pq.top(); pq.pop();
            if(d > all_dist[s][u]) continue;
            ll ur = road_cells[u].fst, uc = road_cells[u].snd;
            rep(dir, 4){
                ll nr = ur + dr[dir];
                ll nc = uc + dc[dir];
                if(nr < 0 || nr >= N || nc < 0 || nc >= N) continue;
                if(grid[nr][nc] == '#') continue;
                int v = cell_id[nr][nc];
                int nd = d + (grid[nr][nc] - '0');
                if(nd < all_dist[s][v]){
                    all_dist[s][v] = nd;
                    all_parent[s][v] = u;
                    pq.push({nd, v});
                }
            }
        }
    }
}

/**
 * @brief 親情報から (src_id, dst_id) の最短路を out に復元（バッファ再利用版）
 */
void reconstruct_path_id(int src_id, int dst_id, vector<pll>& out){
    out.clear();
    int cur = dst_id;
    while(cur != src_id){
        out.pb(road_cells[cur]);
        cur = all_parent[src_id][cur];
    }
    out.pb(road_cells[src_id]);
    reverse(all(out));
}

/**
 * @brief 任意点ペア (i,j) について walk[i+1..j] を最短路で置換できないか試す。
 *        全点対最短路で高速化。dist=0 のループ潰しも内包
 */
ll contract_shortcuts(){
    ll total_saved = 0;
    bool any = true;
    int passes = 0;
    // 関数内で再利用するバッファ
    vector<pll> path;
    vector<pll> new_walk;
    while(any && passes < 10){
        any = false;
        passes++;
        for(ll i = 0; i + 2 < (ll)walk.size(); i++){
            int src_id = cell_id[walk[i].fst][walk[i].snd];
            while(i + 2 < (ll)walk.size()){
                ll best_save = 0, best_j = -1;
                ll cum_cost = 0;
                for(ll j = i + 1; j < (ll)walk.size(); j++){
                    cum_cost += grid[walk[j].fst][walk[j].snd] - '0';
                    int dst_id = cell_id[walk[j].fst][walk[j].snd];
                    int d = all_dist[src_id][dst_id];
                    if(d >= INF_DIST) continue;
                    if(cum_cost - d > best_save){
                        best_save = cum_cost - d;
                        best_j = j;
                    }
                }
                if(best_j == -1) break;

                ll j = best_j;
                int dst_id = cell_id[walk[j].fst][walk[j].snd];
                reconstruct_path_id(src_id, dst_id, path);

                for(ll k = i + 1; k <= j; k++)
                    remove_cell(walk[k].fst, walk[k].snd);
                for(ll k = 1; k < (ll)path.size(); k++)
                    add_cell(path[k].fst, path[k].snd);

                if(unseen_count == 0){
                    new_walk.clear();
                    new_walk.reserve(walk.size() - (j - i) + path.size() - 1);
                    rep(k, i + 1) new_walk.pb(walk[k]);
                    rep(k, 1, (ll)path.size()) new_walk.pb(path[k]);
                    rep(k, j + 1, (ll)walk.size()) new_walk.pb(walk[k]);
                    walk.swap(new_walk);
                    total_saved += best_save;
                    any = true;
                } else {
                    for(ll k = (ll)path.size() - 1; k >= 1; k--)
                        remove_cell(path[k].fst, path[k].snd);
                    for(ll k = j; k >= i + 1; k--)
                        add_cell(walk[k].fst, walk[k].snd);
                    break;
                }
            }
        }
    }
    return total_saved;
}

/**
 * @brief 二部頂点被覆 + TSP に基づく初期巡回路を構築
 *        1. König で最小頂点被覆 (vc_h, vc_v) を計算
 *        2. 各被覆セグメントから最小コスト代表マスを選択
 *        3. start + 代表マスで NN + 2-opt の TSP
 *        4. 区間を最短路で繋いで walk 構築
 *        5. 既存の contract で磨く
 * @return 構築後の総移動時間 t
 */
ll build_vc_initial(){
    // ---- Step 1: 二部マッチング (Kuhn's) ----
    vector<vector<int>> bp_adj(num_h);
    for(auto& cell : road_cells){
        int h = h_id[cell.fst][cell.snd];
        int v = v_id[cell.fst][cell.snd];
        bp_adj[h].pb(v);
    }
    vector<int> match_l(num_h, -1), match_r(num_v, -1);
    vector<bool> visited_v(num_v, false);

    function<bool(int)> try_kuhn = [&](int u) -> bool {
        for(int v : bp_adj[u]){
            if(visited_v[v]) continue;
            visited_v[v] = true;
            if(match_r[v] == -1 || try_kuhn(match_r[v])){
                match_l[u] = v;
                match_r[v] = u;
                return true;
            }
        }
        return false;
    };

    for(int u = 0; u < (int)num_h; u++){
        fill(visited_v.begin(), visited_v.end(), false);
        try_kuhn(u);
    }

    // ---- Step 2: König の到達集合（VC = (L \ Z) ∪ (R ∩ Z)） ----
    vector<bool> in_z_l(num_h, false), in_z_r(num_v, false);
    queue<int> q;
    for(int u = 0; u < (int)num_h; u++){
        if(match_l[u] == -1){
            in_z_l[u] = true;
            q.push(u);
        }
    }
    while(!q.empty()){
        int u = q.front(); q.pop();
        for(int v : bp_adj[u]){
            if(in_z_r[v]) continue;
            in_z_r[v] = true;
            int next = match_r[v];
            if(next != -1 && !in_z_l[next]){
                in_z_l[next] = true;
                q.push(next);
            }
        }
    }

    vector<int> vc_h, vc_v;
    for(int u = 0; u < (int)num_h; u++) if(!in_z_l[u]) vc_h.pb(u);
    for(int v = 0; v < (int)num_v; v++) if(in_z_r[v]) vc_v.pb(v);

    // ---- Step 3: 各被覆セグメントから最小コスト代表を選択 ----
    vector<pll> reps;
    for(int h : vc_h){
        pll best = h_cells[h][0];
        for(auto& c : h_cells[h]){
            if(grid[c.fst][c.snd] < grid[best.fst][best.snd]) best = c;
        }
        reps.pb(best);
    }
    for(int v : vc_v){
        pll best = v_cells[v][0];
        for(auto& c : v_cells[v]){
            if(grid[c.fst][c.snd] < grid[best.fst][best.snd]) best = c;
        }
        reps.pb(best);
    }

    int n = reps.size();
    cerr << "[vc] |MVC|=" << n << " (h=" << vc_h.size() << ", v=" << vc_v.size() << ")" << endl;

    // ---- Step 4: TSP ----
    // 都市: 0=start, 1..n=代表マス
    vector<int> city_rid(n + 1);
    city_rid[0] = cell_id[si][sj];
    for(int i = 0; i < n; i++) city_rid[i+1] = cell_id[reps[i].fst][reps[i].snd];

    // NN 構築
    vector<int> tour;
    tour.pb(0);
    vector<bool> in_tour(n + 1, false);
    in_tour[0] = true;
    int cur = 0;
    for(int step = 0; step < n; step++){
        int best_j = -1;
        int best_d = INT_MAX;
        for(int j = 1; j <= n; j++){
            if(in_tour[j]) continue;
            int d = all_dist[city_rid[cur]][city_rid[j]];
            if(d < best_d){ best_d = d; best_j = j; }
        }
        tour.pb(best_j);
        in_tour[best_j] = true;
        cur = best_j;
    }
    tour.pb(0);  // 始点へ戻る

    // 2-opt（reverse での改善）
    bool improved = true;
    while(improved){
        improved = false;
        int sz = tour.size();
        for(int i = 1; i < sz - 2; i++){
            for(int j = i + 1; j < sz - 1; j++){
                int a = tour[i-1], b = tour[i], c = tour[j], d = tour[j+1];
                int old_c = all_dist[city_rid[a]][city_rid[b]] + all_dist[city_rid[c]][city_rid[d]];
                int new_c = all_dist[city_rid[a]][city_rid[c]] + all_dist[city_rid[b]][city_rid[d]];
                if(new_c < old_c){
                    reverse(tour.begin() + i, tour.begin() + j + 1);
                    improved = true;
                }
            }
        }
    }

    // ---- Step 5: 都市間を最短路で繋いで walk 構築 ----
    walk.clear();
    walk.pb(road_cells[city_rid[tour[0]]]);
    vector<pll> seg;
    for(int k = 0; k + 1 < (int)tour.size(); k++){
        int src = city_rid[tour[k]];
        int dst = city_rid[tour[k+1]];
        reconstruct_path_id(src, dst, seg);
        for(int i = 1; i < (int)seg.size(); i++){
            walk.pb(seg[i]);
        }
    }

    // seg_cnt 初期化
    seg_cnt_h.assign(num_h, 0);
    seg_cnt_v.assign(num_v, 0);
    unseen_count = total_road;
    for(auto& p : walk) add_cell(p.fst, p.snd);

    // ---- Step 6: 縮約で磨く ----
    contract_loops();
    contract_shortcuts();

    ll t = 0;
    rep(k, 1, (ll)walk.size()) t += grid[walk[k].fst][walk[k].snd] - '0';
    return t;
}

/**
 * @brief 焼きなまし。shortcut（Δ≤0、coverage check）と detour（Δ>0、SA受理）を混合。
 *        best_walk を保持して最終的に復元
 */
void simulated_annealing(double time_limit){
    auto start_time = chrono::steady_clock::now();
    mt19937_64 rng(20260429);
    uniform_real_distribution<double> rand_unit(0.0, 1.0);

    // 環境変数 TS, TE で温度を上書きできる（パラメータ探索用）
    double T_start = 18.0, T_end = 0.5;
    if(const char* p = getenv("TS")) T_start = atof(p);
    if(const char* p = getenv("TE")) T_end = atof(p);
    cerr << "[SA] T_start=" << T_start << " T_end=" << T_end << endl;

    ll cur_t = 0;
    rep(k, 1, (ll)walk.size()) cur_t += grid[walk[k].fst][walk[k].snd] - '0';

    vector<pll> best_walk = walk;
    ll best_t = cur_t;

    // 再利用バッファ（毎 iter の alloc を抑える）
    vector<pll> path_buf, walk_buf, out_path_buf, insert_buf;
    walk_buf.reserve(walk.size() + 100);

    // walk_psum[k] = sum of cost(walk[0..k-1])。 cum_cost(i+1..j) = walk_psum[j+1] - walk_psum[i+1]
    vector<int> walk_psum(walk.size() + 1, 0);
    for(ll k = 0; k < (ll)walk.size(); k++){
        walk_psum[k+1] = walk_psum[k] + (grid[walk[k].fst][walk[k].snd] - '0');
    }
    // psum を idx 以降について再計算するヘルパ
    auto rebuild_psum_from = [&](ll idx){
        walk_psum.resize(walk.size() + 1);
        for(ll k = idx; k < (ll)walk.size(); k++){
            walk_psum[k+1] = walk_psum[k] + (grid[walk[k].fst][walk[k].snd] - '0');
        }
    };

    ll iters = 0, acc1 = 0, rej1 = 0, acc2 = 0, rej2 = 0;
    double elapsed = 0.0;
    double T = T_start;
    while(true){
        iters++;
        if((iters & 1023) == 0){
            elapsed = chrono::duration<double>(chrono::steady_clock::now() - start_time).count();
            if(elapsed > time_limit) break;
            // T は 1024 iter ごとに更新（pow 呼び出し頻度を 1/1024 に）
            double progress = elapsed / time_limit;
            T = T_start * pow(T_end / T_start, progress);
        }

        int mtype = rng() % 5;
        if(mtype < 4){
            // ----- Shortcut: 任意点ペア最短路置換 -----
            ll W = walk.size();
            if(W < 3){ rej1++; continue; }
            ll i = rng() % (W - 2);
            ll max_jspan = min<ll>((ll)W - 1 - i, 100);
            ll j = i + 1 + (rng() % max_jspan);

            int src_id = cell_id[walk[i].fst][walk[i].snd];
            int dst_id = cell_id[walk[j].fst][walk[j].snd];
            int new_cost = all_dist[src_id][dst_id];
            if(new_cost >= INF_DIST){ rej1++; continue; }
            ll cur_cost = walk_psum[j+1] - walk_psum[i+1];  // O(1) prefix sum
            ll delta = (ll)new_cost - cur_cost;
            // shortcut は Δ ≤ 0 なので確率受理は不要、coverage チェックのみ

            reconstruct_path_id(src_id, dst_id, path_buf);
            for(ll k = i+1; k <= j; k++) remove_cell(walk[k].fst, walk[k].snd);
            for(ll k = 1; k < (ll)path_buf.size(); k++) add_cell(path_buf[k].fst, path_buf[k].snd);

            if(unseen_count != 0){
                for(ll k = (ll)path_buf.size() - 1; k >= 1; k--) remove_cell(path_buf[k].fst, path_buf[k].snd);
                for(ll k = j; k >= i+1; k--) add_cell(walk[k].fst, walk[k].snd);
                rej1++;
                continue;
            }

            walk_buf.clear();
            walk_buf.reserve(walk.size() - (j - i) + path_buf.size() - 1);
            rep(k, i+1) walk_buf.pb(walk[k]);
            rep(k, 1, (ll)path_buf.size()) walk_buf.pb(path_buf[k]);
            rep(k, j+1, (ll)walk.size()) walk_buf.pb(walk[k]);
            walk.swap(walk_buf);
            // walk[0..i] は不変なので psum も walk_psum[0..i+1] は不変、それ以降を再計算
            rebuild_psum_from(i + 1);
            cur_t += delta;
            acc1++;

            if(cur_t < best_t){
                best_t = cur_t;
                best_walk = walk;
            }
        } else {
            // ----- Detour: 1〜3マスのランダムウォーク往復（Δ>0） -----
            ll W = walk.size();
            if(W < 2){ rej2++; continue; }
            ll k = rng() % W;
            // L 分布: 50% で 1 歩、30% で 2 歩、20% で 3 歩
            int rl = rng() % 10;
            ll L = (rl < 5) ? 1 : ((rl < 8) ? 2 : 3);

            // walk[k] からランダムウォーク（直近で来た方向への逆戻りは禁止）
            out_path_buf.clear();
            ll cur_r = walk[k].fst, cur_c = walk[k].snd;
            int prev_back = -1;
            rep(step, L){
                int dirs[4] = {0, 1, 2, 3};
                shuffle(dirs, dirs + 4, rng);
                bool found = false;
                for(int dir : dirs){
                    if(dir == prev_back) continue;
                    ll nr = cur_r + dr[dir];
                    ll nc = cur_c + dc[dir];
                    if(nr < 0 || nr >= N || nc < 0 || nc >= N) continue;
                    if(grid[nr][nc] == '#') continue;
                    out_path_buf.pb({nr, nc});
                    cur_r = nr; cur_c = nc;
                    prev_back = dir ^ 1;
                    found = true;
                    break;
                }
                if(!found) break;
            }
            if(out_path_buf.empty()){ rej2++; continue; }

            // 挿入セル列 = out_path + 逆順 out_path[0..end-2] + walk[k]
            insert_buf.clear();
            insert_buf.reserve(out_path_buf.size() * 2);
            for(auto& c : out_path_buf) insert_buf.pb(c);
            for(ll idx = (ll)out_path_buf.size() - 2; idx >= 0; idx--)
                insert_buf.pb(out_path_buf[idx]);
            insert_buf.pb(walk[k]);

            ll delta = 0;
            for(auto& c : insert_buf) delta += grid[c.fst][c.snd] - '0';

            double prob = exp(-(double)delta / T);
            if(rand_unit(rng) >= prob){ rej2++; continue; }

            // 採用
            for(auto& c : insert_buf) add_cell(c.fst, c.snd);
            walk.insert(walk.begin() + k + 1, insert_buf.begin(), insert_buf.end());
            // walk[0..k] は不変、walk[k+1..] が変化したので psum 再計算
            rebuild_psum_from(k + 1);
            cur_t += delta;
            acc2++;
        }
    }

    // best_walk を採用、seg_cnt も再構築
    walk = best_walk;
    seg_cnt_h.assign(num_h, 0);
    seg_cnt_v.assign(num_v, 0);
    unseen_count = total_road;
    for(auto& p : walk) add_cell(p.fst, p.snd);

    cerr << "[SA] iters=" << iters
         << " elapsed=" << elapsed
         << " acc1=" << acc1 << "/" << (acc1 + rej1)
         << " acc2=" << acc2 << "/" << (acc2 + rej2)
         << " best_t=" << best_t << endl;
}

/**
 * @brief walk を出力用の UDLR 文字列に変換
 */
string walk_to_moves(){
    string s;
    rep(k, (ll)walk.size() - 1){
        ll ddr = walk[k+1].fst - walk[k].fst;
        ll ddc = walk[k+1].snd - walk[k].snd;
        rep(d, 4){
            if(dr[d] == ddr && dc[d] == ddc){ s += dch[d]; break; }
        }
    }
    return s;
}

int main(){
    auto t0 = chrono::steady_clock::now();

    // ---- 入力 ----
    cin >> N >> si >> sj;
    grid.resize(N);
    rep(i, N) cin >> grid[i];

    // ---- Step 1: セグメント分解 ----
    decompose_segments();

    // ---- 道路マス連番化 + 全点対最短路 ----
    enumerate_roads();
    compute_all_pairs();

    total_road = road_cells.size();

    // ---- Step 2-5 を、複数の DFS 順で実行して最良を採用 ----
    auto build_one_initial = [&](const int* dir_order) -> ll {
        walk.clear();
        walk.pb({si, sj});
        visited.assign(N, vb(N, false));
        dfs(si, sj, dir_order);

        seg_cnt_h.assign(num_h, 0);
        seg_cnt_v.assign(num_v, 0);
        unseen_count = total_road;
        for(auto& p : walk) add_cell(p.fst, p.snd);

        contract_loops();
        contract_shortcuts();

        ll t = 0;
        rep(k, 1, (ll)walk.size()) t += grid[walk[k].fst][walk[k].snd] - '0';
        return t;
    };

    // 24 通りの置換を列挙してシャッフル、先頭 n_init 個を試す（重複なし）
    vector<array<int,4>> all_perms;
    {
        int p[4] = {0, 1, 2, 3};
        do {
            all_perms.push_back({p[0], p[1], p[2], p[3]});
        } while(next_permutation(p, p + 4));
    }
    mt19937_64 init_rng(98765);
    shuffle(all_perms.begin() + 1, all_perms.end(), init_rng);  // {0,1,2,3} は先頭固定

    const int n_init_max = min(8, (int)all_perms.size());
    const double init_phase_deadline = 0.7;  // この時刻を超えたら init 打ち切り → SA に予算を回す
    vector<pll> best_init_walk;
    ll best_init_t = inf;
    int n_init_done = 0;

    // VC + TSP 構造的初期解（必ず実行）
    {
        double el = chrono::duration<double>(chrono::steady_clock::now() - t0).count();
        ll t = build_vc_initial();
        cerr << "[init vc] moves=" << walk.size()-1 << " t=" << t << " elapsed=" << el << endl;
        best_init_t = t;
        best_init_walk = walk;
        n_init_done++;
    }

    rep(trial, n_init_max){
        double el = chrono::duration<double>(chrono::steady_clock::now() - t0).count();
        if(trial > 0 && el > init_phase_deadline) break;  // 1個目は必ず走らせる
        int dir_order[4];
        rep(i, 4) dir_order[i] = all_perms[trial][i];
        ll t = build_one_initial(dir_order);
        cerr << "[init " << trial << "] order={"
             << dir_order[0] << "," << dir_order[1] << ","
             << dir_order[2] << "," << dir_order[3]
             << "} moves=" << walk.size()-1 << " t=" << t
             << " elapsed=" << el << endl;
        if(t < best_init_t){
            best_init_t = t;
            best_init_walk = walk;
        }
        n_init_done++;
    }
    cerr << "[init done] used=" << n_init_done << "/" << n_init_max << endl;

    // 最良初期解を採用、seg_cnt を再構築
    walk = best_init_walk;
    seg_cnt_h.assign(num_h, 0);
    seg_cnt_v.assign(num_v, 0);
    unseen_count = total_road;
    for(auto& p : walk) add_cell(p.fst, p.snd);

    cerr << "[best init] moves=" << walk.size()-1 << " t=" << best_init_t << endl;

    // ---- Step 6: 焼きなまし ----
    double elapsed = chrono::duration<double>(chrono::steady_clock::now() - t0).count();
    double time_budget = 1.85 - elapsed;
    if(time_budget > 0.1){
        simulated_annealing(time_budget);
    }

    // ---- 出力 ----
    string moves = walk_to_moves();
    cout << moves << endl;

    // ---- デバッグ情報 ----
    ll t_final = 0;
    rep(k, 1, (ll)walk.size()) t_final += grid[walk[k].fst][walk[k].snd] - '0';
    cerr << "[final] moves=" << moves.size()
         << " t=" << t_final
         << " unseen=" << unseen_count << endl;
    cerr << "N=" << N << " road_cells=" << total_road
         << " H_seg=" << num_h << " V_seg=" << num_v << endl;
    cerr << "closed=" << (walk.front() == walk.back() ? "yes" : "no") << endl;

    if(unseen_count == 0 && t_final > 0){
        ll score = 10000 + (ll)round(1e7 * (double)N / (double)t_final);
        cerr << "estimated_score=" << score << endl;
    }

    return 0;
}