#pragma once
/**
 * @brief ダブリング（各状態の遷移先から K 回先を高速に求める）
 *
 *   doubling d(nexts)            … テーブル構築  O(N log K)
 *   d.kth_next(n, k)             … n から k 回遷移した先（途中で無くなれば -1）  O(log k)
 *   d.kth_next_all(k)            … 全状態から k 回遷移した先の配列  O(N log k)
 *   d.max_step_while(n, pred)    … pred が真の間だけ進んだ {回数, 到達状態}  O(log K)
 *   d.dub[i][j]                  … i から 2^j 回遷移した先（無ければ -1）
 *
 * @param nexts 各状態の1回の遷移先（無ければ -1）。サイズが状態数 N
 * @note 状態は 0-indexed。k < 2^61 であること
 * @note max_step_while: pred(n) が真で、経路上で pred が true…true→false…false と単調であること
 *
 * @code
 * vll nexts = {1, 2, 3, -1};
 * doubling d(nexts);
 * ll x = d.kth_next(0, 2);   // 2
 * ll y = d.kth_next(0, 5);   // -1
 * auto [cnt, last] = d.max_step_while(0, [&](ll s){ return s <= 2; });  // {2, 2}
 * @endcode
 */
struct doubling{
    vvll dub;

    ll LOG = 60;

    doubling(){}

    doubling(vll &nexts){
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

    ll kth_next(ll n, ll k){
        for(ll j=0;(1LL << j) <= k;j++){
            if(k & (1LL << j)) n = dub[n][j];
            if(n == -1) return -1;
        }
        return n;
    }

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

    template<typename F>
    pair<ll,ll> max_step_while(ll n, F pred){
        ll cnt = 0;
        for(ll j=LOG;j>=0;j--){
            ll nxt = dub[n][j];
            if(nxt != -1 && pred(nxt)){
                n = nxt;
                cnt += (1LL << j);
            }
        }
        return {cnt, n};
    }
};