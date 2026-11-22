#include "template.hpp"  // 先頭に置くこと（プリコンパイル済みヘッダが効く条件）


int main(){
    cinll(n, k);
    cinvll(a, n);
    
    //二分探索のジャッジ用のやつ
    auto nibutan_judge = [&](ll mid){
        // プロジェクトをmid個作れるか？
        ll total = 0;
        rep(i, n) total += min(a[i], mid);
        return total >= mid*k;
    };
    ll ok = 0;
    ll ng = inf/k;
    while(abs(ok-ng) > 1){
        ll mid = (ok+ng)/2;
        if(nibutan_judge(mid)) ok = mid;
        else ng = mid;
    }
    cout << ok << endl;
    return 0;
}