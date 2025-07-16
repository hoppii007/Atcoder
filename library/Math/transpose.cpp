
template<typename T>
vector<vector<T>> transpose(vector<vector<T>> &a){
    ll h = a.size();
    ll w = a[0].size();
    
    vector<vector<T>> ret(w, vector<T>(h));
    for(int i=0;i<h;i++){
        for(int j=0;j<w;j++){
            ret[j][i] = a[i][j];
        }
    }
    return ret;
}
