#include "template.hpp"  // 先頭に置くこと（プリコンパイル済みヘッダが効く条件）
#include "library/string/to_lower.cpp"
#include "library/string/to_upper.cpp"

int main(){
    cinll(n);
    cinstr(s);
    // change[i] := i文字目の変更で最後に行われたものの、{タイミング、文字}
    vector<pair<ll,char>> change(n);
    rep(i, n){
        change[i] = {0, s[i]};
    }
    // 最後に起こった大文字小文字変換。trueなら大文字
    pair<ll,bool> ulchange = {-1, false};
    cinll(q);
    ll i = 1;
    while(q--){
        cinll(t,x);
        x--;
        cinchar(c);
        if(t == 1) change[x] = {i, c};
        else if(t == 2) ulchange = {i, false};
        else ulchange = {i, true};
        i++;
    }
    rep(i, n){
        if(ulchange.fst > change[i].fst){
            if(ulchange.snd) cout << to_upper(change[i].snd);
            else cout << to_lower(change[i].snd);
        }
        else cout << change[i].snd;
    }
    cout << endl;
    return 0;
}