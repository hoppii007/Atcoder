
string to_upper(string s){
    string ret = "";
    rep(i,s.size()){
        if('a' <= s[i] && s[i] <= 'z') ret += string(1, s[i] -'a' + 'A');
        else ret += string(1, s[i]);
    }
    return ret;
}
