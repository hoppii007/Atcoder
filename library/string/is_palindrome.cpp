
//回文判定
bool is_palindrome(string s){
    rep(i,s.size()/2) if(s[i] != s[(ll)s.size()-1-i]) return false;
    return true;
}
