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

//const ll mod = 998244353LL;
const ll mod = 1000000007LL;
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

////戦略/////
// BOMBERとBERSERKERは一番近い二人を取るようにして、それ以外は評価値にした
// 一応職業の特性は考慮してる
// 高速化がやりたい
///////////

/////結果//////
// シルバー100 -> 80
//////////////

// a以上b未満の乱数を返す
std::mt19937_64 rndmt(std::random_device{}());
int randint(int a, int b){
    std::uniform_int_distribution<int> r(a, b-1);
    return r(rndmt);
}

//グローバル変数たち
int my_id;
int height, width;
int agent_data_count;
vvi grid;
int total_my_score = 0;
int total_enemy_score = 0;
int death_count_enemy = 0;
int death_count_player = 0;

const int LOW_COVER = -1;
const int HIGH_COVER = -2;
const int GUNNER = 0;
const int SNIPER = 1;
const int BOMBER = 2;
const int ASSAULT = 3;
const int BERSERKER = 4;
const int UP = 0;
const int RIGHT = 1;
const int DOWN = 2;
const int LEFT = 1;
const vs type_names = {"GUNNER", "SNIPER", "BOMBER", "ASSAULT", "BERSERKER"};

struct Op {
    int agent_id;
    string op_move;
    string op_action;

    void reset(){
        op_move = "";
        op_action = "";
    }

    void setMove(pi p){
        if(my_id == 0){
            op_move = "MOVE " + to_string(p.snd) + " " + to_string(p.fst) + ";";
        }
        else{
            op_move = "MOVE " + to_string(width-1-p.snd) + " " + to_string(p.fst) + ";";
        }
    }

    void setShoot(int target_id){
        op_action = "SHOOT " + to_string(target_id) + ";";
    }
    void setThrow(pi p){
        if(my_id == 0){
            op_action = "THROW " + to_string(p.snd) + " " + to_string(p.fst) + ";";
        }
        else{
            op_action = "THROW " + to_string(width-1-p.snd) + " " + to_string(p.fst) + ";";
        }
    }
    string getStr(){
        string ret = to_string(agent_id)+";"+op_move;
        if(op_action.size() == 0) ret += "HUNKER_DOWN;";
        else ret += op_action;
        return ret;
    }
};
struct Agent {
    int agent_id;
    bool mine;
    pi pos = {-1,-1};
    int wetness;
    int power;
    int range;
    int bom;
    int cooldown;
    
    int cd = 0;
    int agent_type;
    int line; //何行目を担当するか？
    Op op;
    
    Agent(){}
    Agent(int agent_id){
        this->agent_id = agent_id;
    }
    Agent(int agent_id, bool mine, int power, int range, int bom, int cooldown){
        this->agent_id = agent_id;
        this->mine = mine;
        this->power = power;
        this->range = range;
        this->bom = bom;
        this->cooldown = cooldown;
        op.agent_id = agent_id;
        setAgentType();
    }
    void setAgentType(){
        if(cooldown == 1 && power == 16 && range == 4 && bom == 1){
            agent_type = GUNNER;
        }
        else if(cooldown == 5 && power == 24 && range == 6 && bom == 0){
            agent_type = SNIPER;
        }
        else if(cooldown == 2 && power == 8 && range == 2 && bom == 3){
            agent_type = BOMBER;
        }
        else if(cooldown == 2 && power == 16 && range == 4 && bom == 2){
            agent_type = ASSAULT;
        }
        else {
            agent_type = BERSERKER;
        }
    }
    void setPos(pi p){
        if(pos.fst == -1) line = p.fst;
        pos = p;
    }
    bool operator==(const Agent &other) const {
        return agent_id == other.agent_id;
    }

};
vi agent_ids;
map<int,Agent> agents;
vpi allPositions;

/////////////関数宣言//////////////////

    //距離系
    int manhat(pi a, pi b);                                     //マンハッタン距離を求める
    int wayDist(pi a, pi b, bool thinkAgent);                   //道のりの距離（エージェント考慮）
    void initWayDist();                                         //道のりの距離初期化（エージェント考慮なし）
    vvi wayDistTable;

    //座標系
    vpi getNeighbors4(pi p);                                    //盤面からでない、4近傍
    vpi getNeighborsCanMove(pi p);                              //移動可能な近傍（エージェント考慮）
    vpi getNeighborsTo(pi p, pi target);                        //pからtargetに近付く道のりの一歩目リスト（エージェント考慮）
    vpi getPositionsIf(vpi &ps, function<bool(pi)> f);          //座標集合から、特定の条件を満たす座標集合を返す
    vpi getPositionsAll();                                      //盤面上の全ての座標を返す
    vpi getPositionsLte(vpi &ps, pi p, int d);                  //座標pから、距離d以下の座標を全て取得
    pi getPositionOptimalBom(pi p);                             //pから爆弾を投げれる場所の中で、(敵の数-味方の数)が最大の場所を返す（プラスの場所がなければ{-1,-1}）
    pi getPositionNearestWay(vpi &ps, pi p, bool thinkAgent);   //psの中で、pから一番近い場所を取得
    pi getPositionRightmost(vpi &ps);                           //一番右のやつを取得（複数ある場合は適当）
    pi getPositionLeftmost(vpi &ps);                            //一番左のやつを取得（複数ある場合は適当）
    vpi getPositionsAllInRange(pi lu, pi d);                    //全体座標の中から、特定の区画内の座標を全て取得
    pi getPositionSafemost(vpi &ps, pi &enemys);                //敵から一番身を守れる場所を探す

    //エージェント取得系
    vi getAgentsIf(vi &aids, function<bool(Agent)> f);          //条件fがtrueになるやつを抽出
    vi getEnemys(vi &aids);                                     //敵だけを抽出
    vi getPlayers(vi &aids);                                    //味方だけを抽出
    int getAgentMaxWetness(vi &aids);                           //一番濡れてるエージェントを取得
    int getAgentLeftmost(vi &aids);                             //一番左にいるエージェントを取得
    vi getAgentsIfInRange(vi &aids, pi lu, pi d, function<bool(Agent)> f);//指定した範囲内の特定のエージェントを取得(dはh,w)
    int getAgentNearestWay(vi &aids, pi p);                     //特定の場所から、一番近い場所のエージェントを取得
    vi getAgentsLte(vi &aids, pi p, int d);                     //pからマンハッタン距離d以内に敵がいるか？
    int getAgentDamagemost(vi &aids, int agent);                //aidsの中で、agentからの攻撃が一番通るやつを一つ取得

    //エージェントセット系
    void setTargetPos(int agent, pi target);

    //スコア系
    pi getScores();                                             //現状のagent_idsに対して、{自分の占領地数、相手の占有地数}を返す

    //ダメージ系
    double getDamageRate(int aid, pi p2);                       //aidがpを攻撃した時のダメージ割合（高いほどよく通る）
    double getTotalDamageRate(pi p, vi aids);                   //pがエージェント集合から受けうるダメージ割合の合計（高いほどダメージの期待値が高い

    //カウント系
    int countHighCover(pi p);                                   //pの周りのHighCoverの数

    //評価値系
    double getEvaluation(int agent, pi p, double defense_level, double ofence_level, double invasion_level);
    pi getPositionBestEvaluation(vpi &ps, int agent, double defense_level, double ofence_level, double invasion_level);

    //個別：スナイパー
    pi getPositionOptimalSniper(int agent, int away_level, int move_level, int frontEnemy); //スナイパーにとってのベストな位置を探索

    //個別：ガンナー
    pi getPositionOptimalGunner(int agent);                     //ガンナーにとってのベストな位置

    //個別：ボンバー
    pi getPositionOptimalBomber(int agent);

    //個別：アサルト
    pi getPositionOptimalAssault(int agent);

    //個別：バーサーカー
    pi getPositionOptimalBerserker(int agent);
//////////////////////////////////


////////////関数実装///////////////////
    //距離系
    int manhat(pi a, pi b){
        return abs(a.fst-b.fst) + abs(a.snd-b.snd);
    }
    int wayDist(pi a, pi b, bool thinkAgent){
        //エージェントを考慮しない場合は、tableを参照するだけ
        if(thinkAgent == false){
            return wayDistTable[a.fst*width+a.snd][b.fst*width+b.snd];
        }

        //エージェント考慮する場合はちゃんとCanMoveだけedgeとする
        vi dp(height*width, 100000);
        queue<int> q;
        q.push(a.fst*width+a.snd);
        dp[a.fst*width+a.snd] = 0;
        while(!q.empty()){
            int now = q.front();
            q.pop();
            vpi cands = getNeighborsCanMove({now/width, now%width});
            for(pi cand: cands) if(dp[cand.fst*width+cand.snd] == 100000){
                dp[cand.fst*width+cand.snd] = dp[now] + 1;
                q.push(cand.fst*width+cand.snd);
            }
        }
        vpi goalNeighbors = getNeighborsCanMove(b);
        int ret = 100000;
        for(pi nbr: goalNeighbors) chmin(ret, dp[nbr.fst*width+nbr.snd]);
        return ret;
    }
    void initWayDist(){
        wayDistTable = vvi(width*height, vi(width*height, -1));
        function<vi(pi)> bfs = [&](pi p){
            vi dp(height*width, -1);
            queue<int> q;
            q.push(p.fst*width+p.snd);
            dp[p.fst*width+p.snd] = 0;
            while(!q.empty()){
                int now = q.front();
                q.pop();
                vpi cands = getNeighborsCanMove({now/width, now%width});
                for(pi cand: cands) if(dp[cand.fst*width+cand.snd] == -1){
                    dp[cand.fst*width+cand.snd] = dp[now] + 1;
                    q.push(cand.fst*width+cand.snd);
                }
            }
            return dp;
        };
        rep(i,height) rep(j,width){
            wayDistTable[i*width+j] = bfs({i,j});
        }
    }

    //座標系
    vpi getNeighbors4(pi p){
        vpi ret;
        if(p.fst+1 < height) ret.push_back({p.fst+1, p.snd});
        if(p.fst-1 >= 0) ret.push_back({p.fst-1, p.snd});
        if(p.snd+1 < width) ret.push_back({p.fst, p.snd+1});
        if(p.snd-1 >= 0) ret.push_back({p.fst, p.snd-1});
        return ret;
    }
    vpi getNeighborsCanMove(pi p){
        vpi ret;
        if(p.fst+1 < height && (grid[p.fst+1][p.snd] == 0)) ret.push_back({p.fst+1, p.snd});
        if(p.fst-1 >= 0 && grid[p.fst-1][p.snd] == 0) ret.push_back({p.fst-1, p.snd});
        if(p.snd+1 < width && grid[p.fst][p.snd+1] == 0) ret.push_back({p.fst, p.snd+1});
        if(p.snd-1 >= 0 && grid[p.fst][p.snd-1] == 0) ret.push_back({p.fst, p.snd-1});
        return ret; 
    }
    vpi getNeighborsTo(pi p, pi target){
        if(p == target) return {};
        vpi cands = getNeighborsCanMove(p);
        vpi ret;
        int d = wayDist(p, target, true);
        for(pi cand: cands) if(wayDist(cand, target, true) < d) ret.push_back(cand);
        return ret; 
    }
    vpi getPositionsIf(vpi &ps, function<bool(pi)> f){
        vpi ret;
        for(pi p: ps) if(f(p)) ret.push_back(p);
        return ret;
    }
    vpi getPositionsAll(){
        vpi ret;
        rep(i,height) rep(j,width) ret.push_back({i,j});
        return ret;
    }
    vpi getPositionsLte(vpi &ps, pi p, int d){
        vpi ret;
        for(pi cand: ps) if(manhat(cand, p) <= d) ret.push_back(cand);
        return ret;
    }
    pi getPositionOptimalBom(pi p){
        vpi cands = getPositionsLte(allPositions, p, 4);
        pi ret = {-1,-1};
        int maxi = 0;
        for(pi cand: cands){
            int countPlayer = getAgentsIfInRange(agent_ids, {cand.fst-1,cand.snd-1}, {3,3}, [](Agent a){ return a.mine; }).size();
            int countEnemy = getAgentsIfInRange(agent_ids, {cand.fst-1,cand.snd-1}, {3,3}, [](Agent a){ return !a.mine; }).size();
            int score = countEnemy - countPlayer;
            if(0 < score && maxi < score){
                maxi = score;
                ret = cand;
            }
        }
        return ret;
    }
    pi getPositionNearestWay(vpi &ps, pi p, bool thinkAgent){
        int mini = 100000;
        pi ret = {-1,-1};
        for(pi cand: ps){
            int dist = wayDist(cand, p, thinkAgent);
            if(dist < mini){
                mini = dist;
                ret = cand;
            }
        }
        return ret;
    }
    pi getPositionRightmost(vpi &ps){
        pi ret = {-1,-1};
        int maxi = -100000;
        for(pi p: ps){
            if(maxi < p.snd){
                maxi = p.snd;
                ret = p;
            }
        }
        return ret;
    }
    pi getPositionLeftmost(vpi &ps){
        pi ret = {-1,-1};
        int mini = 100000;
        for(pi p: ps){
            if(p.snd < mini){
                mini = p.snd;
                ret = p;
            }
        }
        return ret;
    }
    vpi getPositionsAllInRange(pi lu, pi d){
        vpi ret;
        rep(i,max(0,lu.fst), min(height, lu.fst+d.fst)){
            rep(j,max(0,lu.snd), min(width, lu.snd+d.snd)){
                ret.push_back({i,j});
            }
        }
        return ret;
    }
    pi getPositionSafemost(vpi &ps, vi &enemys){
        pi ret = {-1,-1};
        double mini = 100000;
        for(pi p: ps){
            double score = getTotalDamageRate(p, enemys);
            if(score < mini){
                ret = p;
                mini = score;
            }
        }
        return ret;
    }

    //エージェント取得系
    vi getAgentsIf(vi &aids, function<bool(Agent)> f){
        vi ret;
        for(int aid: aids){
            if(f(agents[aid])) ret.push_back(aid);
        }
        return ret;
    }
    vi getEnemys(vi &aids){
        return getAgentsIf(aids, [](Agent a){ return !a.mine; });
    }
    vi getPlayers(vi &aids){
        return getAgentsIf(aids, [](Agent a){ return a.mine; });
    }
    int getAgentMaxWetness(vi &aids){
        int ret = aids[0];
        for(int aid: aids) if(agents[ret].wetness < agents[aid].wetness) ret = aid;
        return ret;
    }
    int getAgentLeftmost(vi &adis){
        int ret = -1;
        int mini = 100000;
        
        for(int aid: agent_ids) if(!agents[aid].mine){
            if(agents[aid].pos.snd < mini){
                mini = agents[aid].pos.snd;
                ret = aid;
            }
        }
        return ret;
    }
    vi getAgentsIfInRange(vi &aids, pi lu, pi d, function<bool(Agent)> f){
        return getAgentsIf(aids, [lu,d,f](Agent a){
            int i = a.pos.fst;
            int j = a.pos.snd;
            return (lu.fst <= i && i < lu.fst+d.fst) && (lu.snd <= j && j < lu.snd+d.snd && f(a));
        });
    }
    int getAgentNearestWay(vi &aids, pi p){
        int ret = -1;
        int mini = 100000;
        for(int aid: aids){
            int dist = wayDist(agents[aid].pos, p, true);
            if(dist < mini) {
                mini = dist;
                ret = aid;
            }
        }
        return ret;
    }
    vi getAgentsLte(vi &aids, pi p, int d){
        vi ret;
        for(int agent: aids) {
            int dist = manhat(agents[agent].pos, p);
            if(dist <= d) ret.push_back(agent);
        }
        return ret;
    }
    int getAgentDamagemost(vi &aids, int agent){
        double maxi = -100000;
        int ret = -1;
        for(int aid: aids){
            double score = getDamageRate(agent, agents[aid].pos);
            if(maxi < score) {
                maxi = score;
                ret = aid;
            }
        }
        return ret;
    }

    //エージェントセット系
    void setTargetPos(int agent, pi target){
        //-1だったら何もしない
        if(target.fst == -1) return;
        //ターゲットに近付く移動の中で、どれか一つをランダムに選ぶ
        vpi moveTos = getNeighborsTo(agents[agent].pos, target);
        if(moveTos.size() > 0){
            int r = randint(0,moveTos.size());
            vpi cands = getNeighborsCanMove(agents[agent].pos);
            agents[agent].op.setMove(moveTos[r]);
            grid[agents[agent].pos.fst][agents[agent].pos.snd] = 0;
            grid[moveTos[r].fst][moveTos[r].snd] = agent;
            agents[agent].pos = moveTos[r];
        }
    }

    //スコア系
    pi getScores(){
        int p = 0;
        int e = 0;
        rep(i,height) rep(j,width){
            int distToPlayer = 100000;
            int distToEnemy = 100000;
            for(int aid: agent_ids){
                //aidまでのマンハッタン距離を計算
                int dist = manhat(agents[aid].pos, {i,j});
                if(agents[aid].wetness >= 50) dist *= 2;
                //味方への最小と、敵への最小をそれぞれ記録
                if(agents[aid].mine) chmin(distToPlayer, dist);
                else chmin(distToEnemy, dist);
            }
            //どっちかの方がちかいときはそっちのポイントを記録
            if(distToPlayer < distToEnemy) p++;
            else if(distToEnemy < distToPlayer) e++;
        }
        return {p,e};
    }

    //ダメージ系
    double getDamageRate(int aid, pi p){
        Agent a = agents[aid];
        vector<pair<int,double>> ds = {{HIGH_COVER,0.25}, {LOW_COVER,0.5}};
        pi p1 = a.pos;
        int dst = manhat(p1,p);
        if(dst > a.range*2) return 0;
        
        double rate = 1;
        if(manhat(p1,p) > a.range) rate = 0.5;

        for(pi d: ds){
            //右に高壁がある時
            if(p.snd+1 < width && grid[p.fst][p.snd+1] == d.fst){
                if(p.snd+2 < p1.snd) return d.snd*rate;
                if(p.snd+2 == p1.snd){
                    if(p1.fst < p.fst-1 || p.fst+1 < p1.fst) return d.snd*rate;
                }
            }
            if(p.snd-1 >= 0 && grid[p.fst][p.snd-1] == d.fst){
                if(p1.snd < p.snd-2) return d.snd * rate;
                if(p.snd-2 == p1.snd){
                    if(p1.fst < p.fst-1 || p.fst+1 < p1.fst) return d.snd * rate;
                }
            }
            if(p.fst+1 < height && grid[p.fst+1][p.snd] == d.fst){
                if(p.fst+2 < p1.fst) return d.snd * rate;
                if(p.fst+2 == p1.fst){
                    if(p1.snd < p.snd-1 || p.snd+1 < p1.snd) return d.snd * rate;
                }
            }
            if(p.fst-1 >= 0 && grid[p.fst-1][p.snd] == d.fst){
                if(p1.fst < p.fst-2) return d.snd * rate;
                if(p.fst-2 == p1.fst){
                    if(p1.snd < p.snd-1 || p.snd+1 < p1.snd) return d.snd * rate;
                }
            }
        }
        return rate;
    }
    double getTotalDamageRate(pi p, vi aids){
        double ret = 0;
        for(int aid: aids) ret += getDamageRate(aid, p);
        return ret;
    }

    //カウント系
    int countHighCover(pi p){
        int ret = 0;
        vpi neighbors = getNeighbors4(p);
        vpi ds = {{0,1}, {0,-1}, {1,0}, {-1,0}};
        for(pi nbr: neighbors){
            ret += (grid[nbr.fst][nbr.snd] == HIGH_COVER);
        }
        return ret;
    }

    //評価値系
    double getEvaluation(int agent, pi p, double defense_level, double ofence_level, double invasion_level){
        vi enemys = getEnemys(agent_ids);
        //安全度を計算(0 ~ 1)
        double safety = (double)enemys.size() - getTotalDamageRate(p, enemys);
        //pまでの距離（遠いほど価値は低い）
        //double dist = wayDist(agents[agent].pos, p, true);
        //optimal範囲内に敵がいるか？
        double optimalRate = 0;
        if(getAgentsLte(enemys, p, agents[agent].range).size() > 0) optimalRate = 1;
        else if(getAgentsLte(enemys, p, agents[agent].range*2).size() > 0) optimalRate = 0.5;
        //全体の占有度
        pi before = agents[agent].pos;
        agents[agent].pos = p;
        pi scores = getScores();
        agents[agent].pos = before;
        double score = (double)(scores.fst+1)/(scores.snd+1);
        return defense_level*safety + 3*optimalRate + 10*score*score;
    }
    pi getPositionBestEvaluation(vpi &ps, int agent, double defense_level, double ofence_level, double invasion_level){
        pi ret = {-1,-1};
        double bestScore = -100000;
        for(pi p: ps){
            double score = getEvaluation(agent, p, defense_level, ofence_level, invasion_level);
            //cerr << "cand:" << p.fst<<","<<p.snd<<" "<<score<<endl;
            if(bestScore < score){
                bestScore = score;
                ret = p;
            }
        }
        return ret;
    }

    //個別：スナイパー
    pi getPositionOptimalSniper(int agent, int away_level, int move_level, int frontEnemy){
        pi ret = {-1,-1};
        int line = agents[frontEnemy].pos.snd;

        //一番前の敵からマンハッタン距離が7以下のどこかにいく
        vpi nearPositions = getPositionsLte(allPositions, agents[agent].pos, 3);
        
        // //この中で一番評価値が高いところに居座る
        return getPositionBestEvaluation(nearPositions, agent, 9,4,10);

        //とりあえず、右に壁がある場所を出す（後ろには下がらない）
        vpi candsWall = getPositionsIf(allPositions, [line,agent](pi p){
            //右がなければアウト
            if(p.snd+1 >= width) return false;
            //壁がなければアウト
            if(grid[p.fst][p.snd+1] >= 0) return false;
            //もう埋まってたらアウト
            if(grid[p.fst][p.snd] != 0 && grid[p.fst][p.snd] != agent) return false;
            //下がるならアウト
            if(p.snd < agents[agent].pos.snd) return false;
            return true;
        });
        //その中でも良さげなやつを選ぶ
        vpi candsBetter = getPositionsIf(candsWall, [&](pi p){
            //前過ぎたらアウト
            if(line-away_level < p.snd) return false;
            //遠過ぎたらアウト
            if(move_level < wayDist(agents[agent].pos, p, true)) return false;
            //後ろに下がったらアウト
            if(p.snd < agents[agent].pos.snd) return false;
            return true;
        });
        //HighCoverに行ける候補
        vpi candsHighCover = getPositionsIf(candsBetter, [](pi p){ return grid[p.fst][p.snd+1] == HIGH_COVER; });
        //LowCoverに行ける候補
        vpi candsLowCover = getPositionsIf(candsBetter, [](pi p){ return grid[p.fst][p.snd+1] == LOW_COVER; });
        //HighCoverの中でさらに先に行ける場所があれば行く
        if(candsHighCover.size() > 0){
            ret = getPositionRightmost(candsHighCover);
            return ret;
        }
        //LowCoverでも先に行ければそこにいく
        if(candsLowCover.size() > 0){
            ret = getPositionLeftmost(candsLowCover);
            return ret;
        }
        //どれもないなら、とりあえず隠れられるところに行く
        ret = getPositionNearestWay(candsWall, agents[agent].pos, true);
        return ret;
    }

    //個別：ガンナー
    pi getPositionOptimalGunner(int agent){
        pi ret;
        //一番前の敵からマンハッタン距離が7以下のどこかにいく
        vpi nearPositions = getPositionsLte(allPositions, agents[agent].pos, 3);
        
        // //この中で一番評価値が高いところに居座る
        return getPositionBestEvaluation(nearPositions, agent, 3,6,10);
    }

    //個別：ボンバー
    pi getPositionOptimalBomber(int agent){
        pi ret;
        //一番前の敵からマンハッタン距離が7以下のどこかにいく
        vpi nearPositions = getPositionsLte(allPositions, agents[agent].pos, 3);
        
        // //この中で一番評価値が高いところに居座る
        return getPositionBestEvaluation(nearPositions, agent, 2,8,10);
    }

    //個別：アサルト
    pi getPositionOptimalAssault(int agent){
        pi ret;
        //一番前の敵からマンハッタン距離が7以下のどこかにいく
        vpi nearPositions = getPositionsLte(allPositions, agents[agent].pos, 3);
        
        // //この中で一番評価値が高いところに居座る
        return getPositionBestEvaluation(nearPositions, agent, 3,6,10);
    }

    //個別：バーサーカー
    pi getPositionOptimalBerserker(int agent){
        pi ret;
        //一番前の敵からマンハッタン距離が7以下のどこかにいく
        vpi nearPositions = getPositionsLte(allPositions, agents[agent].pos, 3);
        
        // //この中で一番評価値が高いところに居座る
        return getPositionBestEvaluation(nearPositions, agent, 1,9,10);
    }

//////////////////////////////////



string turn(int agent) {
    cerr << "turn :" << agent << endl;
    agents[agent].op.reset();

    //必要な情報を求めておく
    vi enemys = getEnemys(agent_ids);
    int frontEnemy = getAgentLeftmost(enemys);

    if(agents[agent].agent_type == SNIPER){
        int away_level = 7;
        int move_level = 5;
        if(death_count_enemy > 0){
            away_level = 2;
            move_level = 10;
        }
        pi target = getPositionOptimalSniper(agent, away_level, move_level, frontEnemy);
        setTargetPos(agent, target);

        
        //一番ダメージが通るやつにシュート
        if(agents[agent].cd == 0){
            vi nearEnemys = getAgentsLte(enemys, agents[agent].pos, agents[agent].range*1.5);
            int targetEnemy = getAgentDamagemost(nearEnemys, agent);
            if(targetEnemy != -1){
                agents[agent].op.setShoot(targetEnemy);
                agents[agent].cd = agents[agent].cooldown + 1;
            }
        }
        //できなければHUNKER_DOWN
    }
    else if(agents[agent].agent_type == GUNNER){
        pi target = getPositionOptimalGunner(agent);
        setTargetPos(agent, target);

        //一番ダメージが通るやつにシュート
        if(agents[agent].cd == 0){
            vi nearEnemys = getAgentsLte(enemys, agents[agent].pos, agents[agent].range*2);
            int targetEnemy = getAgentDamagemost(nearEnemys, agent);
            if(targetEnemy != -1){
                agents[agent].op.setShoot(targetEnemy);
                agents[agent].cd = agents[agent].cooldown + 1;
            }
        }

        //爆弾が投げれたら投げる
        pi bomPoint = getPositionOptimalBom(agents[agent].pos);
        if(agents[agent].bom > 0 && bomPoint.fst != -1){
            agents[agent].op.setThrow(bomPoint);
            agents[agent].bom--;
        }
    }
    else if(agents[agent].agent_type == BOMBER) {
        pi target = getPositionOptimalBomber(agent);
        // pi target = agents[frontEnemy].pos;
        setTargetPos(agent, target);

        //一番ダメージが通るやつにシュート
        if(agents[agent].cd == 0){
            vi nearEnemys = getAgentsLte(enemys, agents[agent].pos, agents[agent].range*2);
            int targetEnemy = getAgentDamagemost(nearEnemys, agent);
            if(targetEnemy != -1){
                agents[agent].op.setShoot(targetEnemy);
                agents[agent].cd = agents[agent].cooldown + 1;
            }
        }

        //爆弾が投げれたら投げる
        pi bomPoint = getPositionOptimalBom(agents[agent].pos);
        if(agents[agent].bom > 0 && bomPoint.fst != -1){
            agents[agent].op.setThrow(bomPoint);
            agents[agent].bom--;
        }
    }
    else if(agents[agent].agent_type == ASSAULT) {
        pi target = getPositionOptimalAssault(agent);
        setTargetPos(agent, target);

        //一番ダメージが通るやつにシュート
        if(agents[agent].cd == 0){
            vi nearEnemys = getAgentsLte(enemys, agents[agent].pos, agents[agent].range*2);
            int targetEnemy = getAgentDamagemost(nearEnemys, agent);
            if(targetEnemy != -1){
                agents[agent].op.setShoot(targetEnemy);
                agents[agent].cd = agents[agent].cooldown + 1;
            }
        }

        //爆弾が投げれたら投げる
        pi bomPoint = getPositionOptimalBom(agents[agent].pos);
        if(agents[agent].bom > 0 && bomPoint.fst != -1){
            agents[agent].op.setThrow(bomPoint);
            agents[agent].bom--;
        }
    }
    else if(agents[agent].agent_type == BERSERKER){
        pi target = getPositionOptimalBerserker(agent);
        //pi target = agents[frontEnemy].pos;
        setTargetPos(agent, target);

        //一番ダメージが通るやつにシュート
        if(agents[agent].cd == 0){
            vi nearEnemys = getAgentsLte(enemys, agents[agent].pos, agents[agent].range*2);
            int targetEnemy = getAgentDamagemost(nearEnemys, agent);
            if(targetEnemy != -1){
                agents[agent].op.setShoot(targetEnemy);
                agents[agent].cd = agents[agent].cooldown + 1;
            }
        }

        //爆弾が投げれたら投げる
        pi bomPoint = getPositionOptimalBom(agents[agent].pos);
        if(agents[agent].bom > 0 && bomPoint.fst != -1){
            agents[agent].op.setThrow(bomPoint);
            agents[agent].bom--;
        }
    }

    return agents[agent].op.getStr() + "MESSAGE " + type_names[agents[agent].agent_type] +" "+to_string(agents[agent].cd);
    //return agents[agent].op.getStr();
}

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

int main()
{
    TimeKeeper timeKeeper;
    cin >> my_id; cin.ignore();
    cerr << "my_id:" << my_id << endl;
    cin >> agent_data_count; cin.ignore();
    cerr << "agent_cound:" << agent_data_count << endl;
    for (int i = 0; i < agent_data_count; i++) {
        int agent_id; // Unique identifier for this agent
        int player; // Player id of this agent
        int shoot_cooldown; // Number of turns between each of this agent's shots
        int optimal_range; // Maximum manhattan distance for greatest damage output
        int soaking_power; // Damage output within optimal conditions
        int splash_bombs; // Number of splash bombs this can throw this game
        cin >> agent_id >> player >> shoot_cooldown >> optimal_range >> soaking_power >> splash_bombs; cin.ignore();
        agents[agent_id] = (Agent(agent_id, player==my_id, soaking_power, optimal_range, splash_bombs, shoot_cooldown));
        agents[agent_id].setAgentType();
        agent_ids.push_back(agent_id);
    }
    cin >> width >> height; cin.ignore();

    cerr << "width:" << width <<" height:"<<height << endl;
    grid = vvi(height, vi(width));
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int x; // X coordinate, 0 is left edge
            int y; // Y coordinate, 0 is top edge
            int tile_type;
            cin >> x >> y >> tile_type; cin.ignore();
            if(my_id == 1) x = width-1-x;
            grid[y][x] = -tile_type;
        }
    }

    //initWayDist();
    vb alives(agents.size());
    allPositions = getPositionsAll();

    int turnCnt = 1;
    // game loop
    while (1) {
        cerr << "turn : " << turnCnt << endl;
        turnCnt++;
        timeKeeper.start();

        //グリッド上のエージェントを一旦取り除く
        for(int agent: agent_ids){
            pi p = agents[agent].pos;
            if(p.fst == -1) continue;
            grid[p.fst][p.snd] = 0;
        }

        //各エージェントが生きているかを確認
        fill(all(alives), false);

        int agent_count;
        cin >> agent_count; cin.ignore();
        cerr << "agent_count : " << agent_count << endl;
        for (int i = 0; i < agent_count; i++) {
            int agent_id;
            int x;
            int y;
            int cooldown; // Number of turns before this agent can shoot
            int splash_bombs;
            int wetness; // Damage (0-100) this agent has taken
            cin >> agent_id >> x >> y >> cooldown >> splash_bombs >> wetness; cin.ignore();

            if(my_id == 1) x = width-1-x;
            Agent *agent = &agents[agent_id];
            agent->setPos({y,x});
            agent->bom = splash_bombs;
            agent->wetness = wetness;
            grid[y][x] = agent_id;
            alives[agent_id-1] = true;
            //cdの回復もここでやっちゃう
            if(agent->mine){
                agent->cd = max(0, agent->cd-1);
            }

        }
        
        //死んでる奴はagent_idsから消す
        vi dels;
        for(int aid: agent_ids) if(alives[aid-1] == false) dels.push_back(aid);
        for(int del_id: dels){
            cerr << del_id << " dead..." <<endl;
            if(agents[del_id].mine) death_count_player++;
            else death_count_enemy++;
            agent_ids.erase(find(all(agent_ids), del_id));
            agents.erase(del_id);
        }

        vi players = getPlayers(agent_ids);
        cerr << "players" << endl;
        for(int player: players) cerr << agents[player].pos.fst << " " << agents[player].pos.snd << endl;

        vi enemys = getEnemys(agent_ids);
        cerr << "enemys" << endl;
        for(int enemy: enemys) cerr << agents[enemy].pos.fst << " " << agents[enemy].pos.snd << endl;
        
        pi now_scores = getScores();
        int now_player_score = now_scores.fst;
        int now_enemy_score = now_scores.snd;
        total_my_score += max(0, now_player_score-now_enemy_score);
        total_enemy_score += max(0, now_enemy_score-now_player_score);
        cerr << "score:" << total_my_score<<" "<<total_enemy_score << endl;

        int my_agent_count; // Number of alive agents controlled by you
        cin >> my_agent_count; cin.ignore();

        //cerr << "timer:" << timeKeeper.now() << endl;
        //みんな、最初にいた位置からスライドして、真ん中あたりに居座ろうとする
        for(int agent: agent_ids) {
            if(!agents[agent].mine) continue;
            cout << turn(agent) << endl;
            //cerr << "time:" << timeKeeper.now() << endl;
        }

    }
}