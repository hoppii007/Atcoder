#include "template.hpp"  // 先頭に置くこと（プリコンパイル済みヘッダが効く条件）

int main(){
    cinll(n);
    cinvs(s, n);
    ll ans = inf;
    // 各文字をkにすることを考える
    rep(k, 10){
        // 各文字を k にする時に最後に押す時間
        ll t = 0;
        rep(i, 10) {
            ll cnt = 0;
            rep(j, n) if(s[j][i]-'0' == k) cnt++;
            chmax(t, i+10*(cnt-1));
        }
        chmin(ans, t);
    }
    cout << ans << endl;

    return 0;
}