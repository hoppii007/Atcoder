struct ModComb {
    vector<modll> f;
    vector<modll> invf;

    // O(N) 最大Nまでのコンビネーションを計算できるようにする
    void init(ll n){
        f.assign(n+1, 1);
        rep(i, 1, n+1) f[i] = i*f[i-1];
        invf.assign(n+1, 1/f[n]);
        mrep(i, n-1) invf[i] = (i+1)*invf[i+1];
    }

    modll comb(ll n, ll k){
        if(n < k) return 0;
        if(n < 0 || k < 0) return 0;
        return f[n] * invf[k] * invf[n-k];
    }
};