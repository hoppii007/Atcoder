#include "template.hpp"  // 先頭に置くこと（プリコンパイル済みヘッダが効く条件）
#include <numeric>

// vectorを、長さkずつに分割
vvll split_vec_k(vll &a, ll k){
    vvll ret;
    vll t;
    rep(i, a.size()){
        if(i!=0 && i%k == 0){
            ret.pb(t);
            t.clear();
        }
        t.pb(a[i]);
    }
    ret.pb(t);
    return ret;
}

int main(){
    cinll(n, d, p);
    cinvll(f, n);
    sort(all(f));
    reverse(all(f));
    vvll sp = split_vec_k(f, d);
    ll ans = 0;
    rep(i, sp.size()){
        ll total = accumulate(all(sp[i]), 0LL);
        //cout << sp[i] << " :" << total << endl; 
        if(total >= p) ans += p;
        else ans += total;
    }
    cout << ans << endl;
    return 0;
}