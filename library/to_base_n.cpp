
//aをn進数表記する
//[0,1,1]が帰ってきたとき、0a^0 + 1a^1 + 1a^2
vll to_base_n(ll a, ll n){
    vll ret;
    while(a > 0){
        ret.push_back(a%n);
        a /= n;
    }
    return ret;
}
