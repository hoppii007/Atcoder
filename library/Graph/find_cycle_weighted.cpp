#pragma once
/**
 * @brief 重み付き有向グラフのサイクルを 1 つ取得（辺列で返す）
 *
 *   Edge{u, v, c}           … u → v のコスト c の辺
 *   find_cycle(nexts, root) … root から到達できるサイクルの辺列（無ければ空）  O(V+E)
 *
 * @param nexts 隣接リスト nexts[u] = {cost, v}
 * @note 返り値は cycle[k].v == cycle[k+1].u、cycle.back().v == cycle[0].u
 * @note root から到達できないサイクルは見つからない
 *
 * @code
 * vvpll nexts(n);
 * vector<Edge> cycle = find_cycle(nexts, 0);
 * for(Edge e: cycle) cout << e.u << " " << e.v << " " << e.c << endl;
 * @endcode
 */
struct Edge{
    ll u, v, c;
    Edge(){}
    Edge(ll tu, ll tv, ll tc): u(tu), v(tv), c(tc){}
};

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
