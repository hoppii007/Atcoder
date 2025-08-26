
//MonteCarloTreeSearch
//現状はマルバツゲームのAIになってる
namespace MonteCarloTreeSearch{
    
    random_device rd;       //乱数生成用のやつ
    mt19937 gen;            //乱数生成用のやつ
    int TOTAL_SIMULATION_NUM = 0;

    //行動の情報を構造体で記録
    struct Action{
        ll i,c;
    };
    //ゲームの盤面（木のノード）
    struct State {
        int n = 0;                  //選ばれた回数
        double w = 0;               //累計評価（勝った回数とか、最終的な最大スコアとか、最大化したい数値の累積）
        double nowW = 0;            //現在の盤面でのスコア
        bool isExpand = false;      //展開されたか？
        bool isFinished = false;    //ゲームが終了しているか？
        vector<State> children;     //次の盤面
        Action preAction;           //どのアクションでここにきたか？
        
        int SIMU_DEPTH = 20;        //シミュレーションの時、どれだけ深く潜るか
        int EXPAND_THRESHOLD = 10;  //何回訪問したら展開するか？
        
        ///////この辺でアドホックな盤面情報を入れていく///////////
        vi grid;
        int turn;
        bool preIsFinished;
        int preI;
        Action prePreAction;



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
            prePreAction = preAction;
            preAction = action;
            turn = (turn==1?-1:1);
            vvi lines = {{0,1,2}, {3,4,5}, {6,7,8}, {0,3,6}, {1,4,7}, {2,5,8}, {0,4,8}, {2,4,6}};
            for(vi &line: lines) if(grid[line[0]] == grid[line[1]] && grid[line[1]] == grid[line[2]] && grid[line[0]] != 0){
                isFinished = true;
                if(grid[line[0]] == 1)  nowW = 1;
                else nowW = -1;
            }
            int zcnt = 0;
            rep(i,9) if(grid[i] == 0) zcnt++;
            if(zcnt == 0){
                nowW = 0;
                isFinished = true;
            }
        }
        //直前のapplyActionを取り消す
        void undo(){
            isFinished = false;
            grid[preI] = 0;
            turn = (turn==1?-1:1);
            preAction = prePreAction;
        }

        vector<Action> getActions(){
            vector<Action> ret;
            rep(i,9) if(grid[i] == 0) ret.push_back(Action{i,turn});
            return ret;
        }


        double getEvaluation(){
            //UTC1
            return w/(n+1) + sqrt(2*log(TOTAL_SIMULATION_NUM+1)/(n+1));
        }

        //childrenに展開
        void expansion(){
            vector<Action> actions = getActions();
            for(Action &action: actions){
                applyAction(action);
                children.push_back(*this);
                undo();
            }
            isExpand = true;
        }

        //子供の中で、uct1値が一番大きいものを選ぶ
        //選んで評価値を出して、その評価値を返すところまでやる
        double selection(){
            //葉っぱにたどり着いたらsimulationスタート
            if(isExpand == false){
                double ret = simulation(0);
                n++;
                w += ret;
                return ret;
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
            double ret = nextChild->selection();
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
            State nextChild = *this;
            double ret = nextChild.simulation(depth+1);
            undo();
            return ret;
        }

        Action getActionMaxEvaluation(){
            vector<Action> actions = getActions();
            expansion();
            TOTAL_SIMULATION_NUM = 0;
            rep(i,600){
                selection();
                TOTAL_SIMULATION_NUM++;
            }
            Action ret;
            double maxi = -100000;
            for(State &child: children){
                if(maxi < child.w / child.n){
                    maxi = child.w / child.n;
                    ret = child.preAction;
                }
            }
            return ret;
        }
    };
};
