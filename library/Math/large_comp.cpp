
// a <= b*cを判定
bool lte(ll a, ll b, ll c){
    if(b == 0 || c == 0) return a <= 0;
    if(a%b == 0) return a/b <= c;
    return a/b < c;
}
// a < b*c
bool lt(ll a, ll b, ll c){
    if(b == 0 || c == 0) return a < 0;
    return a/b < c;
}
// a >= b*c
bool gte(ll a, ll b, ll c){
    if(b == 0 || c == 0) return a >= 0;
    return a/b >= c;
}
// a > b*c
bool gt(ll a, ll b, ll c){
    if(b == 0 || c == 0) return a > 0;
    if(a%b == 0) return a/b > c;
    return a/b+1>c;
}
