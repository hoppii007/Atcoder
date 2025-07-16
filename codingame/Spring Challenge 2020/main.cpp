// Wood div 1のトップ解法
// ぶつかった時の処理をちゃんとしないとPINKYに勝てない
// とりあえず全員が最短の10ペレット（なければ1ペレット）を目指して進む。ぶつかったらターゲットをランダムにする
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>
#include <map>
#include <random>

#define rep(i,n) for(int i=0;i<n;i++)
#define fst first
#define snd second

using namespace std;
using vb = vector<bool>;
using vvb = vector<vector<bool>>;
using vi = vector<int>;
using vvi = vector<vector<int>>;
using pi = pair<int,int>;
using vpi = vector<pi>;
/**
 * Grab the pellets as fast as you can!
 **/

vvb grid;
int width;
int height;
const int inf = 10000000;

vpi get_neighbors(pi p){
    vpi ret;

    //上移動
    if(p.fst-1 >= 0 && grid[p.fst-1][p.snd]) ret.push_back({p.fst-1, p.snd});
    //右移動
    int r = (p.snd+1)%width;
    if(grid[p.fst][r]) ret.push_back({p.fst, r});
    //したいどう
    if(p.fst+1 < height && grid[p.fst+1][p.snd]) ret.push_back({p.fst+1, p.snd});
    //左移動
    int l = (p.snd-1+width)%width;
    if(grid[p.fst][l]) ret.push_back({p.fst, l});

    return ret;
}

vvi calc_dists(pi a){

    vvi dist(height, vi(width, inf));

    queue<pi> q;
    q.push(a);
    dist[a.fst][a.snd] = 0;

    while(!q.empty()){
        pi now = q.front();
        q.pop();

        vpi neighbors = get_neighbors(now);
        for(pi nxt: neighbors) if(dist[nxt.fst][nxt.snd] == inf){
            dist[nxt.fst][nxt.snd] = dist[now.fst][now.snd] + 1;
            q.push(nxt);
        }
    }

    return dist;
}

class Pac {
public:
    int id;
    pi pos;
    pi pre_pos;
    pi target;

    Pac(): pos({-1,-1}), pre_pos({-1,-1}), target({-1,-1}) {}
    Pac(int pac_id, int x, int y): id(pac_id), pos({y,x}), pre_pos({-1,-1}), target({y,x}) {}

    void move(pi p, vpi &one_pellets, vpi &ten_pellets){
        pos = p;
        if(pos == pre_pos){
            set_random_target(one_pellets, ten_pellets);
        }
        if(pos == target){
            set_target(one_pellets, ten_pellets);
        }
        pre_pos = pos;
    }

    void set_random_target(vpi &one_pellets, vpi &ten_pellets){
        std::random_device rnd;     // 非決定的な乱数生成器を生成
        std::mt19937 mt(rnd());     //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
        if(ten_pellets.size() > 0){
            std::uniform_int_distribution<> rand100(0, ten_pellets.size()-1);        // [0, 99] 範囲の一様乱数
            target = ten_pellets[rand100(mt)];
        }
        else{
            std::uniform_int_distribution<> rand100(0, one_pellets.size()-1);        // [0, 99] 範囲の一様乱数
            target = one_pellets[rand100(mt)];
        }
    }

    void set_target(vpi &one_pellets, vpi &ten_pellets){

        vvi dists = calc_dists(pos);

        function<void(vpi&)> set_target_with_pellets = [&](vpi &pellets){
            int min_dist = inf;
            for(pi p: pellets){
                if(min_dist > dists[p.fst][p.snd]){
                    min_dist = dists[p.fst][p.snd];
                    this->target = p;
                }
            }
            pellets.erase(find(pellets.begin(), pellets.end(), target));
            return;
        };

        //ten_pelletsがあれば、その中で一番近いやつを設定
        if(ten_pellets.size() > 0){
            set_target_with_pellets(ten_pellets);
        }
        //ten_pelletsがなければ、one_pelletsの中から選ぶ
        else{
            set_target_with_pellets(one_pellets);
        }
    }
};

map<int,Pac> pacs;

int main()
{
    cin >> width >> height; cin.ignore();

    grid = vvb(height, vb(width));

    for (int i = 0; i < height; i++) {
        string row;
        getline(cin, row); // one line of the grid: space " " is floor, pound "#" is wall
        rep(j,width) grid[i][j] = (row[j] == ' ');
    }
    
    bool is_first_turn = true;

    // game loop
    while (1) {
        int my_score;
        int opponent_score;
        cin >> my_score >> opponent_score; cin.ignore();
        int visible_pac_count; // all your pacs and enemy pacs in sight
        cin >> visible_pac_count; cin.ignore();

        map<int,pi> moves;
        vi pac_ids;

        for (int i = 0; i < visible_pac_count; i++) {
            int pac_id; // pac number (unique within a team)
            bool mine; // true if this pac is yours
            int x; // position in the grid
            int y; // position in the grid
            string type_id; // unused in wood leagues
            int speed_turns_left; // unused in wood leagues
            int ability_cooldown; // unused in wood leagues
            cin >> pac_id >> mine >> x >> y >> type_id >> speed_turns_left >> ability_cooldown; cin.ignore();
            if(mine){
                if(is_first_turn) pacs[pac_id] = Pac(pac_id, x,y);
                moves[pac_id] = {y,x};
                pac_ids.push_back(pac_id);
            }
        }


        int visible_pellet_count; // all pellets in sight
        cin >> visible_pellet_count; cin.ignore();
        
        vpi ten_pellets, one_pellets;

        for (int i = 0; i < visible_pellet_count; i++) {
            int x;
            int y;
            int value; // amount of points this pellet is worth
            cin >> x >> y >> value; cin.ignore();
            if(value == 10) ten_pellets.push_back({y,x});
            else one_pellets.push_back({y,x});
        }
        
        for(int pac_id: pac_ids){
            pacs[pac_id].move(moves[pac_id], one_pellets, ten_pellets);
        }

        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;
        cout<<"MOVE " << pac_ids[0] << " " << pacs[pac_ids[0]].target.snd <<" "<< pacs[pac_ids[0]].target.fst;

        for(int i=1;i<pac_ids.size();i++){
            cout<<"|MOVE " << pac_ids[i] << " " << pacs[pac_ids[i]].target.snd <<" "<< pacs[pac_ids[i]].target.fst;
        }
        cout<<endl;

        is_first_turn = false;
    }
}