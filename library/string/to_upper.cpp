
string to_upper(string s){
    string ret = "";
    rep(i,s.size()){
        if('a' <= s[i] && s[i] <= 'z') ret += s[i] -'a' + 'A';
        else ret += s[i];
    }
    return ret;
}
