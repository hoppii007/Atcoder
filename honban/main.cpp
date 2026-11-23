#include "template.hpp"  // 先頭に置くこと（プリコンパイル済みヘッダが効く条件）
#include "library/Math/ceil.cpp"
#include <numeric>

//各商品の重さ配列w、価値配列vとした時、容量Hのナップサックに詰め込める価値の最大
// O(NH)
ll knapsack(ll H, vll& w, vll& v){
    ll n = w.size();
    // dp[i][j] := i番目まで考えた時、重さ合計をj以下にする時の最大価値
    vvll dp(n, vll(H+1, 0));
    rep(i, min(w[0], H)+1) dp[0][i] = v[0];
    rep(i, 1, n) rep(j, H+1){
        dp[i][j] = dp[i-1][j];
        if(j >= w[i]) chmax(dp[i][j], dp[i-1][j-w[i]]+v[i]);
    }
    return dp[n-1][H];
}

// 各商品の重さ配列w、価値配列vとした時、容量Hのナップサックに詰め込める価値の最大
ll knapsack2(ll H, vll& w, vll& v){ 
    ll n = w.size();
    ll c = accumulate(all(v), 0LL);
    // dp[i][j] := i番目まで考えた時、価値の合計をjにする時の最小の重さ
    vvll dp(n, vll(c+1, inf));
    dp[0][v[0]] = w[0];
    rep(i, n) dp[i][0] = 0;
    rep(i, 1, n) rep(j, c+1){
        dp[i][j] = dp[i-1][j];
        if(j >= v[i]) chmin(dp[i][j], dp[i-1][j-v[i]]+w[i]);
    }
    mrep(i, c) if(dp[n-1][i] <= H) return i;
    return 0;
}

int main(){
    cinll(n);
    vll x(n), y(n), z(n);
    vll w, v;
    ll takz = 0;
    rep(i, n) {
        cin >> x[i] >> y[i] >> z[i];
        if(x[i] < y[i]) {
            w.pb(ceil(y[i]-x[i], 2));
            v.pb(z[i]);
        }
        else{
            takz += z[i];
        }
    }
    if(w.size() == 0){
        cout << 0 << endl;
        return 0;
    }

    ll totz = accumulate(all(z), 0LL);
    //cout << "ngo" << endl;
    
    //二分探索のジャッジ用のやつ
    auto nibutan_judge = [&](ll mid){
        return 2*(knapsack2(mid, w, v)+takz) > totz;
    };
    ll ok = 1000000000000LL;
    ll ng = -1;
    while(abs(ok-ng) > 1){
        ll mid = (ok+ng)/2;
        if(nibutan_judge(mid)) ok = mid;
        else ng = mid;
    }

    cout << ok << endl;
    return 0;
}