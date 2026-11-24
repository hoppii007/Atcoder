#include "template.hpp"  // 先頭に置くこと（プリコンパイル済みヘッダが効く条件）
#include "library/string/substr.cpp"
#include "library/string/is_palindrome.cpp"

int main(){
    cinstr(s);
    ll r = s.size()-1;
    while(r >= 0 && s[r] == 'a') r--;
    if(r == -1){
        cout << "Yes" << endl;
        return 0;
    }
    ll l = 0;
    while(s[l] == 'a') l++;
    string t = substr(s, l, r+1);
    if(is_palindrome(t) && l <= (s.size()-1-r)) cout << "Yes" << endl;
    else cout << "No" << endl;

    return 0;
}