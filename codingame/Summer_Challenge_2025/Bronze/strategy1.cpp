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

// a以上b未満の乱数を返す
std::mt19937_64 rndmt(std::random_device{}());
int randll(int a, int b){
    std::uniform_int_distribution<int> r(a, b-1);
    return r(rndmt);
}

struct Agent {
    int agent_id;
    bool mine;
    pi pos;
    int wetness;
    int power;
    int range;
    int bom;
    int cooldown;
    bool ace = false;
    int cd = 0;
    bool operator==(const Agent &other) const {
        return agent_id == other.agent_id;
    }
};

//グローバル変数たち
int height,width;
int agent_data_count;
vvi grid;
vector<Agent> agents;

const int LOW_COVER = -1;
const int HIGH_COVER = -2;


vector<Agent> filterAgents(function<bool(Agent)> f){
    vector<Agent> ret;
    for(Agent a: agents){
        if(f(a)) ret.push_back(a);
    }
    return ret;
}
Agent getHighestWetness(vector<Agent> agents){
    Agent ret = agents[0];
    for(Agent agent: agents) if(ret.wetness < agent.wetness) ret = agent;
    return ret;
}
int dist(pi a, pi b){
    return abs(a.fst-b.fst) + abs(a.snd-b.snd);
}
//移動可能な座標全体を返す
vpi getCanMoveNeighbors(pi p){
    vpi ret;
    if(p.fst+1 < height && grid[p.fst+1][p.snd] == 0) ret.push_back({p.fst+1, p.snd});
    if(p.fst-1 >= 0 && grid[p.fst-1][p.snd] == 0) ret.push_back({p.fst-1, p.snd});
    if(p.snd+1 < width && grid[p.fst][p.snd+1] == 0) ret.push_back({p.fst, p.snd+1});
    if(p.snd-1 >= 0 && grid[p.fst][p.snd-1] == 0) ret.push_back({p.fst, p.snd-1});
    return ret; 
}
//移動可能な座標の中で、特定の座標に近づく場所
vpi getCanMoveNeighborsApproachingTargetPos(pi p, pi target){
    vpi cands = getCanMoveNeighbors(p);
    vpi ret;
    int d = dist(p, target);
    for(pi cand: cands) if(dist(cand, target) < d) ret.push_back(cand);
    return ret; 
}

//座標の周りの高壁の数
int countHighCover(pi p){
    int ret = 0;
    vpi ds = {{0,1}, {0,-1}, {1,0}, {-1,0}};
    for(pi d: ds){
        if(p.fst+d.fst < 0) continue;
        if(p.fst+d.fst >= height) continue;
        if(p.snd+d.snd < 0) continue;
        if(p.snd+d.snd >= width) continue;
        ret += (grid[p.fst+d.fst][p.snd+d.snd] == HIGH_COVER);
    }
    return ret;
}

//aがp2に攻撃した時の、ダメージ割合を計算
double getDamageRate(Agent a, pi p2){

    vector<pair<int,double>> ds = {{HIGH_COVER,0.75}, {LOW_COVER,0.5}};
    pi p1 = a.pos;
    int dst = dist(p1,p2);
    if(dst > a.range*2) return 0;
    
    double rate = 1;
    if(dist(p1,p2) > a.range) rate = 0.5;

    for(pi d: ds){
        //右に高壁がある時
        if(p2.snd+1 < width && grid[p2.fst][p2.snd+1] == d.fst){
            if(p2.snd+2 < p1.snd) return d.snd*rate;
            if(p2.snd+2 == p1.snd){
                if(p1.fst < p2.fst-1 || p2.fst+1 < p1.fst) return d.snd*rate;
            }
        }
        if(p2.snd-1 >= 0 && grid[p2.fst][p2.snd-1] == d.fst){
            if(p1.snd < p2.snd-2) return d.snd * rate;
            if(p2.snd-2 == p1.snd){
                if(p1.fst < p2.fst-1 || p2.fst+1 < p1.fst) return d.snd * rate;
            }
        }
        if(p2.fst+1 < height && grid[p2.fst+1][p2.snd] == d.fst){
            if(p2.fst+2 < p1.fst) return d.snd * rate;
            if(p2.fst+2 == p1.fst){
                if(p1.snd < p2.snd-1 || p2.snd+1 < p1.snd) return d.snd * rate;
            }
        }
        if(p2.fst-1 >= 0 && grid[p2.fst-1][p2.snd] == d.fst){
            if(p1.fst < p2.fst-2) return d.snd * rate;
            if(p2.fst-2 == p1.fst){
                if(p1.snd < p2.snd-1 || p2.snd+1 < p1.snd) return d.snd * rate;
            }
        }
    }
    return rate;
}

//全員から受けるかもしれないダメージ軽減の合計(高いほど攻撃がよく通る)
double getTotalDamageReduction(pi p, vector<Agent> enemys){
    double ret = 0;
    for(Agent enemy: enemys) ret += getDamageRate(enemy, p);
    return ret;
}

//指定した範囲にいるエージェントの中で、特定の条件を満たすものを数える
int countIfAgentsInRange(pi lu, pi d, function<bool(Agent)> f){
    int ret = 0;
    rep(i,lu.fst,min(height,lu.fst+d.fst)){
        rep(j,lu.snd,min(width,lu.snd+d.snd)){
            if(grid[i][j] <= 0) continue;
            ret += f(agents[grid[i][j]]);
        }
    }
    return ret;
}

// 戦術1 前線ゴリゴリ上げ
// とりあえずみんな、自分の行の真ん中に行く（戦線を上げる）
// 少しでもダメージ通るならブッパ
// 爆弾も、4以内に相手がいるならどんどん投げる
// 爆弾優先で投げる

// 戦術2 弱いやつから倒していく
// 一番wetnessが高いやつに特攻
// 全ての行ける場所の中で、こいつへのダメージが一番通るところにいく
// 防御のことは考えない
// これはダメそう

// 戦術3 最前線の壁に隠れつつ打つ
// 敵が入ってきていない壁で、防御できる場所に隠れて、手前のやつを撃ちまくる

int main()
{

    int my_id; // Your player id (0 or 1)
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
        agent_id--;
        agents.push_back(Agent{agent_id, player==my_id, {-1,-1}, -1, soaking_power, optimal_range, splash_bombs, shoot_cooldown});
    }
    cin >> width >> height; cin.ignore();
    grid = vvi(height, vi(width));
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int x; // X coordinate, 0 is left edge
            int y; // Y coordinate, 0 is top edge
            int tile_type;
            cin >> x >> y >> tile_type; cin.ignore();
            grid[y][x] = -tile_type;
        }
    }


    // game loop
    while (1) {

        //グリッド上のエージェントを一旦取り除く
        rep(i,height) rep(j,width) if(grid[i][j] > 0) grid[i][j] = 0;

        //各エージェントが生きているかを確認
        vb alives(agent_data_count, false);

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

            agent_id--;
            agents[agent_id].pos = {y,x};
            agents[agent_id].bom = splash_bombs;
            agents[agent_id].wetness = wetness;
            grid[y][x] = agent_id;
            alives[agent_id] = true;
        }
        
        //死んでる奴はagent_idsから消す
        vi dels;
        for(Agent &agent: agents) if(alives[agent.agent_id] == false) dels.push_back(agent.agent_id);
        for(int del_id: dels){
            cerr << del_id+1 << " dead..." <<endl;
            agents.erase(find(all(agents), Agent{del_id}));
        }

        int my_agent_count; // Number of alive agents controlled by you
        cin >> my_agent_count; cin.ignore();

        //まずはみんなcdが1回復する
        for(Agent &agent: agents) if(agent.mine){
            agent.cd = max(0, agent.cd-1);
        }

        //みんな、最初にいた位置からスライドして、真ん中あたりに居座ろうとする
        vector<Agent> playerAgents = filterAgents([](Agent a){ return a.mine; });
        vector<Agent> enemyAgents = filterAgents([](Agent a){ return !a.mine; });
        cerr << "AlivePlayers: " << playerAgents.size() << endl;
        for(Agent &agent: playerAgents) {
            
            //最初のID表示
            cout<< agent.agent_id+1 <<";";

            ////////////移動フェーズ///////////////

            //だんだん真ん中に行きたい
            //特定の場所に近づくための手
            vpi cands = getCanMoveNeighbors(agent.pos);
            int now_dist = dist(agent.pos, {agent.pos.fst, width/2});

            //移動キャンセルが起こらないところを選ぶ
            //どこを選んでもダメな場合は移動しない
            pi move_pos = {-1,-1};
            for(pi cand: cands){
                if(now_dist > dist(cand, {agent.pos.fst, width/2})) move_pos = cand;
                //else if(move_pos.fst == -1) move_pos = cand;
            }
            if(move_pos.fst != -1){
                cout <<"MOVE " << move_pos.snd << " " << move_pos.fst << ";";
                grid[agent.pos.fst][agent.pos.snd] = 0;
                grid[move_pos.fst][move_pos.snd] = agent.agent_id;
            }
            else{
                cout <<"MOVE " << width/2 << " " << agent.pos.fst << ";";
            }
            

            /////////////攻撃フェーズ////////////////

            //近くにいたら投げる
            pi bom_target_pos = {-1,-1};
            for(Agent &enemy: enemyAgents) {
                if(dist(agent.pos, enemy.pos) > 4) continue;
                if(agent.bom > 0){
                    agent.bom--;
                    bom_target_pos = enemy.pos;
                }
            }
            if(bom_target_pos.fst != -1){
                cout << "THROW " << bom_target_pos.snd <<" " <<bom_target_pos.fst<<";";
            }
            //近くにいなければ誰かを攻撃
            else{
                //ダメージが0.5以上通る奴がいたら、通す
                int target_id = -1;
                for(Agent &enemy: enemyAgents) {
                    double damageRate = getDamageRate(agent, enemy.pos);
                    //double damageRate = 1;
                    if(0 < damageRate){
                        target_id = enemy.agent_id;
                    }
                }
                if(target_id != -1 && agent.cd == 0){
                    cout <<"SHOOT " << target_id+1 << ";";
                    agent.cd = agent.cooldown;
                }
                else{
                    cout <<"HUNKER_DOWN" << ";";
                }
            }
            cout<<endl;
        }

    }
}