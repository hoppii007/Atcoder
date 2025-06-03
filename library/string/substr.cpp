
//部分文字列を切り出す
//閉区間[i,j]でとる
string substr(string s, ll i, ll j){
	string t = "";
	rep(ii,i,j+1) t += string(1,s[ii]);
	return t;
}
