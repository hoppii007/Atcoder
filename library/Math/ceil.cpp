#pragma once
/**
 * @brief 整数の切り上げ除算
 *
 *   ceil(a, b) … ⌈a / b⌉  O(1)
 *
 * @note b != 0。a, b の符号は任意
 *
 * @code
 * ll x = ceil(7, 3);   // 3
 * ll y = ceil(6, 3);   // 2
 * ll z = ceil(-7, 3);  // -2
 * @endcode
 */
ll ceil(ll a, ll b){
    if(b < 0) a = -a, b = -b;
    return a / b + (a%b > 0 ? 1 : 0);
}