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
// 7-2
////戦略/////
// 基本的には、陣営のエージェント全てを動かすのを一手としてる
// getScoresが走りすぎるとタイムアウトするから、getNextsの最後で80個に絞ってる（ランダム）
// ボマーはどんどんボム投げて、他の人はもったいぶって投げるように変更した
// ダメージ80以上のやつを優先的に殴るようにした
// getScoresが重かったのは、for(agents:agent)の時に&つけてなかったから
// 7に、近すぎたらダメにするのを追加
// ダメージおそらく0のやつは打たない
// スナイパーの防御力も考慮
// 撃つアルゴリズムを少し変更し、打てる中で一番wetnessが高いやつを打つことにした
///////////

/////結果//////
// 待てばゴールド190位くらい
//////////////

// a以上b未満の乱数を返す
std::mt19937_64 rndmt(std::random_device{}());
int randint(int a, int b){
    std::uniform_int_distribution<int> r(a, b-1);
    return r(rndmt);
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

//グローバル変数たち
int my_id;
int height, width;
int agent_data_count;
vvi grid;
int total_my_score = 0;
int total_enemy_score = 0;
int death_count_enemy = 0;
int death_count_player = 0;
int getScoreCnt = 0;

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
    vpi getNeighborsCanMove(pi p, map<int,Agent> &ants);                              //移動可能な近傍（エージェント考慮）
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
    int getAgentMaxWetness(map<int,Agent> &ants);                           //一番濡れてるエージェントを取得
    int getAgentLeftmost(vi &aids);                             //一番左にいるエージェントを取得
    vi getAgentsIfInRange(vi &aids, pi lu, pi d, function<bool(Agent)> f);//指定した範囲内の特定のエージェントを取得(dはh,w)
    int getAgentNearestWay(vi &aids, pi p);                     //特定の場所から、一番近い場所のエージェントを取得
    vi getAgentsLte(vi &aids, pi p, int d);                     //pからマンハッタン距離d以内に敵がいるか？
    int getAgentDamagemost(vi &aids, int agent);                //aidsの中で、agentからの攻撃が一番通るやつを一つ取得

    //エージェントセット系
    void setTargetPos(int agent, pi target);

    //スコア系
    double getScore(map<int, Agent> &ants);                                             //現状のagent_idsに対して、{自分の占領地数、相手の占有地数}を返す

    //ダメージ系
    double getDamageRate(int aid, int target_id);                       //aidがpを攻撃した時のダメージ割合（高いほどよく通る）
    double getDanger(int aid, map<int,Agent> &ants);

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

struct Node {
    public:
    map<int, Agent> nowAgents;
    bool is_my_turn;
    void init(){
        nowAgents = agents;
        is_my_turn = true;
    }
    
    vector<Node> getNexts(){
        vector<Node> ret;
        map<int, vpi> top2;

        //今回操作するエージェントのid集合を作る
        vi now_agent_ids;
        for(pair<const int,Agent> &ant: nowAgents){
            if(is_my_turn == ant.snd.mine) now_agent_ids.push_back(ant.fst);
        }

        //全員が上位二つずつを選ぶ
        function<void(int)> dfs = [&](int nowIdx){
            if(nowIdx >= now_agent_ids.size()){
                ret.push_back(*this);
                return;
            }
            int now_agent_id = now_agent_ids[nowIdx];
            Agent &a = nowAgents[now_agent_id];
            //上下左右行けるところ
            vpi cands = getNeighborsCanMove(a.pos, nowAgents);
            //動かない場合も考慮
            cands.push_back(a.pos);
            //全体の中でtop2を決める
            // vpi top2;
            // vector<pair<int,pi>> scores;
            // for(pi cand: cands){
            //     pi s = getScores(nowAgents);
            //     scores.push_back({s.fst-s.snd, cand});
            // }
            // if(is_my_turn){
            //     sort(all(scores), [](pair<int,pi> t1, pair<int,pi> t2){ return t1.fst > t2.fst; });
            //     rep(i,min(2,(int)scores.size())) top2.push_back(scores[i].snd);
            // }
            // else{
            //     sort(all(scores), [](pair<int,pi> t1, pair<int,pi> t2){ return t1.fst < t2.fst; });
            //     rep(i,min(2,(int)scores.size())) top2.push_back(scores[i].snd);
            // }
            for(pi cand: cands){
                pi before = nowAgents[now_agent_id].pos;
                nowAgents[now_agent_id].pos = cand;
                is_my_turn = !is_my_turn;
                dfs(nowIdx+1);
                is_my_turn = !is_my_turn;
                nowAgents[now_agent_id].pos = before;
            }
        };

        dfs(0);
        //return ret;
        // 乱数生成器を用意する
        std::random_device seed_gen;
        std::mt19937 engine {seed_gen()};
        vector<Node> ret10;
        sample(all(ret), back_inserter(ret10), 70, engine);
        return ret10;
    }

    double myGetScore(){
        return getScore(nowAgents);
    }
};

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
            vpi cands = getNeighborsCanMove({now/width, now%width}, agents);
            for(pi cand: cands) if(dp[cand.fst*width+cand.snd] == 100000){
                dp[cand.fst*width+cand.snd] = dp[now] + 1;
                q.push(cand.fst*width+cand.snd);
            }
        }
        vpi goalNeighbors = getNeighborsCanMove(b, agents);
        int ret = 100000;
        for(pi nbr: goalNeighbors) chmin(ret, dp[nbr.fst*width+nbr.snd]);
        return ret;
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
    vpi getNeighborsCanMove(pi p, map<int, Agent> &ants){
        function<bool(pi)> have_same_pos = [&](pi pos){
            auto it = find_if(all(ants), [pos](pair<const int, Agent> &m){ return m.snd.pos == pos; });
            return it != ants.end();
        };
        vpi ret;
        if(p.fst+1 < height && (grid[p.fst+1][p.snd] == 0 && !have_same_pos({p.fst+1, p.snd}))) ret.push_back({p.fst+1, p.snd});
        if(p.fst-1 >= 0 && grid[p.fst-1][p.snd] == 0 && !have_same_pos({p.fst-1, p.snd})) ret.push_back({p.fst-1, p.snd});
        if(p.snd+1 < width && grid[p.fst][p.snd+1] == 0 && !have_same_pos({p.fst,p. snd+1})) ret.push_back({p.fst, p.snd+1});
        if(p.snd-1 >= 0 && grid[p.fst][p.snd-1] == 0 && !have_same_pos({p.fst, p.snd-1})) ret.push_back({p.fst, p.snd-1});
        return ret; 
    }
    vpi getNeighborsTo(pi p, pi target){
        if(p == target) return {};
        vpi cands = getNeighborsCanMove(p, agents);
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
            if(agents[grid[p.fst][p.snd]].wetness < 50){
                //元気な時は、2以上じゃないと投げない
                if(2 <= score && maxi < score){
                    maxi = score;
                    ret = cand;
                }
            }
            //中盤だったら、確実に当たるとこには投げる
            else {
                int tile = grid[cand.fst][cand.snd];
                if(1 <= score && maxi < score && tile > 0 && agents[tile].mine == false){
                    maxi = score;
                    ret = cand;
                }
            }
        }
        return ret;
    }
    pi getPositionOptimalBomBomber(pi p){
        vpi cands = getPositionsLte(allPositions, p, 4);
        pi ret = {-1,-1};
        int maxi = 0;
        for(pi cand: cands){
            int countPlayer = getAgentsIfInRange(agent_ids, {cand.fst-1,cand.snd-1}, {3,3}, [](Agent a){ return a.mine; }).size();
            int countEnemy = getAgentsIfInRange(agent_ids, {cand.fst-1,cand.snd-1}, {3,3}, [](Agent a){ return !a.mine; }).size();
            int score = countEnemy - countPlayer;
            int tile = grid[cand.fst][cand.snd];
            if(maxi < score){
                if(1 < score || (tile > 0 && agents[tile].mine == false)){
                    maxi = score;
                    ret = cand;
                }
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
    int getAgentMaxWetness(map<int,Agent> &ants){
        int ret = -1;
        int maxi = -100000;
        for(pair<const int,Agent> &a: agents){
            if(maxi < a.snd.wetness){
                maxi = a.snd.wetness;
                ret = a.fst;
            }
        }
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
        double maxi = 0;
        int ret = -1;
        for(int aid: aids){
            double score = getDamageRate(agent, aid);
            if(maxi < score) {
                maxi = score;
                ret = aid;
            }
        }
        if(agents[agent].agent_type == SNIPER){
            if(maxi != 1) ret = -1;
        }
        return ret;
    }
    int getAgentCanKill(vi &aids, int agent){
        int ret = -1;
        double maxW = 0;
        double maxi = 0;
        for(int aid: aids){
            //int damage = round(getDamageRate(agent, agents[aid].pos) * agents[agent].power);
            double score = getDamageRate(agent, aid);
            if(agents[aid].wetness >= 70) {
                if(maxW < agents[aid].wetness && score != 0){
                    maxi = score;
                    ret = aid;
                    maxW = agents[aid].wetness;
                }
            }
        }
        return ret;
    }

    //エージェントセット系
    void setTargetPos(int agent, pi target){
        //-1だったら何もしない
        if(target.fst == -1) return;

        agents[agent].op.setMove(target);
        grid[agents[agent].pos.fst][agents[agent].pos.snd] = 0;
        grid[target.fst][target.snd] = agent;
        agents[agent].pos = target;
        
    }

    //スコア系
    double getScore(map<int,Agent> &ants){
        getScoreCnt++;
        int p = 0;
        int e = 0;
        // // gmap[{i,j}] := {最短距離,誰の領土か(-1相手、0同点, 1自分)}
        // map<pi,pi> gmap;
        // //各エージェントの周り9*9についてのみ考える(遠くはあんまり考えない)
        // for(pair<const int, Agent> &agent: ants){
        //     pi pos = agent.snd.pos;
        //     rep(i,max(0, pos.fst-4), min(height,pos.fst+4+1)){
        //         rep(j,max(0,pos.snd-4), min(width,pos.snd+4+1)){
        //             int dist = manhat(pos, {i,j});
        //             int newOwner = agent.snd.mine ? 1:-1;
        //             //この場所が初めての時
        //             if(gmap.count({i,j}) == 0){
        //                 gmap[{i,j}] = {dist, newOwner};
        //                 if(agent.snd.mine) p++;
        //                 else e++;
        //             }
        //             //すでにみたことある時
        //             else{
        //                 //最小が同じ時
        //                 if(dist == gmap[{i,j}].fst){
        //                     //同点になるかも
        //                     if(gmap[{i,j}].snd != 0 && gmap[{i,j}].snd != newOwner){
        //                         if(gmap[{i,j}].snd == 1) p--;
        //                         else e--;
        //                         gmap[{i,j}].snd = 0;
        //                     }
        //                 }
        //                 else if(dist < gmap[{i,j}].fst){
        //                     if(gmap[{i,j}].snd == 1) p--;
        //                     else e--;
        //                     gmap[{i,j}].fst = dist;
        //                     gmap[{i,j}].snd = newOwner;
        //                     if(gmap[{i,j}].snd == 1) p++;
        //                     else e++;
        //                 }
        //             }
        //         }
        //     }
        // }
        rep(i,height) rep(j,width){
            int distToPlayer = 100000;
            int distToEnemy = 100000;
            for(pair<const int,Agent> &agent: ants){
                //aidまでのマンハッタン距離を計算
                int dist = manhat(agent.snd.pos, {i,j});
                if(agent.snd.wetness >= 50) dist *= 2;
                //味方への最小と、敵への最小をそれぞれ記録
                if(agent.snd.mine) chmin(distToPlayer, dist);
                else chmin(distToEnemy, dist);
            }
            //どっちかの方がちかいときはそっちのポイントを記録
            if(distToPlayer < distToEnemy) p++;
            else if(distToEnemy < distToPlayer) e++;
        }
        double territory = (double)(p-e);
        //近すぎたらダメ
        //近くにはいない方がいい
        vi players = getPlayers(agent_ids);
        int cnt = 0;
        rep(i,players.size()) rep(j,i+1,players.size()){
            if(manhat(ants[players[i]].pos, ants[players[j]].pos) <= 3){
                cnt++;
            }
        }
        //スナイパーの防御力考慮
        double safety = 0;
        map<int,Agent> enemys;
        for(pair<const int, Agent> &a: ants) if(a.snd.mine == false) enemys[a.fst] = a.snd;
        for(pair<const int,Agent> &a: ants) {
            safety -= getDanger(a.fst, enemys);
        }
        //死にかけとの距離が近いほどいい
        int dying_id = getAgentMaxWetness(enemys);
        //dying_idを攻撃できるやつの個数
        double attack = 0;
        for(pair<const int,Agent> &a: ants) if(a.snd.mine){
            attack -= manhat(a.snd.pos, ants[dying_id].pos) * 0.2;
        }
        return territory + safety*3 - cnt*10;
    }
    int getScoreMinMax(Node &node, int depth){
        //cerr << "depth:" << depth << endl;
        if(depth == 0){
            int score = node.myGetScore();
            return score;
        }
        vector<Node> nextNodes = node.getNexts();
        //自分が動く時は最大化したい
        if(node.is_my_turn == true){
            int maxi = -100000;
            for(Node &nextNode: node.getNexts()){
                chmax(maxi, getScoreMinMax(nextNode, depth+1));
            }
            return maxi;
        }
        else{
            int mini = 100000;
            for(Node &nextNode: node.getNexts()){
                chmin(mini, getScoreMinMax(nextNode, depth+1));
            }
            return mini;
        }
    }
    map<int,pi> getPositionsMaxScore(Node &node){
        int maxi = -100000;
        map<int,pi> ret;
        for(pair<const int, Agent> &m: agents) if(m.snd.mine) ret[m.fst] = m.snd.pos;
        vector<Node> nextNodes = node.getNexts();
        cerr << "nextSize:" << nextNodes.size() << endl;
        for(Node &nextNode: nextNodes){
            // cerr << "-----------cand-----------" << endl;
            // for(pair<const int,Agent> &m: nextNode.nowAgents){
            //     cerr << m.fst<<" : " << m.snd.pos.fst<<","<<m.snd.pos.snd<<endl;
            // }
            int score = getScoreMinMax(nextNode, 0);
            //cerr << "score : " << score << endl;
            if(maxi < score){
                maxi = score;
                for(pair<const int, Agent> &m: agents) if(m.snd.mine) ret[m.fst] = nextNode.nowAgents[m.fst].pos;
            }
        }

        return ret;
    }

    //ダメージ系
    double getDamageRate(int aid, int target_id){
        Agent &a = agents[aid];
        Agent &target = agents[target_id];
        vector<pair<int,double>> ds = {{HIGH_COVER,0.25}, {LOW_COVER,0.5}};
        pi p = agents[target_id].pos;
        pi p1 = a.pos;
        int dst = manhat(p1,p);
        if(dst > a.range*2) return 0;
        
        //HUNKER_DOWNを考慮
        if(target.cd != 0){
            ds[0].snd -= 0.25;
            ds[1].snd -= 0.25;
        }
        
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
    double getDanger(int aid, map<int,Agent> &ants){
        double ave = 0;
        double maxi = 0;
        for(pair<const int,Agent> agent: ants) if(agent.snd.mine != ants[aid].mine){
            double damage = getDamageRate(agent.fst, aid) * agent.snd.power;
            ave += damage;
            chmax(maxi, damage);
        }
        ave /= ants.size();
        return ave*0.05 + maxi*0.95;
    }
    
/////////////////////////////////////



string turn(int agent, map<int,pi> &targetMoves) {
    cerr << "turn :" << agent << endl;
    cerr << "bomb: " << agents[agent].bom << endl;
    agents[agent].op.reset();

    //まず移動オペレーション
    setTargetPos(agent, targetMoves[agent]);
    

    //爆弾が投げれたら投げる
    pi bomPoint;
    if(agents[agent].agent_type == BOMBER) bomPoint = getPositionOptimalBomBomber(agents[agent].pos);
    else bomPoint = getPositionOptimalBom(agents[agent].pos);
    if(agents[agent].bom > 0 && bomPoint.fst != -1){
        agents[agent].op.setThrow(bomPoint);
        agents[agent].bom--;
    }
    //爆弾が投げれない時
    else{
        //一番ダメージが通るやつにシュート
        if(agents[agent].cd == 0){
            vi enemys = getEnemys(agent_ids);
            //確キルできるなら確キル
            int targetEnemy = getAgentCanKill(enemys, agent);
            if(targetEnemy != -1){
                agents[agent].op.setShoot(targetEnemy);
                agents[agent].cd = agents[agent].cooldown + 1;
            }
            else{
                targetEnemy = getAgentDamagemost(enemys, agent);
                if(targetEnemy != -1){
                    agents[agent].op.setShoot(targetEnemy);
                    agents[agent].cd = agents[agent].cooldown + 1;
                }
            }
        }
    }
    string ret = agents[agent].op.getStr();
    ret += "MESSAGE " + type_names[agents[agent].agent_type] + " " + to_string(agents[agent].wetness) + " ";
    mrep(i, agents[agent].cooldown-1) {
        if(i >= agents[agent].cd) ret += "#";
        else ret += "-";
    }
    ret += " ";
    rep(i, agents[agent].bom){
        ret += "O";
    }
    //return agents[agent].op.getStr();
    return ret;
}

int main()
{
    TimeKeeper timeKeeper;
    cin >> my_id; cin.ignore();
    cerr << "my_id:" << my_id << endl;
    cin >> agent_data_count; cin.ignore();
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

    // game loop
    while (1) {
        getScoreCnt = 0;
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
            agent->cd = cooldown;
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

        std::random_device seed_gen;
        std::mt19937 engine(seed_gen());
        //shuffle(all(agent_ids), engine);
        

        int my_agent_count; // Number of alive agents controlled by you
        cin >> my_agent_count; cin.ignore();
        
        //まずはみんな移動
        Node node;
        node.init();
        map<int, pi> targetMoves = getPositionsMaxScore(node);

        for(int agent: agent_ids) {
            if(!agents[agent].mine) continue;
            cout << turn(agent, targetMoves) << endl;
            cerr << "getScoreCnt:" << getScoreCnt << endl;
        }

    }
}