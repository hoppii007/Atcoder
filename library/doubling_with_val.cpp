#pragma once
/**
 * @brief ダブリング＋モノイド値（K 回先の状態と経路上の辺の値の累積を求める）
 *
 *   DoublingWithVal<M> d(nexts, vals)  … テーブル構築  O(N log K)
 *   d.kth_next(n, k)                   … {累積値, n から k 回遷移した先}  O(log k)
 *   d.kth_next_all(k)                  … 全状態について {累積値, 到達状態} の配列  O(N log k)
 *
 * @tparam M モノイド（library/Math/monoid.cpp の Monoid）
 * @param nexts 各状態の1回の遷移先（無ければ -1）
 * @param vals  vals[i] = 状態 i 発の辺の値（M::value_type）
 * @note 途中で遷移先が無くなると到達状態は -1、累積値はそこまでの分。k = 0 なら {M::e(), n}
 * @note 状態は 0-indexed。k < 2^61 であること
 *
 * @code
 * ll op_sum(ll a, ll b){ return a + b; }
 * ll e_sum(){ return 0; }
 * using Sum = Monoid<ll, op_sum, e_sum>;
 *
 * vll nexts = {1, 2, 3, -1};
 * vll vals  = {10, 20, 30, 0};
 * DoublingWithVal<Sum> d(nexts, vals);
 * auto [acc, dest] = d.kth_next(0, 2);  // {30, 2}
 * @endcode
 */
template<typename M>
struct DoublingWithVal {
    using T = typename M::value_type;
    static constexpr ll LOG = 60;

    vvll dub;
    vector<vector<T>> val;

    DoublingWithVal(){}

    DoublingWithVal(vll& nexts, vector<T>& vals){
        ll n = nexts.size();
        dub.assign(n, vll(LOG+1, -1));
        val.assign(n, vector<T>(LOG+1, M::e()));
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

    pair<T, ll> kth_next(ll n, ll k){
        T ret = M::e();
        for(ll j=0;(1LL << j) <= k;j++){
            if(k & (1LL << j)){
                ret = M::merge(ret, val[n][j]);
                n = dub[n][j];
            }
            if(n == -1) return {ret, n};
        }
        return {ret, n};
    }

    vector<pair<T, ll>> kth_next_all(ll k){
        ll n = dub.size();
        vector<pair<T, ll>> cur(n);
        rep(i, n) cur[i] = {M::e(), i};
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
