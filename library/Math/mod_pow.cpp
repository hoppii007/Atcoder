
/**
 * @brief a^b mod p を計算する
 * @param a 底
 * @param b 指数（b >= 0）
 * @return a^b を mod で割った余り
 * @note 計算量: O(log b)
 * @note グローバル変数 mod を法として用いる
 */
ll mpow(ll a, ll b){
    if(b == 0) return 1;
    if(b == 1) return a % mod;
    if(b % 2 == 0) return mpow((a*a) % mod, b/2);
    else return (a % mod) * mpow((a*a) % mod, (b-1)/2) % mod;
}
