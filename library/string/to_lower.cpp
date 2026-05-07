
string to_lower(string s){
    string ret = "";
    rep(i,s.size()) {
        if('A' <= s[i] && s[i] <= 'Z') ret += string(1,s[i]-'A'+'a');
        else ret += string(1,s[i]);
    }
    return ret;
}
