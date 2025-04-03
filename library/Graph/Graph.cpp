// 0 ~ N-1の番号がついた頂点
template<typename T>
struct Graph {
public:
    ll n;
    map<T,vector<T>> nexts;
    Graph(){}
    Graph(ll n): n(n) {}
    addEdge(T a, T b){
        map[a].push_back(b);
    }
};