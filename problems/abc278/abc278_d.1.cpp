#include "template.hpp"  // 先頭に置くこと（プリコンパイル済みヘッダが効く条件）

int main(){
    cinll(n);
    // a[i] := {値、時刻}
    vpll a(n);
    rep(i, n) {
        cin >> a[i].fst;
        a[i].snd = 0;
    }
    cinll(q);
    // {値、時刻}
    pll b = {0, -1};
    ll m = 1;
    while(q--){
        cinll(t)
        if(t == 1){
            cinll(x);
            b = {x, m};
        }
        else if(t == 2){
            cinll(i, x);
            i--;
            if(a[i].snd < b.snd){
                a[i] = b;
            }
            a[i].fst += x;
            a[i].snd = m;
        }
        else{
            cinll(i);
            i--;
            if(a[i].snd < b.snd){
                a[i] = b;
            }
            cout << a[i].fst << endl;
        }
        m++;
    }
    return 0;
}