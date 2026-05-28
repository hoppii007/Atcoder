
/// @brief 重み付き有向辺。u から v へ向かうコスト c の辺を表す
struct Edge{
    ll u, v, c;
    Edge(){}
    Edge(ll tu, ll tv, ll tc): u(tu), v(tv), c(tc){}
};

/**
 * @brief 重み付き有向グラフから root を起点としてサイクルを 1 つ取得
 * @param nexts 重み付き隣接リスト（nexts[i] は頂点 i から出る辺の {コスト, 行き先} 一覧）
 * @param root 探索の起点となる頂点
 * @return サイクルを構成する辺列。cycle[k].v == cycle[k+1].u でつながり、cycle[0].u -> ... -> cycle.back().v(== cycle[0].u) の順に巡る。サイクルが無ければ空配列
 * @note 計算量: O(V + E)
 * @note root から到達可能なサイクルのうち 1 つだけを返す。グラフが非連結だったり root より手前にサイクルがあると検出できない
 * @note DFS の行きがけ順（fst）と帰りがけ順（snd）を記録し、訪問中（snd 未確定）の頂点へ戻る後退辺をサイクルとして検出する
 * @note 辺は最後に検出した側から push されるため、最後に reverse して巡回順に並べ直している
 */
vector<Edge> find_cycle(vvpll &nexts, ll root){
    vpll ret(nexts.size(), {-1,-1});
    ll order = 0;
    vector<Edge> cycle;
    std::function<ll(ll)> dfs = [&](ll now){
        ret[now].fst = order++;
        for(auto& [cost, next]: nexts[now]){
            if(ret[next].fst == -1){
                ll result = dfs(next);
                //ループを見つけて帰ってきた時
                if(result != -1){
                    //ループ元に辿り着いてなければ自分もループの仲間
                    if(result != inf) cycle.push_back(Edge(now,next,cost));
                    if(result == now) return inf;
                    else return result;
                }
            }
            else{
                //きたことはあって終わってないところに来たらループ！
                if(ret[next].snd == -1){
                    cycle.push_back(Edge(now,next,cost));
                    return next;
                }
            }
        }
        ret[now].snd = order;
        return -1LL;
    };
    dfs(root);
    //最後についたところから保存されるので、ループ順にするには逆にしないといけない
    reverse(all(cycle));
    return cycle;
}
