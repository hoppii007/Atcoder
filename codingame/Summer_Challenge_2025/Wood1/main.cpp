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

struct Agent {
    int agent_id;
    bool mine;
    pi pos;
    int wetness;
    int power;
    int range;
    int bom;
    bool ace = false;
    bool operator==(const Agent &other) const {
        return agent_id == other.agent_id;
    }
};

//グローバル変数たち
int height,width;
vvi grid;
vi agent_ids;
map<int, Agent> agents;

const int LOW_COVER = -1;
const int HIGH_COVER = -2;



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

//aがp2に攻撃した時の、軽減率を計算(75 or 50)
int getDamageReduction(Agent a, pi p2){

    vpi ds = {{HIGH_COVER,75}, {LOW_COVER,50}};
    pi p1 = a.pos;
    if(dist(p1,p2) > a.range) return 100;

    for(pi d: ds){
        //右に高壁がある時
        if(p2.snd+1 < width && grid[p2.fst][p2.snd+1] == d.fst){
            if(p2.snd+2 < p1.snd) return d.snd;
            if(p2.snd+2 == p1.snd){
                if(p1.fst < p2.fst-1 || p2.fst+1 < p1.fst) return d.snd;
            }
        }
        if(p2.snd-1 >= 0 && grid[p2.fst][p2.snd-1] == d.fst){
            if(p1.snd < p2.snd-2) return d.snd;
            if(p2.snd-2 == p1.snd){
                if(p1.fst < p2.fst-1 || p2.fst+1 < p1.fst) return d.snd;
            }
        }
        if(p2.fst+1 < height && grid[p2.fst+1][p2.snd] == d.fst){
            if(p2.fst+2 < p1.fst) return d.snd;
            if(p2.fst+2 == p1.fst){
                if(p1.snd < p2.snd-1 || p2.snd+1 < p1.snd) return d.snd;
            }
        }
        if(p2.fst-1 >= 0 && grid[p2.fst+1][p2.snd] == d.fst){
            if(p1.fst < p2.fst-2) return d.snd;
            if(p2.fst-2 == p1.fst){
                if(p1.snd < p2.snd-1 || p2.snd+1 < p1.snd) return d.snd;
            }
        }
    }
    return 0;
}

//全員から受けるかもしれないダメージ軽減の合計(高いほどいい)
int getTotalDamageReduction(pi p, vector<Agent> enemys){
    int ret = 0;
    for(Agent enemy: enemys) ret += getDamageReduction(enemy, p);
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

int main()
{

    int my_id; // Your player id (0 or 1)
    cin >> my_id; cin.ignore();
    int agent_count; // Total number of agents in the game
    cin >> agent_count; cin.ignore();
    for (int i = 0; i < agent_count; i++) {
        int agent_id; // Unique identifier for this agent
        int player; // Player id of this agent
        int shoot_cooldown; // Number of turns between each of this agent's shots
        int optimal_range; // Maximum manhattan distance for greatest damage output
        int soaking_power; // Damage output within optimal conditions
        int splash_bombs; // Number of splash bombs this can throw this game
        cin >> agent_id >> player >> shoot_cooldown >> optimal_range >> soaking_power >> splash_bombs; cin.ignore();
        agent_ids.push_back(agent_id);
        agents[agent_id] = Agent{agent_id, player==my_id, {-1,-1}, -1, soaking_power, optimal_range, splash_bombs};
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

            agents[agent_id].pos = {y,x};
            agents[agent_id].bom = splash_bombs;
            agents[agent_id].wetness = wetness;
            grid[y][x] = agent_id;
            if(splash_bombs == 3) {
                agents[agent_id].ace = true;
            }
        }

        int my_agent_count; // Number of alive agents controlled by you
        cin >> my_agent_count; cin.ignore();
        
        for(int agent_id: agent_ids) {
            Agent agent = agents[agent_id];
            if(agent.mine == false) continue;
            if(agent.wetness >= 100) continue;

            //ボムを持ってないやつはなにもしなくていい
            if(!agent.ace) {
                cout<< agent_id << ";HUNKER_DOWN" << endl;
                continue;
            }
            
            //自分がいなくて、敵だけいる区画を探す
            function<bool(pi,pi)> isOK = [&](pi lu, pi d){
                int cntPlayer = countIfAgentsInRange(lu,d,[](Agent a){ return a.mine; });
                int cntEnemy = countIfAgentsInRange(lu,d,[](Agent a){ return !a.mine; });
                if(cntPlayer > 0) return false;
                if(cntEnemy == 0) return false;
                return true;
            };

            if(isOK({1,1}, {3,3})){
                cout << agent_id << ";MOVE 6 2;THROW 2 2" << endl;
            }
            else if(isOK({height-4,1}, {3,3})){
                cout << agent_id << ";MOVE 6 "<<(height-3)<<";THROW 2 " << (height-3) << endl;
            }
            else if(isOK({height-4,width-4},{3,3})){
                cout << agent_id << ";MOVE "<<(width-7)<<" "<<(height-3)<<";THROW "<<(width-3)<<" " << (height-3) << endl;
            }
            else if(isOK({1,width-4}, {3,3})){
                cout << agent_id << ";MOVE "<<(width-7)<<" 2;THROW "<<(width-3)<<" 2" << endl;
            }
            //cout << agent_id << ";MOVE 6 2;THROW 2 2" << endl;


        }

    }
}