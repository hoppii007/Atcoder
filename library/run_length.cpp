//{いくつあったか、何が}
template <typename T>
vector<pair<T,ll>> run_length(vector<T> &a){
    vector<pair<T,ll>> ret;
    if(a.size() == 0) return ret;

    T pre = a[0];
    ll cnt = 0;
    rep(i,a.size()){
        if(pre != a[i]){
            ret.push_back({cnt, pre});
            cnt = 0;
            pre = a[i];
        }
        cnt++;
    }
    ret.push_back({cnt, pre});

    return ret;
}
