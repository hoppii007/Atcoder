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
// モンテカルロ木探索
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


//MonteCarloTreeSearch
namespace MonteCarloTreeSearch{
    
    random_device rd;       //乱数生成用のやつ
    mt19937 gen;            //乱数生成用のやつ
    //行動の情報を構造体で記録
    struct Action{
        ll i,c;
    };
    //ゲームの盤面（木のノード）
    struct State {
        int n = 0;              //選ばれた回数
        double w = 0;           //累計評価（勝った回数とか、最終的な最大スコアとか、最大化したい数値の累積）
        double nowW = 0;        //現在の盤面でのスコア
        bool isExpand = false;  //展開されたか？
        bool isFinished = false;//ゲームが終了しているか？
        vector<State> children; //次の盤面
        
        int SIMU_DEPTH = 2;     //シミュレーションの時、どれだけ深く潜るか
        
        ///////この辺でアドホックな盤面情報を入れていく///////////
        vi grid;
        int turn;
        bool preIsFinished;
        int preI;



        State(){
            gen = mt19937(rd());
            grid = vi(9, 0);
            turn = 1;
        }

        //isFinishedの設定もしておく
        //nowWの設定もしておく
        void applyAction(Action &action){
            if(isFinished) return;
            preIsFinished = false;
            grid[action.i] = action.c;
            preI = action.i;
            turn = (turn==1?-1:1);
            vvi lines = {{0,1,2}, {3,4,5}, {6,7,8}, {0,3,6}, {1,4,7}, {2,5,8}, {0,4,8}, {2,4,6}};
            for(vi &line: lines) if(grid[line[0]] == grid[line[1]] && grid[line[1]] == grid[line[2]] && grid[line[0]] != 0){
                isFinished = true;
                if(grid[line[0]] == 1)  nowW = 1;
                else nowW = -1;
            }
        }
        //直前のapplyActionを取り消す
        void undo(){
            isFinished = false;
            grid[preI] = 0;
            turn = (turn==1?-1:1);
        }

        vector<Action> getActions(){
            vector<Action> ret;
            rep(i,9) if(grid[i] == 0) ret.push_back(Action{i,turn});
            return ret;
        }


        double getEvaluation(){
            //UTC1
            return w/n + sqrt(2*log(n)/n);
        }

        //childrenに展開
        void expansion(){
            isExpand = true;
            vector<Action> actions = getActions();
            for(Action &action: actions){
                applyAction(action);
                children.push_back(*this);
                undo();
            }
        }

        //子供の中で、uct1値が一番大きいものを選ぶ
        //選んで評価値を出して、その評価値を返すところまでやる
        double selection(){
            //葉っぱにたどり着いたらsimulationスタート
            if(isExpand == false){
                return simulation(0);
            }
            double maxEvaluation = children[0].getEvaluation();
            State *nextChild = &children[0];
            for(State &child: children){
                double eval = child.getEvaluation();
                if(maxEvaluation < eval){
                    maxEvaluation = eval;
                    nextChild = &child;
                }
            }
            n++;
            double ret = (*nextChild).selection();
            w += ret;
            return ret;
        }

        //基本的にランダムでシミュレーション（高速で貪欲っぽくいい手を探せるならそれをやってもいい）
        double simulation(int depth){
            //ゲームが終わってるなら、勝敗なり、スコアなりを返す
            if(isFinished || depth == SIMU_DEPTH){
                return nowW;
            }
            //終わってないなら、終わるまでとか、特定の時間・深さになるまで潜っていく
            vector<Action> actions = getActions();
            uniform_int_distribution<> rnd(0, actions.size() - 1);
            Action &action = actions[rnd(gen)];

            //潜った先の結果を返す
            applyAction(action);
            double ret = simulation(depth+1);
            undo();
            return ret;
        }
    };
};

using namespace MonteCarloTreeSearch;
int main()
{
    State s;
    vector<Action> actions = s.getActions();
    s.applyAction(actions[0]);
    actions = s.getActions();
    for(Action action: actions) cout<<action.i<<" "<<action.c<<endl;
    return 0;
}