#include "template.hpp"  // 先頭に置くこと（プリコンパイル済みヘッダが効く条件）
#include "library/prefix_sum_2d.cpp"

int main(){
    cinll(h, w, n);
    vvll g(h, vll(w, 0));
    rep(i, n){
        cinll(a,b);
        a--; b--;
        g[a][b] = 1;
    }

    vvll s = prefix_sum_2d(g);
    ll ans = 0;
    rep(i, h) rep(j, w) if(g[i][j] == 0) {
        // [i,j]から始まる穴なし正方形が最大どこまで作れるか？
        //二分探索のジャッジ用のやつ
        auto nibutan_judge = [&](ll mid){
            // [i,i+mid) x [j,j+mid)がOKか？
            if(i+mid > h) return false;
            if(j+mid > w) return false;
            return rect_sum(s, i,j ,i+mid, j+mid) == 0;
        };
        ll ok = 1;
        ll ng = 3500;
        while(abs(ok-ng) > 1){
            ll mid = (ok+ng)/2;
            if(nibutan_judge(mid)) ok = mid;
            else ng = mid;
        }
        ans += ok;
    }
    cout << ans << endl;
    return 0;
}