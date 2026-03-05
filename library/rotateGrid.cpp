
//正方形のグリッドを右90度回転
template<typename T>
vector<vector<T>> rotateGrid(vector<vector<T>> &s){
    vector<vector<T>> ret(s.size(), vector<T>(s.size()));
    rep(j,s.size()) rep(i,s.size()) ret[j][i] += s[(ll)s.size()-1-i][j];
    return ret;
}
