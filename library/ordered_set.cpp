#pragma once
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
/**
 * @brief 順序付き集合（GNU pb_ds の tree。k 番目の要素・x 未満の個数を O(log N) で求められる set）
 *
 *   ordered_set<T> s              … 重複なし（set 相当）
 *   s.insert(x) / s.erase(x)      … 追加・削除  O(log N)
 *   s.find_by_order(k)            … 0-indexed で k 番目（小さい順）の要素のイテレータ。k >= size なら end()  O(log N)
 *   s.order_of_key(x)             … x 未満の要素数  O(log N)
 *   lower_bound / upper_bound / begin / end / size … std::set と同じ
 *
 *   ordered_multiset<T> ms        … 重複あり（multiset 相当。内部は {値, 通し番号} の ordered_set）
 *   ms.insert(x)                  … x を 1 個追加  O(log N)
 *   ms.erase(it)                  … イテレータ it の要素だけ削除。次の要素のイテレータを返す  O(log N)
 *   ms.erase(x)                   … 値 x を全部削除。削除した個数を返す  O((個数 + 1) log N)
 *   ms.erase_one(x)               … 値 x を 1 個だけ削除（無ければ false）  O(log N)
 *   ms.count(x)                   … x の個数  O(log N)
 *   ms.find(x)                    … 値 x のうち先頭のイテレータ。無ければ end()  O(log N)
 *   ms.find_by_order(k)           … 0-indexed で k 番目（小さい順）のイテレータ。k >= size なら end()  O(log N)
 *   ms.order_of_key(x)            … x 未満の要素数  O(log N)
 *   lower_bound / upper_bound / begin / end / size … std::multiset と同じ
 *   ※ イテレータは *it で値（const T&）を返す。双方向（++ / --）
 *
 * 降順にしたいときは ordered_set<T, greater<T>> とする。
 * ※ less_equal<T> で multiset 化する小技は erase / find / lower_bound が壊れるので使わない。
 *
 * @code
 * ordered_set<ll> s;
 * s.insert(5); s.insert(1); s.insert(3);
 * ll a = *s.find_by_order(1);   // 3
 * ll b = s.order_of_key(4);     // 2  （1, 3）
 *
 * ordered_multiset<ll> ms;
 * ms.insert(2); ms.insert(2); ms.insert(7);
 * ll c = *ms.find_by_order(1);  // 2
 * ll d = ms.order_of_key(7);    // 2
 * ms.erase_one(2);              // true （2 が 1 個残る）
 * ms.erase(ms.find(7));         // 7 を 1 個削除
 * ms.erase(2);                  // 2 を全部削除して 1 を返す
 * @endcode
 */
template <typename T, typename Comp = less<T>>
using ordered_set = __gnu_pbds::tree<T, __gnu_pbds::null_type, Comp, __gnu_pbds::rb_tree_tag,
                                     __gnu_pbds::tree_order_statistics_node_update>;

template <typename T>
struct ordered_multiset {
    using base_set = ordered_set<pair<T, ll>>;
    using base_iterator = typename base_set::iterator;

    /// @brief *it で値を返す双方向イテレータ
    struct iterator {
        using iterator_category = bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = const T *;
        using reference = const T &;

        base_iterator it;
        iterator() {}
        iterator(base_iterator it_) : it(it_) {}
        const T &operator*() const { return it->first; }
        const T *operator->() const { return &it->first; }
        iterator &operator++() { ++it; return *this; }
        iterator operator++(int) { iterator r = *this; ++it; return r; }
        iterator &operator--() { --it; return *this; }
        iterator operator--(int) { iterator r = *this; --it; return r; }
        bool operator==(const iterator &o) const { return it == o.it; }
        bool operator!=(const iterator &o) const { return it != o.it; }
    };

    base_set s;
    ll id = 0;  // 同じ値を区別するための通し番号

    void insert(const T &x) { s.insert({x, id++}); }

    iterator erase(iterator pos) {
        iterator nx = std::next(pos);
        s.erase(pos.it);
        return nx;
    }

    ll erase(const T &x) {
        ll cnt = 0;
        for (auto it = s.lower_bound({x, LLONG_MIN}); it != s.end() && it->first == x; cnt++) {
            auto nx = std::next(it);
            s.erase(it);
            it = nx;
        }
        return cnt;
    }

    bool erase_one(const T &x) {
        auto it = s.lower_bound({x, LLONG_MIN});
        if (it == s.end() || it->first != x) return false;
        s.erase(it);
        return true;
    }

    ll count(const T &x) const {
        return (ll)s.order_of_key({x, LLONG_MAX}) - (ll)s.order_of_key({x, LLONG_MIN});
    }

    iterator find(const T &x) const {
        auto it = s.lower_bound({x, LLONG_MIN});
        if (it == s.end() || it->first != x) return end();
        return iterator(it);
    }

    iterator find_by_order(ll k) const { return iterator(s.find_by_order(k)); }

    ll order_of_key(const T &x) const { return s.order_of_key({x, LLONG_MIN}); }

    iterator lower_bound(const T &x) const { return iterator(s.lower_bound({x, LLONG_MIN})); }
    iterator upper_bound(const T &x) const { return iterator(s.upper_bound({x, LLONG_MAX})); }
    iterator begin() const { return iterator(s.begin()); }
    iterator end() const { return iterator(s.end()); }

    ll size() const { return s.size(); }
};
