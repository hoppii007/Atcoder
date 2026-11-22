#include "template.hpp"  // 先頭に置くこと（プリコンパイル済みヘッダが効く条件）

ll f(vll& to, vll& a, ll now){
    if(a[to[now]] == -1) return to[now];
    return to[now] = f(to, a, to[now]);
}

int main(){
    ll n = (1LL << 20);
    vll ans(n, -1);
    vll to(n);
    rep(i, n) to[i] = i;
    cinll(q);
    while(q--){
        cinll(t, x);
        //cout << t << " " << x << endl;
        if(t == 1){
            ll h = x%n;
            ll nxt = f(to, ans, h);
            ans[nxt] = x;
            to[nxt] = (to[nxt]+1)%n;
        }
        else{
            cout << ans[x%n] << endl;
        }
    }
    return 0;
}