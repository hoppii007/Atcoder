/**
 * @brief ダブリング（Doubling）＋ モノイド値の累積
 *
 * 各状態から1回の遷移先が決まっており、各遷移にモノイド値が乗っているとき、
 * 任意の状態から K 回遷移した先と、その経路で得られる値の累積（M::merge による畳み込み）を高速に求める。
 *
 * @tparam M モノイド型（library/Math/monoid.cpp の Monoid を想定）
 *
 * @note 前処理 O(N log K), クエリ O(log K)（N = 状態数, K = 最大遷移回数）
 *
 * @code
 * ll op_sum(ll a, ll b){ return a + b; }
 * using Sum = Monoid<ll, op_sum, 0>;
 *
 * vll vals  = {10, 20, 30, 0};   // 状態 i 発の辺の値（M::value_type 型）
 * vll nexts = {1, 2, 3, -1};     // 状態 i の遷移先（なければ -1）
 * Doubling<Sum> d(vals, nexts);
 *
 * auto [acc, dest] = d.kth_next(0, 2);  // {30, 2}（10+20）
 * @endcode
 */
template<typename M>
struct Doubling {
    using T = typename M::value_type;
    static constexpr ll LOG = 60;

    /** @brief dub[i][j] := 状態 i から 2^j 回遷移した先（遷移先がなければ -1） */
    vvll dub;
    /** @brief val[i][j] := 状態 i から 2^j 回遷移する間に通る辺の値を M::merge で畳み込んだ結果 */
    vector<vector<T>> val;

    Doubling(){}

    /**
     * @brief ダブリングテーブルを構築する
     * @param vals  各状態から1回遷移する辺に乗る値。vals[i] が状態 i 発の辺の値。
     * @param nexts 各状態からの1回の遷移先。遷移先がない場合は -1。
     *              nexts のサイズが状態数 N となる（vals も同じサイズ）。
     */
    Doubling(vll& nexts, vector<T>& vals){
        ll n = nexts.size();
        dub.assign(n, vll(LOG+1, -1));
        val.assign(n, vector<T>(LOG+1, M::e));
        // 2^0 = 1回の遷移先と辺の値をそのまま格納
        rep(i, n){
            dub[i][0] = nexts[i];
            val[i][0] = vals[i];
        }
        // 2^j 回の遷移 = 2^(j-1) 回の遷移を2回適用
        rep(j, 1, LOG+1) rep(i, n){
            if(dub[i][j-1] == -1){
                dub[i][j] = -1;
                val[i][j] = val[i][j-1];
            } else {
                dub[i][j] = dub[dub[i][j-1]][j-1];
                val[i][j] = M::merge(val[i][j-1], val[dub[i][j-1]][j-1]);
            }
        }
    }

    /**
     * @brief 状態 n から k 回遷移した先と、その経路の値の累積を返す（単発クエリ用）
     * @param n 開始状態
     * @param k 遷移回数（0 <= k <= 2^60 ≒ 10^18）
     * @return {累積値, 到達状態} のペア。途中で遷移先がなくなった場合、到達状態は -1
     *         （累積値はそこまでに通った辺の累積）。k = 0 のときは {M::e, n}。
     * @note 全状態を一括で求める場合は kth_next_all を使うこと（キャッシュ効率が良い）
     */
    pair<T, ll> kth_next(ll n, ll k){
        T ret = M::e;
        for(ll j=0;(1LL << j) <= k;j++){
            if(k & (1LL << j)){
                ret = M::merge(ret, val[n][j]);
                n = dub[n][j];
            }
            if(n == -1) return {ret, n};
        }
        return {ret, n};
    }

    /**
     * @brief 全状態から k 回遷移した先と、その経路の値の累積を一括で求める
     * @param k 遷移回数（0 <= k <= 2^60 ≒ 10^18）
     * @return dest[i] := {状態 i からの経路の累積値, 到達状態}。
     *         途中で遷移先がなくなった場合、到達状態は -1（累積値はそこまでの累積）。
     * @note kth_next を N 回呼ぶよりキャッシュ効率が良く高速
     */
    vector<pair<T, ll>> kth_next_all(ll k){
        ll n = dub.size();
        vector<pair<T, ll>> cur(n);
        rep(i, n) cur[i] = {M::e, i};
        for(ll j=0;(1LL << j) <= k;j++){
            if(k & (1LL << j)){
                vector<pair<T, ll>> nxt(n);
                rep(i, n){
                    if(cur[i].snd == -1) nxt[i] = cur[i];
                    else nxt[i] = {M::merge(cur[i].fst, val[cur[i].snd][j]), dub[cur[i].snd][j]};
                }
                cur = nxt;
            }
        }
        return cur;
    }
};
