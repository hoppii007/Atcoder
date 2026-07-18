

/**
 * @brief a と b の最大公約数を求める
 * @param a 非負整数
 * @param b 非負整数
 * @return gcd(a, b)
 * @note 計算量: O(log min(a, b))
 */
ll gcd(ll a,ll b){
	if(b == 0) return a;
    if(a<b) return gcd(b,a);
    ll r;
    while((r=a%b)){
        a = b;
        b = r;
    }
    return b;
}

