/**
 * @brief 有向グラフの閉路判定
 * @param nexts 隣接リスト（nexts[i] は頂点 i から伸びる辺の行き先一覧）
 * @return 閉路が存在すれば true、しなければ false
 * @note 計算量: O(V + E)
 * @note DFS で訪問中（visit=true かつ finished=false）の頂点に戻る辺があれば閉路
 */
bool have_cycle(vvll &nexts){
    vb visit(nexts.size(), false);
    vb finished(nexts.size(), false);

    function<bool(ll,ll)> dfs = [&](ll now, ll prev){
        visit[now] = true;
        for(ll nxt: nexts[now]) {
            
            /////////////////////////////////////////
            // 無向グラフの場合、戻らないためにこれを追加 //
            ////////////////////////////////////////
            //if(nxt == prev) continue;

            if(visit[nxt] == false) {
                if(dfs(nxt, now)) return true;
            }
            else if(finished[nxt] == false) return true;
        }
        finished[now] = true;
        return false;
    };

    rep(i, nexts.size()) if(visit[i] == false) if(dfs(i,-1)) return true;
    return false;
}
