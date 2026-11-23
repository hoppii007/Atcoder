#pragma once
/**
 * @brief 二部グラフ判定と塗り分け（無向グラフ）
 *
 *   bipartite_graph(nexts) … {二部グラフか, 各頂点の色(false/true)} を返す  O(V+E)
 *
 * @note 連結である必要はない。未着色の頂点を番号順に起点にし、起点を false として塗る
 * @note 非連結のとき、各連結成分の塗り方は「そのまま／全反転」の 2 通りあるが、返すのはその 1 つに固定したもの。
 *       別の成分どうしの色の比較には意味がない。成分ごとに反転を選びたい（黒白の個数を成分ごとに見たい等）
 *       ときは、先に split_connected_graph で分割してから成分ごとに呼ぶ
 * @note 二部グラフでないとき、色の配列は途中まで塗った状態（意味を持たない）
 * @note 再帰の深さは最大 V
 *
 * @code
 * vvll nexts(n);
 * auto [ok, color] = bipartite_graph(nexts);
 * if(!ok) cout << 0 << endl;
 *
 * // 成分ごとに見る場合
 * for(vvll& g: split_connected_graph(nexts)){
 *     auto [ok, color] = bipartite_graph(g);
 *     ll black = count(all(color), true);
 * }
 * @endcode
 */
pair<bool,vb> bipartite_graph(vvll &nexts){
    ll n = nexts.size();
    vb color(n, false);
    vb visit(n, false);

    // now を c で塗り、隣を逆の色で塗っていく。同色の辺があれば false
    function<bool(ll,bool)> dfs = [&](ll now, bool c){
        visit[now] = true;
        color[now] = c;
        for(ll nxt: nexts[now]){
            if(visit[nxt] == false){
                if(dfs(nxt, !c) == false) return false;  // 子の失敗を必ず伝播させる
            }
            else if(color[nxt] == c) return false;
        }
        return true;
    };

    rep(i, n) if(visit[i] == false) if(dfs(i, false) == false) return {false, color};
    return {true, color};
}
