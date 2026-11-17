#pragma once
/**
 * @brief a に最も近い平方数 N^2 を探す（戻り値は {N^2, N}）
 *
 *   sqlt(a)  … a 未満の最大の平方数
 *   sqlte(a) … a 以下の最大の平方数
 *   sqgt(a)  … a より大きい最小の平方数
 *   sqgte(a) … a 以上の最小の平方数
 *
 * @note 0 <= a <= 10^18 程度。O(1)（sqrt の前後 200 点を調べる）
 * @note 該当なし（sqlt(0)）のときは {-1, -1} を返す
 *
 * @code
 * auto [sq, n] = sqlte(10);  // sq = 9, n = 3
 * @endcode
 */
pll sqlt(ll a){
    ll sq = round(sqrt(a));
    pll ret = {-1, -1};
    rep(i,-100, 101){
        ll cand = max(0LL, (sq+i));
        if(cand*cand < a) ret = {cand*cand, cand};
    }
    return ret;
}

pll sqlte(ll a){
    ll sq = round(sqrt(a));
    pll ret;
    rep(i,-100, 101){
        ll cand = max(0LL, (sq+i));
        if(cand*cand <= a) ret = {cand*cand, cand};
    }
    return ret;
}

pll sqgt(ll a){
    ll sq = round(sqrt(a));
    pll ret;
    rep(i, -100, 101){
        ll cand = max(0LL, (sq+i));
        if(cand*cand > a){
            ret = {cand*cand, cand};
            break;
        }
    }
    return ret;
}

pll sqgte(ll a){
    ll sq = round(sqrt(a));
    pll ret;
    rep(i, -100, 101){
        ll cand = max(0LL, (sq+i));
        if(cand*cand >= a){
            ret = {cand*cand, cand};
            break;
        }
    }
    return ret;
}
