/**
 * @brief ダブリング（Doubling）
 *
 * 各状態から1回の遷移先が決まっているとき、任意の状態から K 回先の遷移先を高速に求める。
 * 前処理でダブリングテーブルを構築し、クエリごとに K のビットを見て遷移を合成する。
 *
 * @note 前処理 O(N log K), クエリ O(log K)（N = 状態数, K = 最大遷移回数）
 */
struct Doubling{
    /** @brief dub[i][j] := 状態 i から 2^j 回遷移した先（遷移先がなければ -1） */
    vvll dub;

    ll LOG = 60;

    Doubling(){}

    /**
     * @brief ダブリングテーブルを構築する
     * @param nexts 各状態からの1回の遷移先。遷移先がない場合は -1。
     *              nexts のサイズが状態数 N となる。
     */
    Doubling(vll &nexts){
        ll n = nexts.size();
        dub.resize(n, vll(LOG+1));
        // 2^0 = 1回の遷移先をそのまま格納
        rep(i,n) dub[i][0] = nexts[i];
        // 2^j 回の遷移 = 2^(j-1) 回の遷移を2回適用
        rep(j,1,LOG+1) rep(i,n){
            if(dub[i][j-1] == -1) dub[i][j] = -1;
            else dub[i][j] = dub[dub[i][j-1]][j-1];
        }
    }

    /**
     * @brief 状態 n から k 回遷移した先を返す（単発クエリ用）
     * @param n 開始状態
     * @param k 遷移回数（0 <= k <= 2^60 ≒ 10^18）
     * @return k 回遷移した先の状態。途中で遷移先がなくなった場合は -1。
     * @note 全状態を一括で求める場合は kth_next_all を使うこと（キャッシュ効率が良い）
     */
    ll kth_next(ll n, ll k){
        for(ll j=0;(1LL << j) <= k;j++){
            if(k & (1LL << j)) n = dub[n][j];
            if(n == -1) return -1;
        }
        return n;
    }

    /**
     * @brief 全状態から k 回遷移した先を一括で求める
     * @param k 遷移回数（0 <= k <= 2^60 ≒ 10^18）
     * @return vll dest[i] := 状態 i から k 回遷移した先。遷移先がなければ -1。
     * @note kth_next を N 回呼ぶよりキャッシュ効率が良く高速
     */
    vll kth_next_all(ll k){
        ll n = dub.size();
        vll cur(n);
        rep(i,n) cur[i] = i;
        for(ll j=0;(1LL << j) <= k;j++){
            if(k & (1LL << j)){
                vll nxt(n);
                rep(i,n){
                    if(cur[i] == -1) nxt[i] = -1;
                    else nxt[i] = dub[cur[i]][j];
                }
                cur = nxt;
            }
        }
        return cur;
    }
};