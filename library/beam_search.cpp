#pragma once
#include "library/timekeeper.cpp"
/**
 * @brief ビームサーチ（AHC 用テンプレート。Action / State / enumActions を問題に合わせて書く）
 *
 *   Action                    … 1手の情報（中身を書く）
 *   State                     … 状態。score / adapt(a)（手を適用）/ undo()（直前の adapt を戻す）を書く
 *   bs.enumActions(s)         … s から選べる手を全列挙（中身を書く）
 *   bs.enumNextStates(s)      … s から1手進めた状態の列
 *   bs.init()                 … 初期状態を topK に入れる
 *   bs.search(BEAM_K, msec)   … msec ミリ秒経つか次の状態が無くなるまで1手ずつ進め、
 *                               score 上位 BEAM_K 個を残す。最後のビームで score 最大の State を返す
 *
 * @note 手順は init() → search()。返す State に操作列を持たせておく
 * @note score が大きいほど良い。topK は score の小さい順に取り出す優先度付きキュー
 *
 * @code
 * BeamSearch bs;
 * bs.init();
 * BeamSearch::State best = bs.search(100, 1800);
 * @endcode
 */
class BeamSearch {
public:
    struct Action {
        
    };
    struct State {
        int score = 0;

        void adapt(Action a){

        }
        void undo(){

        }
    };

    
    vector<Action> enumActions(State &s){
        //全員の中で、動いても大丈夫なのが一つでもあれば入れる。
        //全部ダメならランダム
        vector<Action> ret;
        return ret;
    }

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
    
    State search(int BEAM_K, double millsec){
        
        priority_queue<State,vector<State>,comp_pri> nextTopK;

        // 現在のビームで score 最大の状態（次の状態が無くなったときはこれを返す）
        State best;
        {
            auto tmp = topK;
            while(!tmp.empty()){
                best = tmp.top();
                tmp.pop();
            }
        }

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
            // 次の状態が1つも無ければ打ち切り（topK は空なので best を返す）
            if(nextTopK.empty()) break;
            // 小さい順に取り出すので、最後に取り出したものが score 最大
            while(!nextTopK.empty()){
                best = nextTopK.top();
                topK.push(nextTopK.top());
                nextTopK.pop();
            }
        }
        

        return best;
    }
};
