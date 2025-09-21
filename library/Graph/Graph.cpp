
template<typename T = long long> // costの型
struct Graph {
    ///////////////辺の構造体↓/////////////
    struct Edge{
        T c;
        long long u,v; // u→vの辺
        // 辺同士の比較は、c → u → vの順番に比較
        bool operator<(const Edge &other) const {
            return tie(c,u,v) < tie(other.c, other.u, other.v);
        }
    };
    
    /////////////メンバ変数↓///////////////
    vector<vector<pair<T,long long>>> _nexts; // nexts[i] := i番目からの隣接リスト{コスト、行き先}
    vector<Edge> _edges; // 辺リスト
    T _total_edge_cost; // 全ての辺のコストの合計
    vector<long long> _deg_in; // 全ての頂点の入次数を保持
    vector<long long> _deg_out; // 全ての頂点の出次数を保持
    
    //////////コンストラクタ↓////////////
    Graph(long long n): _nexts(n), _total_edge_cost(0), _deg_in(n,0), _deg_out(n,0) {}
    
    //////////メンバ関数↓////////////////
    //コスト1の辺u → vを追加する関数
    void add_edge(long long u, long long v){
        add_edge(T(1), u, v);
    }
    //コストcの辺u → vを追加する関数
    void add_edge(T c, long long u, long long v){
        _nexts[u].push_back({c, v});
        _edges.push_back(Edge{c, u, v});
        _total_edge_cost += c;
        _deg_out[u]++;
        _deg_in[v]++;
    }
    //頂点数を返す関数
    long long size_n() const {
        return _nexts.size();
    }
    //辺の本数を返す関数
    long long size_e() const {
        return _edges.size();
    }
    // 全ての辺のコストの合計を返す
    T total_edge_cost() const {
        return _total_edge_cost;
    }
    // 辺リストを返す関数
    const vector<Edge>& edges() const {
        return _edges;
    }
    // 隣接リストを返す関数
    const vector<vector<pair<T,long long>>>& nexts() const {
        return _nexts;
    }
    // iから行ける場所の{コスト、行き先}の集合を返す関数
    const vector<pair<T,long long>>& nexts(long long i) const {
        return _nexts[i];
    }
    // 入次数配列を返す関数
    const vector<long long>& deg_in() const {
        return _deg_in;
    }
    // 頂点iの入次数を返す関数
    long long deg_in(long long i) const {
        return _deg_in[i];
    }
    // 出次数配列を返す関数
    const vector<long long>& deg_out() const {
        return _deg_out;
    }
    // 頂点iの出次数を返す関数
    long long deg_out(long long i) const {
        return _deg_out[i];
    }
};
