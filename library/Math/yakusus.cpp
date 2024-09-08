//O(root(N))
vll yakusus(ll n){
    vll ret(1,1);
    for(ll i=2;i*i<=n;i++){
        if(n%i == 0){
            ret.push_back(i);
            if(n/i != i) ret.push_back(n/i);
        }
    }
    return ret;
}