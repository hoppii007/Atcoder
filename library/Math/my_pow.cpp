
//O(logB)でa^bが計算できる
ll mypow(ll a,ll b){
    if(b == 0) return 1;
    if(b == 1) return a;
    if(b%2 == 0) return mypow(a*a,b/2);
    else return a*mypow(a*a,(b-1)/2);
}
