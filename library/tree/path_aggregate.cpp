#pragma once
#include "library/Math/monoid.cpp"
/**
 * @brief 木のパス上の辺の値の集約（LCA つき）
 *
 *   path_aggregate<M> pa(root, nexts) … 構築  O(N log N)
 *   pa.path_val(u, v)                … u-v パス上の辺の値の集約  O(log N)
 *   pa.lca(u, v)                     … u と v の LCA  O(log N)
 *   pa.dist(u, v)                    … u-v 間の辺数  O(log N)
 *   pa.back(u, k)                    … u から k 回親に遡った頂点（根を越えたら -1）  O(log N)
 *   pa.climb_val(u, k)               … u から k 回遡る間の辺の値の集約  O(log N)
 *
 * @tparam M     モノイド（library/Math/monoid.cpp）
 * @param  nexts nexts[u] = {{辺の値, 隣接頂点}, ...}（無向木として両方向に登録）
 * @note path_val は可換モノイド前提（パスの向きの順序を保たない）
 *
 * @code
 * ll op_sum(ll a, ll b){ return a + b; }
 * ll e_sum(){ return 0; }
 * using sum_monoid = monoid<ll, op_sum, e_sum>;
 *
 * // 辺 (u, v, w) を双方向に登録
 * vector<vector<pll>> g(n);   // {辺の値, 隣接頂点}
 * for(auto& [u, v, w] : edges){
 *     g[u].push_back({w, v});
 *     g[v].push_back({w, u});
 * }
 * path_aggregate<sum_monoid> tree(0, g);
 * ll s = tree.path_val(3, 7);  // 3-7 パス上の辺重みの和
 * @endcode
 */
template<typename M>
struct path_aggregate {
    using T = typename M::value_type;
    static constexpr ll LOG = 60;

    ll root;
    vll parents;
    vector<T> parent_val;
    vll dist_to_root;
    vvll back_dub;
    vector<vector<T>> val_dub;

    path_aggregate(ll r, vector<vector<pair<T,ll>>>& nexts): root(r){
        ll n = nexts.size();
        parents.assign(n, -1);
        parent_val.assign(n, M::e());
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
        dfs(root, -1, M::e(), 0);

        // ダブリングテーブル構築
        back_dub.assign(n, vll(LOG+1, -1));
        val_dub.assign(n, vector<T>(LOG+1, M::e()));
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

    ll back(ll u, ll steps){
        rep(j, LOG+1) if(steps & (1LL << j)){
            u = back_dub[u][j];
            if(u == -1) return -1;
        }
        return u;
    }

    T climb_val(ll u, ll steps){
        T ret = M::e();
        rep(j, LOG+1) if(steps & (1LL << j)){
            ret = M::merge(ret, val_dub[u][j]);
            u = back_dub[u][j];
            if(u == -1) break;
        }
        return ret;
    }

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

    T path_val(ll u, ll v){
        ll l = lca(u, v);
        return M::merge(
            climb_val(u, dist_to_root[u] - dist_to_root[l]),
            climb_val(v, dist_to_root[v] - dist_to_root[l])
        );
    }

    ll dist(ll u, ll v){
        ll l = lca(u, v);
        return dist_to_root[u] + dist_to_root[v] - 2*dist_to_root[l];
    }
};
