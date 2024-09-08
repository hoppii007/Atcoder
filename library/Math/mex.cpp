//O(a.size)
ll mex(vll &a){
    vb f(a.size()+1,false);
    rep(i,a.size()) if(a[i] < f.size()) f[a[i]] = true;
    rep(i,f.size()) if(f[i] == false) return i;
    return (ll)f.size()-1;
}