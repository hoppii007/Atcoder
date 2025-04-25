#include<bits/stdc++.h>
//入力系
#define cinll(...) ll __VA_ARGS__; input(__VA_ARGS__);
#define cinint(...) int __VA_ARGS__; input(__VA_ARGS__);
#define cinstr(...) string __VA_ARGS__; input(__VA_ARGS__);
#define cinchar(...) char __VA_ARGS__; input(__VA_ARGS__);
#define cinvll(a,n) vll a(n); rep(i,n) cin>>a[i];
#define cinvvll(a,n,m) vvll a(n,vll(m)); rep(i,n) rep(j,m) cin>>a[i][j];
#define cinvs(a,n) vs a(n); rep(i,n) cin>>a[i];
//繰り返し系
#define rep1(n) for(int i=0;i<n;i++)
#define rep2(i,n) for(int i=0;i<n;i++)
#define rep3(i,a,n) for(int i=a;i<n;i++)
#define rep4(i,a,n,b) for(int i=a;i<n;i+=b)
#define overload4(a,b,c,d,e,...) e
#define rep(...) overload4(__VA_ARGS__,rep4,rep3,rep2,rep1)(__VA_ARGS__)
#define mrep1(n) for(int i=n;i>=0;i--)
#define mrep2(i,n) for(int i=n;i>=0;i--)
#define mrep3(i,n,a) for(int i=n;i>=a;i--)
#define mrep4(i,n,a,b) for(int i=n;i>=a;i-=b)
#define mrep(...) overload4(__VA_ARGS__,mrep4,mrep3,mrep2,mrep1)(__VA_ARGS__)
//iterator系
#define all(a) a.begin(),a.end()
using namespace std;
//型系
using ll = long long;
using vll = vector<long long>;
using vvll = vector<vector<long long>>;
using vi = vector<int>;
using vvi = vector<vector<int>>;
using vb = vector<bool>;
using vvb = vector<vector<bool>>;
using vc = vector<char>;
using vvc = vector<vector<char>>;
using vs = vector<string>;
using pll = pair<long long,long long>;
using vpll = vector<pair<long long,long long>>;

const ll mod = 998244353LL;
const ll inf = 100100100100100100LL;

//便利系
//vectorをjoinするやつ
template<typename T>
string v_join(vector<T> &a,string c){
	string ret = "";
	rep(i,a.size()-1) ret += (to_string(a[i]) + c);
	return ret + to_string(a[a.size()-1]);
}
//空白区切り表示
template<class T, class... Ts>
void out(const T& a,const Ts&... b){
	cout<<a;
	(cout<< ... << (cout << ' ',b));
	cout<<endl;
}
//入力
template<class... T>
void input(T&... a){
	(cin >> ... >> a);
}


struct Value{
public:
	ll kindValue;
	ll allValue;
	Value(ll k,ll a){
		kindValue = k;
		allValue = a;
	}
	Value(){
	    kindValue = 0;
	    allValue = 0;
	}
	bool operator<(const Value &v) const {
	    return allValue < v.allValue;
	}

};

struct Node{
public:
	ll i;
	ll c;
	Node(ll ti,ll tc){
		i = ti;
		c = tc;
	}
	Node(){
	    i = -1;
	    c = -1;
	}
    bool operator<(const Node& node)const{
        return i < node.i;
    }
};


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




Value add(Value a,Value b){
    return Value(a.kindValue+b.kindValue, a.allValue+b.allValue);
}

Value calc(ll myScore,Value margeScore){
    return Value(margeScore.kindValue+myScore, margeScore.allValue+margeScore.kindValue);
}

int main(void){

	cinll(n);

	cinvvll(edges,n-1,2);

	cinvll(c,n);
	
	map<Node,vector<Node>> nexts;
	vector<Node> nodes;
	rep(i,n){
		nodes.push_back(Node(i,c[i]));
	}
	rep(i,n-1){
		edges[i][0]--;
		edges[i][1]--;
		nexts[nodes[edges[i][0]]].push_back(nodes[edges[i][1]]);
		nexts[nodes[edges[i][1]]].push_back(nodes[edges[i][0]]);
	}
	
	RerootingTreeDP<Node,Value> rtd(nodes,add,Value(0,0),nexts,calc);
	

	return 0;
}
