
// a以上b未満の乱数を返す
std::mt19937_64 rndmt(std::random_device{}());
ll randll(ll a, ll b){
    std::uniform_int_distribution<ll> r(a, b-1);
    return r(rndmt);
}
