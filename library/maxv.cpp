
//{v,i}の順で入れる
template<typename T>
pair<T,ll> maxvi(vector<T> &a){
    pair<T,ll> ret = {a[0],0};
    rep(i,a.size()) if(ret.first < a[i]){
        ret = {a[i],i};
    }
    return ret;
}

template<typename T>
T maxv(vector<T> &a){
    T ret = a[0];
    rep(i,a.size()) if(ret < a[i]){
        ret = a[i];
    }
    return ret;
}

//{v,i}の順で入れる
template<typename T>
pair<T,ll> minvi(vector<T> &a){
    pair<T,ll> ret = {a[0],0};
    rep(i,a.size()) if(ret.first > a[i]){
        ret = {a[i],i};
    }
    return ret;
}

template<typename T>
T minv(vector<T> &a){
    T ret = a[0];
    rep(i,a.size()) if(ret > a[i]){
        ret = a[i];
    }
    return ret;
}
