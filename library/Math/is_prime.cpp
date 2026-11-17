#pragma once
/**
 * @brief 素数判定（試し割り）
 *
 *   is_prime(a) … a が素数なら true  O(√a)
 *
 * @note a <= 1 は false
 *
 * @code
 * bool p = is_prime(97);  // true
 * @endcode
 */
bool is_prime(ll a){
    if(a <= 1) return false;
    if(a == 2) return true;

    for(ll i=2;i*i <= a;i++){
        if(a%i == 0) return false;
    }
    return true;
}
