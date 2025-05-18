
//O(a.size)
//0,1,2,...の中で、aに含まれない最小
ll mex(vll &a){
    vb f(max(0LL, *max_element(all(a))),false);
    rep(i,a.size()) if(a[i] < f.size()) f[a[i]] = true;
    rep(i,f.size()) if(f[i] == false) return i;
    return (ll)f.size()-1;
}
