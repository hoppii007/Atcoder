/**
 * @brief a を b で割った値の切り上げを返す
 * @param a 被除数
 * @param b 除数（正の整数）
 * @return ⌈a / b⌉
 * @note 計算量: O(1)
 * @note 例: ceil(7, 3) = 3, ceil(6, 3) = 2
 */
ll ceil(ll a, ll b){
    return a / b + (a%b != 0 ? 1 : 0);
}