#pragma once
/**
 * @brief 全方位木DP（全頂点を根としたときの木DP）
 *
 *   rerooting_tree_dp<T> rd(nexts, values, aggrigate, marge_child, e) … 構築  O(N log N)
 *   rd.root_dp[v]     … v を根としたときの木全体の値
 *   rd.dp[v][i]      … v から見て nexts[v][i] 側の部分木の値
 *   rd.dpmap[{u, v}] … u から見て v 側の部分木の値
 *
 * @param aggrigate   T aggrigate(T 頂点の値, T 子の集約)：部分木の値を作る
 * @param marge_child T marge_child(T acc, T x)：子の値を集約（結合則が必要。可換でなくてよい）
 * @param e           marge_child の単位元
 * @note nexts は無向木の隣接リスト（0-indexed、連結であること）
 *
 * @code
 * // 各頂点から最も遠い頂点までの距離
 * ll agg(ll own, ll c){ return c + 1; }
 * ll mrg(ll a, ll b){ return max(a, b); }
 *
 * rerooting_tree_dp<ll> rd(nexts, vll(n, 0), agg, mrg, 0);
 * ll ecc = rd.root_dp[v] - 1;
 * @endcode
 */
template <typename T>
struct rerooting_tree_dp{
public:
    ll n;

    vector<vector<T>> dp;

    map<pll, T> dpmap;

    vector<T> root_dp;

    vvll nexts;

    vector<T> values;

    vector<unordered_map<ll,ll>> edge_idx;

    T (*marge_child)(T,T);
    T (*aggrigate)(T,T);

    T e;

    rerooting_tree_dp(const vvll& _nexts, const vector<T>& _values, T (*_aggrigate)(T,T), T(*_marge_child)(T,T), T _e): nexts(_nexts), values(_values), aggrigate(_aggrigate), marge_child(_marge_child), e(_e){
        n = nexts.size();

        // 逆向きの辺インデックスを引けるようにしておく
        edge_idx.resize(n);
        rep(i, n) rep(j, nexts[i].size()) edge_idx[i][nexts[i][j]] = j;

        root_dp.resize(n);

        // dp を単位元で初期化
        dp.resize(n);
        rep(i,n) dp[i].resize(nexts[i].size(), e);

        // 頂点0を根とした通常の木DPで、根から葉に向かう辺の dp を埋める
        rep(i, nexts[0].size()) set_node_dfs(0, i);

        // BFSで逆向きの辺の dp と root_dp を求める
        rerooting_bfs();
    }

    rerooting_tree_dp(){}

    void set_node_dfs(ll parent, ll i){
        ll now = nexts[parent][i];
        T childs_value = e;
        rep(ni,  nexts[now].size()){
            if(nexts[now][ni] == parent) continue;
            set_node_dfs(now, ni);
            childs_value = marge_child(childs_value, dp[now][ni]);
        }
        dp[parent][i] = aggrigate(values[now], childs_value);
        dpmap[pll{parent,now}] = dp[parent][i];
        return;
    }

    void rerooting_bfs(){

        queue<ll> q;
        q.push(0);

        vb visit(nexts.size());
        visit[0] = true;

        while(q.empty() == false){
            // dp[now][*] は既に求まっている。dp[*][now] の側を求めていく。

            ll now = q.front();
            q.pop();

            // marge_r[i] := i 番目より後ろの子供の dp を marge_child で連結した値
            vector<T> marge_r(nexts[now].size(), e);
            mrep(i, (ll)nexts[now].size()-2) marge_r[i] = marge_child(marge_r[i+1], dp[now][i+1]);

            // left := i 番目より前の子供の dp を marge_child で連結した値（ループしながら更新）
            T left = e;

            // 左右からの累積を使い、i 番目の隣接頂点への辺を逆向きに見たときの dp を求める
            rep(i, nexts[now].size()){
                ll next = nexts[now][i];
                dp[next][edge_idx[next][now]] = aggrigate(values[now], marge_child(left, marge_r[i]));
                dpmap[pll{now,next}] = dp[next][edge_idx[next][now]];
                left = marge_child(left, dp[now][i]);

                // まだ訪問していない隣接頂点はキューに積む
                if(visit[next] == false){
                    q.push(next);
                    visit[next] = true;
                }
            }

            // now を根としたときの全体の値（left は最終的に全子供をマージした値）
            root_dp[now] = aggrigate(values[now], left);
        }

    }

};
