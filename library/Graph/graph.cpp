#pragma once
/**
 * @brief 有向グラフ（隣接リスト・辺リスト・次数をまとめて保持）
 *
 *   Graph<T> g(n)            … 頂点数 n の空グラフ（T はコストの型、既定 ll）
 *   g.add_edge(u, v)         … コスト 1 の辺 u → v を追加
 *   g.add_edge(c, u, v)      … コスト c の辺 u → v を追加
 *   g.size_n() / size_e()    … 頂点数 / 辺数
 *   g.total_edge_cost()      … 全辺のコスト合計
 *   g.nexts() / nexts(i)     … 隣接リスト nexts[u] = {cost, v} / 頂点 i の分
 *   g.edges()                … 辺リスト（Edge{c, u, v}、c → u → v の順で比較可能）
 *   g.deg_in() / deg_in(i)   … 入次数配列 / 頂点 i の入次数
 *   g.deg_out() / deg_out(i) … 出次数配列 / 頂点 i の出次数
 *
 * @note 無向グラフは add_edge を両向きに呼ぶ
 *
 * @code
 * Graph<ll> g(n);
 * g.add_edge(5, 0, 1);
 * g.add_edge(1, 0);
 * for(auto [c, v]: g.nexts(0)) cout << c << " " << v << endl;
 * @endcode
 */
template<typename T = long long>
struct Graph {
    struct Edge{
        T c;
        long long u,v;
        bool operator<(const Edge &other) const {
            return tie(c,u,v) < tie(other.c, other.u, other.v);
        }
    };
    
    vector<vector<pair<T,long long>>> _nexts;
    vector<Edge> _edges;
    T _total_edge_cost;
    vector<long long> _deg_in;
    vector<long long> _deg_out;
    
    Graph(long long n): _nexts(n), _total_edge_cost(0), _deg_in(n,0), _deg_out(n,0) {}
    
    void add_edge(long long u, long long v){
        add_edge(T(1), u, v);
    }
    void add_edge(T c, long long u, long long v){
        _nexts[u].push_back({c, v});
        _edges.push_back(Edge{c, u, v});
        _total_edge_cost += c;
        _deg_out[u]++;
        _deg_in[v]++;
    }
    long long size_n() const {
        return _nexts.size();
    }
    long long size_e() const {
        return _edges.size();
    }
    T total_edge_cost() const {
        return _total_edge_cost;
    }
    const vector<Edge>& edges() const {
        return _edges;
    }
    const vector<vector<pair<T,long long>>>& nexts() const {
        return _nexts;
    }
    const vector<pair<T,long long>>& nexts(long long i) const {
        return _nexts[i];
    }
    const vector<long long>& deg_in() const {
        return _deg_in;
    }
    long long deg_in(long long i) const {
        return _deg_in[i];
    }
    const vector<long long>& deg_out() const {
        return _deg_out;
    }
    long long deg_out(long long i) const {
        return _deg_out[i];
    }
};
