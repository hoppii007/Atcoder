
// a未満の最大の平方数と、その時のNを返す
// O(1) （200回くらい）
// 0 <= a <= 10^18くらいなら耐える
pll sqlt(ll a){
    ll sq = round(sqrt(a));
    pll ret;
    rep(i,-100, 101){
        ll cand = max(0LL, (sq+i));
        if(cand*cand < a) ret = {cand*cand, cand};
    }
    return ret;
}

// a以下の最大の平方数と、その時のNを返す
// O(1) （200回くらい）
// 0 <= a <= 10^18くらいなら耐える
pll sqlte(ll a){
    ll sq = round(sqrt(a));
    pll ret;
    rep(i,-100, 101){
        ll cand = max(0LL, (sq+i));
        if(cand*cand <= a) ret = {cand*cand, cand};
    }
    return ret;
}

// aより大きい最小の平方数と、その時のNを返す
// O(1)（200回くらい）
// 0 <= a <= 10^18くらいなら耐える
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

// a以上の最小の平方数と、その時のNを返す
// O(1)（200回くらい）
// 0 <= a <= 10^18くらいなら耐える
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
