#include "template.hpp"  // 先頭に置くこと（プリコンパイル済みヘッダが効く条件）

int main(){
    cinll(n);
    cinstr(s);
    bool is_in = false;
    rep(i, n){
        if(s[i] == '"') is_in = !is_in;
        if(s[i] == ',' && is_in == false) s[i] = '.';
        cout << s[i];
    }
    return 0;
}