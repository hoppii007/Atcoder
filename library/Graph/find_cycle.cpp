
/**
 * @brief 有向グラフから root を起点としてサイクルを 1 つ取得
 * @param nexts 隣接リスト（nexts[i] は頂点 i から伸びる辺の行き先一覧）
 * @param root 探索の起点となる頂点
 * @return サイクルを構成する頂点列。cycle[0] -> cycle[1] -> ... -> cycle.back() -> cycle[0] の順に巡る。サイクルが無ければ空配列
 * @note 計算量: O(V + E)
 * @note root から到達可能なサイクルのうち 1 つだけを返す。グラフが非連結だったり root より手前にサイクルがあると検出できない
 * @note DFS の行きがけ順（fst）と帰りがけ順（snd）を記録し、訪問中（snd 未確定）の頂点へ戻る後退辺をサイクルとして検出する
 */
vll find_cycle(vvll &nexts, ll root){
    vpll ret(nexts.size(), {-1,-1});
    ll order = 0;
    vll cycle;
    std::function<ll(ll)> dfs = [&](ll now){
        ret[now].fst = order++;
        for(ll next: nexts[now]){
            if(ret[next].fst == -1){
                ll result = dfs(next);
                //ループを見つけて帰ってきた時
                if(result != -1){
                    //ループ元に辿り着いてなければ自分もループの仲間
                    if(result != inf) cycle.push_back(now);
                    if(result == now) return inf;
                    else return result;
                }
            }
            else{
                //きたことはあって終わってないところに来たらループ！
                if(ret[next].snd == -1){
                    cycle.push_back(now);
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