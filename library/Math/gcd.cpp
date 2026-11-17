#pragma once
/**
 * @brief 最大公約数
 *
 *   gcd(a, b) … a と b の最大公約数  O(log min(a, b))
 *
 * @note a, b >= 0。gcd(a, 0) = a
 *
 * @code
 * ll g = gcd(12, 18);  // 6
 * @endcode
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

