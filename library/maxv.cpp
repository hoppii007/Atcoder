
//{v,i}の順で入れる
template<typename T>
pair<T,ll> maxv(vector<T> &a){
    pair<T,ll> ret = {a[0],0};
    rep(i,a.size()) if(ret.first < a[i]){
        ret = {a[i],i};
    }
    return ret;
}
