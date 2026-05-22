/**
 * @brief パス上のモノイド値集約（LCA は内部で利用）
 *
 * 各辺にモノイド M の値が乗った木で、
 *   - u-v パス上の辺の値の集約を O(log V)
 *   - LCA を O(log V)
 *   - u-v 間の辺数を O(log V)
 * で求める。前処理 O(V log V)。
 *
 * @tparam M モノイド型（library/Math/monoid.cpp の Monoid を想定）
 *
 * @note path_val は可換モノイド前提（u→lca と v→lca を別々に登って合成するため、
 *       u→lca→v の方向順序を保たない）。
 *       非可換モノイドで方向を区別したい場合は別途反転テーブルが必要。
 *
 * @code
 * ll op_sum(ll a, ll b){ return a + b; }
 * using Sum = Monoid<ll, op_sum, 0>;
 *
 * // 辺 (u, v, w) を双方向に登録
 * vector<vector<pll>> g(n);   // {辺の値, 隣接頂点}
 * for(auto& [u, v, w] : edges){
 *     g[u].push_back({w, v});
 *     g[v].push_back({w, u});
 * }
 * PathAggregate<Sum> tree(0, g);
 * ll s = tree.path_val(3, 7);  // 3-7 パス上の辺重みの和
 * @endcode
 */
template<typename M>
struct PathAggregate {
    using T = typename M::value_type;
    static constexpr ll LOG = 60;

    /** @brief 根の頂点番号 */
    ll root;
    /** @brief parents[i] := i の親（root のときは -1） */
    vll parents;
    /** @brief parent_val[i] := i から親への辺の値（root のときは単位元） */
    vector<T> parent_val;
    /** @brief dist_to_root[i] := root から i までの辺数（未訪問は -1） */
    vll dist_to_root;
    /** @brief back_dub[i][j] := i から 2^j 回親方向に遡った頂点（範囲外は -1） */
    vvll back_dub;
    /** @brief val_dub[i][j] := i から 2^j 回遡る間の辺の値の M::merge による畳み込み */
    vector<vector<T>> val_dub;

    /**
     * @brief コンストラクタ
     * @param r 根の頂点番号
     * @param nexts nexts[i] := i に隣接する {辺の値, 隣接頂点} のリスト（無向木として渡す）
     */
    PathAggregate(ll r, vector<vector<pair<T,ll>>>& nexts): root(r){
        ll n = nexts.size();
        parents.assign(n, -1);
        parent_val.assign(n, M::e);
        dist_to_root.assign(n, -1);

        // 1パス DFS で parents / parent_val / dist_to_root を埋める
        function<void(ll, ll, T, ll)> dfs = [&](ll now, ll par, T val_from_par, ll d){
            parents[now] = par;
            parent_val[now] = val_from_par;
            dist_to_root[now] = d;
            for(auto& [val, next] : nexts[now]) if(dist_to_root[next] == -1){
                dfs(next, now, val, d+1);
            }
        };
        dfs(root, -1, M::e, 0);

        // ダブリングテーブル構築
        back_dub.assign(n, vll(LOG+1, -1));
        val_dub.assign(n, vector<T>(LOG+1, M::e));
        rep(i, n){
            back_dub[i][0] = parents[i];
            val_dub[i][0] = parent_val[i];
        }
        rep(j, 1, LOG+1) rep(i, n){
            if(back_dub[i][j-1] == -1){
                back_dub[i][j] = -1;
                val_dub[i][j] = val_dub[i][j-1];
            } else {
                back_dub[i][j] = back_dub[back_dub[i][j-1]][j-1];
                val_dub[i][j] = M::merge(val_dub[i][j-1], val_dub[back_dub[i][j-1]][j-1]);
            }
        }
    }

    /**
     * @brief u から steps 回親方向に遡った頂点を返す
     * @return 遡った先の頂点。root を越えた場合は -1
     */
    ll back(ll u, ll steps){
        rep(j, LOG+1) if(steps & (1LL << j)){
            u = back_dub[u][j];
            if(u == -1) return -1;
        }
        return u;
    }

    /**
     * @brief u から steps 回親方向に遡る間の辺の値の畳み込みを返す
     * @note steps == 0 のときは単位元
     */
    T climb_val(ll u, ll steps){
        T ret = M::e;
        rep(j, LOG+1) if(steps & (1LL << j)){
            ret = M::merge(ret, val_dub[u][j]);
            u = back_dub[u][j];
            if(u == -1) break;
        }
        return ret;
    }

    /** @brief u と v の最近共通祖先（LCA） */
    ll lca(ll u, ll v){
        // 深さを揃える
        if(dist_to_root[u] < dist_to_root[v]) v = back(v, dist_to_root[v] - dist_to_root[u]);
        else if(dist_to_root[u] > dist_to_root[v]) u = back(u, dist_to_root[u] - dist_to_root[v]);
        if(u == v) return u;

        // 2^j 進んでも違うなら進む、を j 大きい順に
        mrep(j, LOG) if(back_dub[u][j] != back_dub[v][j]){
            u = back_dub[u][j];
            v = back_dub[v][j];
        }
        return parents[u];
    }

    /**
     * @brief u-v パス上の辺の値の畳み込み（可換モノイド前提）
     */
    T path_val(ll u, ll v){
        ll l = lca(u, v);
        return M::merge(
            climb_val(u, dist_to_root[u] - dist_to_root[l]),
            climb_val(v, dist_to_root[v] - dist_to_root[l])
        );
    }

    /** @brief u-v 間の辺数 */
    ll dist(ll u, ll v){
        ll l = lca(u, v);
        return dist_to_root[u] + dist_to_root[v] - 2*dist_to_root[l];
    }
};
