
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

//ビームサーチ
//Initで初期設定をしてから
class BeamSearch {
public:
    struct Action {
        
    };
    //状態用の構造体
    struct State {
        int score = 0;

        void adapt(Action a){

        }
        void undo(){

        }
    };

    
    //現状の状態から選べる全ての行動
    vector<Action> enumActions(State &s){
        //全員の中で、動いても大丈夫なのが一つでもあれば入れる。
        //全部ダメならランダム
        vector<Action> ret;
        return ret;
    }

    //初期状態を設定
    //topKの初期値を入れる
    //スコアが小さい順に消していく
    struct comp_pri{
        bool operator()(const State &p1,const State &p2) const{
        return p1.score > p2.score;
        }
    };
    priority_queue<State,vector<State>,comp_pri> topK;
    void init(){
        State firstState;
        topK.push(firstState);
    }

    //現在の状態から1進めた状態の集合を返す
    vector<State> enumNextStates(State &s){
        vector<Action> actions = enumActions(s);
        vector<State> ret;
        for(Action action: actions){
            s.adapt(action);
            ret.push_back(s);
            s.undo();
        }
        return ret;
    }
    
    //深さDまで探索（深さ0は初期状態）
    //最終的に最大スコアとなるようなStateを返す(State内にOP列を持っておく)
    State search(int BEAM_K, double millsec){
        
        priority_queue<State,vector<State>,comp_pri> nextTopK;

        TimeKeeper timekeeper;
        timekeeper.start();
        int cnt = 0;
        while(timekeeper.now() <= millsec){
            cnt++;
            //cerr<<"cnt: "<<cnt<<endl;
            while(!topK.empty()){
                State now = topK.top();
                topK.pop();
                vector<State> nextStates = enumNextStates(now);
                for(State nextState: nextStates){
                    nextTopK.push(nextState);
                    if(nextTopK.size() > BEAM_K) nextTopK.pop();
                }
            }
            while(!nextTopK.empty()){
                topK.push(nextTopK.top());
                nextTopK.pop();
            }
        }
        

        return topK.top();
    }
};
