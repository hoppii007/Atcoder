#pragma once
/**
 * @brief 乱数（mt19937_64）
 *
 *   randll(a, b) … [a, b) の一様乱数
 *   rndmt        … 乱数生成器本体（shuffle などに渡せる）
 *
 * @code
 * ll x = randll(0, 10);  // 0〜9
 * shuffle(all(v), rndmt);
 * @endcode
 */
std::mt19937_64 rndmt(std::random_device{}());
ll randll(ll a, ll b){
    std::uniform_int_distribution<ll> r(a, b-1);
    return r(rndmt);
}
