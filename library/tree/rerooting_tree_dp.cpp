/**
 * @brief 全方位木DP（Rerooting Tree DP）
 *
 * 木の各頂点を根としたときの木DP結果を、全頂点について合計 O(N) で求める。
 * 各頂点から愚直に DFS すると O(N^2) になるが、一度だけ DFS した結果を
 * 左右からの累積で再利用することで O(N) で全頂点分の答えを得られる。
 *
 * @tparam T DPで扱う値の型
 *
 * コンストラクタに渡す関数:
 *   - aggrigate(own_value, children_combined) : 自分の値と子供たちの集計結果から、自分の部分木の値を作る
 *   - marge_child(acc, x)                     : 子供方向の部分木の結果を順次マージ（単位元 e から始める）
 *   - e                                        : marge_child の単位元
 *
 * @note marge_child は結合則を満たす必要がある（左右からの累積で組むため）。
 *       可換性は不要だが、左から acc に右の x をマージする向きで呼ぶ点に注意。
 *
 * メソッド:
 *   rootDP[v]    - 頂点 v を根としたときの木全体のDP結果
 *   dp[v][i]     - 頂点 v から nexts[v][i] 方向に伸びる部分木のDP結果（v を含まない側）
 *   dpmap[{u,v}] - 辺 u->v 方向の部分木のDP結果（dp と同内容を辺ペアで引けるようにしたもの）
 */
template <typename T>
struct RerootingTreeDP{
public:
    /** @brief 頂点数 */
    ll n;

    /** @brief dp[v][i] := 頂点 v から見て nexts[v][i] 方向に伸びる部分木のDP結果 */
    vector<vector<T>> dp;

    /** @brief dpmap[{u,v}] := 辺 u->v 方向（v を含む側）の部分木のDP結果 */
    map<pll, T> dpmap;

    /** @brief rootDP[v] := 頂点 v を根としたときの木全体のDP結果 */
    vector<T> rootDP;

    /** @brief 木の隣接リスト */
    vvll nexts;

    /** @brief 各頂点が持つ値 */
    vector<T> values;

    /** @brief edge_idx[i][j] := 頂点 i から頂点 j に伸びる辺は nexts[i] の何番目か */
    vector<unordered_map<ll,ll>> edge_idx;

    T (*marge_child)(T,T);
    T (*aggrigate)(T,T);

    /** @brief marge_child の単位元 */
    T e;

    /**
     * @brief 全方位木DPを構築
     * @param _nexts       木の隣接リスト
     * @param _values      各頂点の値
     * @param _aggrigate   (自分の値, 子供たちの集計) -> 自分の部分木の値
     * @param _marge_child (累積, 次の子供) -> 累積（単位元 e から始める）
     * @param _e           marge_child の単位元
     */
    RerootingTreeDP(const vvll& _nexts, const vector<T>& _values, T (*_aggrigate)(T,T), T(*_marge_child)(T,T), T _e): nexts(_nexts), values(_values), aggrigate(_aggrigate), marge_child(_marge_child), e(_e){
        n = nexts.size();

        // 逆向きの辺インデックスを引けるようにしておく
        edge_idx.resize(n);
        rep(i, n) rep(j, nexts[i].size()) edge_idx[i][nexts[i][j]] = j;

        rootDP.resize(n);

        // dp を単位元で初期化
        dp.resize(n);
        rep(i,n) dp[i].resize(nexts[i].size(), e);

        // 頂点0を根とした通常の木DPで、根から葉に向かう辺の dp を埋める
        rep(i, nexts[0].size()) setNodeDfs(0, i);

        // BFSで逆向きの辺の dp と rootDP を求める
        rerootingBfs();
    }

    /** @brief デフォルトコンストラクタ（後から代入する用） */
    RerootingTreeDP(){}

    /**
     * @brief 通常の木DPで dp[parent][i] を求める（再帰）
     * @param parent 親頂点
     * @param i      nexts[parent] における今見ている子のインデックス
     *
     * 子供方向を先に再帰で計算してから、自分の部分木の値を集計する。
     */
    void setNodeDfs(ll parent, ll i){
        ll now = nexts[parent][i];
        T childs_value = e;
        rep(ni,  nexts[now].size()){
            if(nexts[now][ni] == parent) continue;
            setNodeDfs(now, ni);
            childs_value = marge_child(childs_value, dp[now][ni]);
        }
        dp[parent][i] = aggrigate(values[now], childs_value);
        dpmap[pll{parent,now}] = dp[parent][i];
        return;
    }

    /**
     * @brief 根からBFSで逆向きの dp を求めつつ、rootDP も決定する
     *
     * 頂点 now について「子供方向の dp[now][*]」が既に求まっている状態で進む。
     * 左右からの累積（left と margeR）を使うことで、
     * 「now の i 番目の隣接頂点を除いた残り全部をマージした値」を O(1) で取り出せ、
     * dp[next][edge_idx[next][now]]（now 側を含む向きの dp）を deg(now) 個まとめて埋められる。
     */
    void rerootingBfs(){

        queue<ll> q;
        q.push(0);

        vb visit(nexts.size());
        visit[0] = true;

        while(q.empty() == false){
            // dp[now][*] は既に求まっている。dp[*][now] の側を求めていく。

            ll now = q.front();
            q.pop();

            // margeR[i] := i 番目より後ろの子供の dp を marge_child で連結した値
            vector<T> margeR(nexts[now].size(), e);
            mrep(i, (ll)nexts[now].size()-2) margeR[i] = marge_child(margeR[i+1], dp[now][i+1]);

            // left := i 番目より前の子供の dp を marge_child で連結した値（ループしながら更新）
            T left = e;

            // 左右からの累積を使い、i 番目の隣接頂点への辺を逆向きに見たときの dp を求める
            rep(i, nexts[now].size()){
                ll next = nexts[now][i];
                dp[next][edge_idx[next][now]] = aggrigate(values[now], marge_child(left, margeR[i]));
                dpmap[pll{now,next}] = dp[next][edge_idx[next][now]];
                left = marge_child(left, dp[now][i]);

                // まだ訪問していない隣接頂点はキューに積む
                if(visit[next] == false){
                    q.push(next);
                    visit[next] = true;
                }
            }

            // now を根としたときの全体の値（left は最終的に全子供をマージした値）
            rootDP[now] = aggrigate(values[now], left);
        }

    }

};
