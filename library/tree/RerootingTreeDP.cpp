
//全方位木DP
//各頂点からのDFSで解ける問題がO(N)で解ける

//条件
//木DPを普通にやった後、頂点を「ずらす」操作がある程度高速（O(|childs|）程度）でできればOK
//左右から半壊区間を伸ばして、それの結合とかでできれば嬉しいね

//関数
template <typename T>
struct RerootingTreeDP{
public:
    //集計結果と自分の値から、自分が持つべき値を計算する
    //引数 : 自分の値、集計結果

    ll n;
    
    //edgedp用マップ
    vector<vector<T>> dp;
    
    //頂点DP用マップ
    vector<T> rootDP;

    //edges
    vvll nexts;
    vector<T> values;
    // edge_idx[i][j] := iから、jに伸びている辺は、nexts[j]の何番目か
    vector<unordered_map<ll,ll>> edge_idx;

    T (*marge_child)(T,T);
    T (*aggrigate)(T,T);
    T e;

    RerootingTreeDP(const vvll& _nexts, const vector<T>& _values, T (*_aggrigate)(T,T), T(*_marge_child)(T,T), T _e): nexts(_nexts), values(_values), aggrigate(_aggrigate), marge_child(_marge_child), e(_e){
        n = nexts.size();

        edge_idx.resize(n);
        rep(i, n) rep(j, nexts[i].size()) edge_idx[i][nexts[i][j]] = j;

        rootDP.resize(n);

        dp.resize(n);
        //最初にDP配列を初期化
        rep(i,n) dp[i].resize(nexts[i].size(), e);
        
        //0番目を根として最初のdfsをやる
        rep(i, nexts[0].size()) setNodeDfs(0, i);
        
        //今の根っこからBFSしながら逆辺のdpを求めていく
        // rootDPの設定もやっちゃう
        rerootingBfs();
    }
    RerootingTreeDP(){}

    //もらった辺の先っちょ以降の部分技について値を求める
    //最初に呼ぶだけ
    //普通に木DPすればOK
    //アドホックに作ってね
    //dp[parent][now]の初期化を忘れずに
    void setNodeDfs(ll parent, ll i){
        ll now = nexts[parent][i];
        T childs_value = e;
        rep(ni,  nexts[now].size()){
            if(nexts[now][ni] == parent) continue;
            setNodeDfs(now, ni);
            childs_value = marge_child(childs_value, dp[now][ni]);
        }
        dp[parent][i] = aggrigate(values[now], childs_value);
        return;
    }
    
    // 根っこからBFSで逆辺のDPを求めていく
    // rootDPの設定もやっちゃう
    void rerootingBfs(){
        
        queue<ll> q;
        q.push(0);
        
        vb visit(nexts.size());
        visit[0] = true;
        
        while(q.empty() == false){
            //dp[{now,*}]がすでに求まっている
            //その状態で、dp[{*,now}]を求めていく
            
            ll now = q.front();
            q.pop();
            
            //margeR[i] := i番目より後の子供の値を連結した値
            vector<T> margeR(nexts[now].size(), e);
            mrep(i, (ll)nexts[now].size()-2) margeR[i] = marge_child(margeR[i+1], dp[now][i+1]);
            
            T left = e;
            
            //左右からの累積和を使ってi番目の子供への辺を逆転したdpを求める
            rep(i, nexts[now].size()){
                ll next = nexts[now][i];
                dp[next][edge_idx[next][now]] = aggrigate(values[now], marge_child(left, margeR[i]));
                left = marge_child(left, dp[now][i]);
                //行った先始点verをまだ計算してなければ計算する
                if(visit[next] == false){
                    q.push(next);
                    visit[next] = true;
                }
            }

            rootDP[now] = aggrigate(values[now], left);
        }
        
    }

};
