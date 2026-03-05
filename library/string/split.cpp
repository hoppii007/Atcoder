

vs split(string s, char c){
    vs ret;
    s += ".";
    string pre = "";
    rep(i, s.size()){
        if(s[i] == c){
            ret.pb(pre);
            pre = "";
        }
        else{
            pre += string(1, s[i]);
        }
    }
    return ret;
}
