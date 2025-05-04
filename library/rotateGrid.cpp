
//正方形のグリッドを右90度回転
template<typename T>
vector<T> rotateGrid(vector<T> &s){
    vector<T> ret(s.size());
    rep(j,s.size()) rep(i,s.size()) ret[j] += s[(ll)s.size()-1-i][j];
    return ret;
}
