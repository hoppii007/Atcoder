#pragma once
/**
 * @brief 有向グラフの閉路判定
 *
 *   have_cycle(nexts) … 閉路があれば true  O(V+E)
 *
 * @note 無向グラフで使うときは関数内の if(nxt == prev) continue; を有効にする
 *
 * @code
 * vvll nexts(n);
 * if(have_cycle(nexts)) cout << "Yes" << endl;
 * @endcode
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
