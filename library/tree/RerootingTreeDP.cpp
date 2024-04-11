//全方位木DP
//各頂点からのDFSで解ける問題がO(N)で解ける

//条件
//Nは頂点の型、Tは答えを求めるときの演算の型
//T,idT,opTがモノイドになっている
//各頂点から、子供の結果をopTで結合するとその頂点の値もわかるのが必要
//nexts : nexts[v]にはvからいけるノードが全部入っている
//calcOwnValue(自分の値,集計結果) : 集計結果と自分の値から、自分が持つべき値を計算する
//NもTもbool operator<(const &T t) const { return 自分 < t }を作らない解けない

//コンストラクタ　関数ポインタの部分は普通に関数名書くだけでOK
//RerootingTreeDP<N,T>(vector<N> 木の頂点要素全部、T演算、T単位元、map<N,vector<N>> tnexts, T calcOwnValue(N,T))

//関数
//get(N n) : 頂点nが根だった時の値を返してくれる
template<typename N,typename T>
class RerootingTreeDP{
public:
	//要素のモノイド
    T idT;
    T (*opT)(T,T);

	//集計結果と自分の値から、自分が持つべき値を計算する
	//引数 : 自分の値、集計結果
	T (*calcOwnValue)(N,T);
	
	//edgedp用マップ
	map<N,map<N,T>> dp;
	
	//頂点DP用マップ
	map<N,T> rootDP;

	//edges
	map<N,vector<N>> nexts;

	RerootingTreeDP(vector<N> nodes, T (*opt)(T,T), T idt, map<N,vector<N>> tnexts, T (*tcalcOwnValue)(N,T)){
		this->idT = idt;
		this->opT = opt;
		this->nexts = tnexts;
		this->calcOwnValue = tcalcOwnValue;

		//最初にDP配列を初期化
		for(pair<N,vector<N>> p : nexts){
			for(N next : p.second){
				dp[p.first][next] = idT;
			}
		}
		
		//0番目を根として最初のdfsをやる
		setNodeDfs(nodes[0]);
		
		//今の根っこからBFSしながら逆辺のdpを求めていく
		rerootingBfs(nodes[0]);
		
		//各頂点がrootだった時の値を保存していく
		setRootDpDfs(nodes[0]);
	}
	RerootingTreeDP(){}

	//貰ったやつを根としてDFS
	void setNodeDfs(N now, T parent=NULL){
		T childMarge = idT;
		for(N child : nexts[now]){
			if(child == parent) continue;
			setNodeDfs(child, now);
			childMarge = opT(childMarge,dp[now][child]);
		}
		if(parent != NULL) dp[parent][now] = calcOwnValue(now,childMarge);
	}
	
	//根っこからBFSで逆辺のDPを求めていく
	void rerootingBfs(N now){
	    
	    queue<N> q;
	    q.push(now);
	    
	    map<N,bool> visit;
	    
	    while(q.empty() == false){
	        
	        now = q.front();
	        q.pop();
	        
	        if(visit[now] == true) continue;
	        visit[now] = true;
	        
	        //[)みたいな半開区間でやる
    	    //margeL[i] := i番目の子供を含まない
    	    vector<T> margeL(nexts[now].size(),idT);
    	    //margeR[i] := i番目の子供を含む
    	    vector<T> margeR(nexts[now].size(),idT);
    	    
    	    rep(i,nexts[now].size()){
    	        if(i == 0){
    	            margeR[nexts[now].size()-i-1] = dp[now][nexts[now][nexts[now].size()-i-1]];
    	        }
    	        else{
    	            margeL[i] = opT(margeL[i-1],dp[now][nexts[now][i-1]]);
    	            margeR[nexts[now].size()-i-1] = opT(margeR[nexts[now].size()-i],dp[now][nexts[now][nexts[now].size()-i-1]]);
    	        }
    	        if(visit[nexts[now][i]] == true) continue;
    	        q.push(nexts[now][i]);
    	    }
    	    
    	    //左右からの累積和を使ってi番目の子供への辺を逆転したdpを求める
    	    rep(i,nexts[now].size()){
    	        if(i == nexts[now].size()-1){
    	            dp[nexts[now][i]][now] = calcOwnValue(now,margeL[i]);
    	            break;
    	        }
    	        dp[nexts[now][i]][now] = calcOwnValue(now,opT(margeL[i],margeR[i+1]));
    	    }
	    }
	    
	}
	
	//各頂点がrootだった時の値を保存していくDFS
	void setRootDpDfs(N now, N parent=NULL){
	    T margeChild = idT;
	    for(N child : nexts[now]){
	        if(parent != child) setRootDpDfs(child,now);
	        margeChild = opT(margeChild,dp[now][child]);
	    }
	    rootDP[now] = calcOwnValue(now, margeChild);
	}
	
	//頂点DPをセットしていくDFS
	T get(N root){
	    return rootDP[root];
	}

};

