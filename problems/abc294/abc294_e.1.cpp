#include "template.hpp"  // 先頭に置くこと（プリコンパイル済みヘッダが効く条件）

int main(){
    cinll(l, n1, n2);
    // {値、個数}
    cinvpll(a, n1);
    cinvpll(b, n2);
    // 何番目の区画にいるか？
    ll ai = 0;
    ll bi = 0;
    // ai,biより前にいくつ文字があったか？
    ll ap = 0;
    ll bp = 0;
    // 前回引いたライン、その時始まったbとaの数値
    ll p = 0;
    ll ans = 0;
    while(true){
        if(a[ai].snd+ap <= b[bi].snd+bp){
            if(b[bi].fst == a[ai].fst){
                ans += (a[ai].snd+ap-p);
            }
            ap += a[ai].snd;
            p = ap;
            ai++;
        }
        else {
            if(b[bi].fst == a[ai].fst){
                ans += (b[bi].snd+bp-p);
            }
            bp += b[bi].snd;
            p = bp;
            bi++;
        }
        if(ai == n1 || bi == n2) break;
    }
    cout << ans << endl;
}