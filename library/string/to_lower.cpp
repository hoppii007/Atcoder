
string to_lower(string s){
    string ret = "";
    rep(i,s.size()) {
        if('A' <= s[i] && s[i] <= 'Z') ret += s[i]-'A'+'a';
        else ret += s[i];
    }
    return ret;
}
