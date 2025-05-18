
//全方位木DP
//各頂点からのDFSで解ける問題がO(N)で解ける

//条件
//木DPを普通にやった後、頂点を「ずらす」操作がある程度高速（O(|childs|）程度）でできればOK
//左右から半壊区間を伸ばして、それの結合とかでできれば嬉しいね


//関数
class RerootingTreeDP{
public:
    //集計結果と自分の値から、自分が持つべき値を計算する
    //引数 : 自分の値、集計結果
    
    //edgedp用マップ
    map<pll,ll> dp;
    
    //頂点DP用マップ
    vector<ll> rootDP;

    //edges
    vvll nexts;

    RerootingTreeDP(ll n, vvll tnexts): nexts(tnexts), rootDP(n){

        //最初にDP配列を初期化
        rep(i,n){
            for(ll next: nexts[i]){
                dp[{i,next}] = 0;
            }
        }
        
        //0番目を根として最初のdfsをやる
        for(ll next: nexts[0]) setNodeDfs(0,next);
        
        //今の根っこからBFSしながら逆辺のdpを求めていく
        rerootingBfs(0);
        
        //各頂点がrootだった時の値を保存していく
        rep(i,n) setRootDpDfs(i);
    }
    RerootingTreeDP(){}

    //もらった辺の先っちょ以降の部分技について値を求める
    //最初に呼ぶだけ
    //普通に木DPすればOK
    //アドホックに作ってね
    //dp[parent][now]の初期化を忘れずに
    void setNodeDfs(ll parent, ll now){
        //ここから先は長さ足りてる
        //上位3つを取る
        vll childeScores;
        for(ll next : nexts[now]){
            if(next == parent) continue;
            setNodeDfs(now, next);
            childeScores.push_back(dp[{now,next}]);
        }
        sorts(childeScores);
        reverse(all(childeScores));
        dp[{parent,now}] = 0;
        if(nexts[now].size() >= 4) rep(i,3) dp[{parent,now}] += childeScores[i];
        dp[{parent,now}]++;
        return;
    }
    
    //根っこからBFSで逆辺のDPを求めていく
    void rerootingBfs(ll now){
        
        queue<ll> q;
        q.push(now);
        
        vb visit(nexts.size());
        visit[now] = true;
        
        while(q.empty() == false){
            //dp[{now,*}]がすでに求まっている
            //その状態で、dp[{*,now}]を求めていく
            //O(|nexts[now]|)とかO(|nexts[now]|log(|nexts[now|))とかでできればOK

            //どっちも半開区間でやる
            //margeL[i] := i番目の子供を含まない
            vector<ll> margeL(nexts[now].size(),0);
            //margeR[i] := i番目の子供を含まない
            vector<ll> margeR(nexts[now].size(),0);
            
            
            //左右からの累積和を使ってi番目の子供への辺を逆転したdpを求める
            rep(i,nexts[now].size()){
                ll next = nexts[now][i];
                dp[{next,now}] = /* margeLとmargeRの結合 */
                //行った先始点verをまだ計算してなければ計算する
                if(visit[next] == false){
                    q.push(next);
                    visit[next] = true;
                }
            }
        }
        
    }
    
    //各頂点がrootだった時の値を保存していくDFS
    void setRootDpDfs(ll now){
        //dp[{now,*}]を集計する
        //O(|nexts[now|)でOK

        rootDP[now] = /* dp[{now,next}]の結合 */
    }

};
