
//O(max(a) + a.size())
//0,1,2,...の中で、aに含まれない最小
ll mex(vll& a){
    if(a.size() == 0) return 0;
    vb f(max(0LL, *max_element(all(a))+1),false);
    rep(i,a.size()) if(a[i] < f.size()) f[a[i]] = true;
    rep(i,f.size()) if(f[i] == false) return i;
    return (ll)f.size();
}
