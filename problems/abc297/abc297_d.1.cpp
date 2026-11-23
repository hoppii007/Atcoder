#include "template.hpp"  // 先頭に置くこと（プリコンパイル済みヘッダが効く条件）
int main(){
    cinll(a, b);
    ll ans = 0;
    while(a != b){
        if (b > a) swap(a,b);
        if(a%b == 0){
            ans += a/b-1;
            a = b;
        }
        else {
            ans += a/b;
            a %= b;
        }
    }
    cout << ans << endl;
    return 0;
}