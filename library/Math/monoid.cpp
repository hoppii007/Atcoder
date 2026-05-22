/**
 * @brief モノイド（Monoid）
 *
 * 値の型 T・二項演算 op・単位元 E を1つの型に束ねる「型レベルの設定」。
 * 値そのものは保持せず、ライブラリ側で `T` を直接持って `M::merge(a, b)` や `M::e`
 * を呼び出して使う想定（AtCoder Library の `segtree<S, op, e>` と同じ流儀）。
 *
 * @tparam T  値の型（integral 型のみ。値そのものを単位元として渡すため）
 * @tparam op 二項演算 T(*)(T, T)（結合則を満たすこと）
 * @tparam E  単位元の値
 *
 * @code
 * // 和モノイドを定義
 * ll op_sum(ll a, ll b){ return a + b; }
 * using Sum = Monoid<ll, op_sum, 0>;
 *
 * // 使用例
 * ll x = Sum::merge(3, 5);  // 8
 * ll e = Sum::e;            // 0
 * using T = Sum::value_type; // ll
 *
 * // ライブラリへの渡し方
 * vll fval = {1, 2, 3};
 * vll nexts = {1, 2, -1};
 * Doubling<Sum> d(fval, nexts);
 * @endcode
 */
template<typename T, T (*op)(T, T), T E>
struct Monoid {
    /** @brief 値の型エイリアス（テンプレート外から `M::value_type` で参照） */
    using value_type = T;

    /** @brief 単位元 */
    static constexpr T e = E;

    /** @brief 二項演算（外部から `M::merge(a, b)` で呼べる） */
    static constexpr auto merge = op;
};
