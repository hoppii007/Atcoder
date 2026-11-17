#pragma once
#include "library/Math/monoid.cpp"
#include "library/tree/segment_tree.cpp"
/**
 * @brief 転倒数系（各 i について左右にある a[i] 未満の要素を数える・探す）
 *
 *   Tentosu::cnt_right_lt(a)   … ret[i] = j > i かつ a[j] < a[i] の個数   O(N log V)
 *   Tentosu::cnt_left_lt(a)    … ret[i] = j < i かつ a[j] < a[i] の個数   O(N log V)
 *   Tentosu::find_right_lt(a)  … ret[i] = a[j] < a[i] となる i より右で最も近い j（無ければ inf）
 *   Tentosu::find_left_lt(a)   … ret[i] = a[j] < a[i] となる i より左で最も近い j（無ければ -inf）
 *
 * @note V = max(a) - min(a)（負の値・重複可）。V が大きいときは座標圧縮してから渡す
 *
 * @code
 * vll a = {3, 1, 2};
 * vll c = Tentosu::cnt_right_lt(a);   // {2, 0, 0}
 * ll inv = accumulate(all(c), 0LL);   // 転倒数 2
 * vll f = Tentosu::find_left_lt(a);   // {-inf, -inf, 1}
 * @endcode
 */
namespace Tentosu {
    ll my_add(ll a, ll b){ return a+b; }
    ll my_max(ll a, ll b){ return max(a, b); }
    ll my_min(ll a, ll b){ return min(a, b); }
    ll e_zero(){ return 0; }
    ll e_minus_inf(){ return -inf; }
    ll e_inf(){ return inf; }
    using AddM = Monoid<ll, my_add, e_zero>;
    using MaxM = Monoid<ll, my_max, e_minus_inf>;
    using MinM = Monoid<ll, my_min, e_inf>;
    vll shift_minus(vll& a){
        ll min_a = *min_element(all(a));
        vll ta = a;
        if(min_a < 0) rep(i, a.size()) ta[i] -= min_a;
        return ta;
    }

    vll cnt_right_lt(vll& a){
        vll ta = shift_minus(a);

        SegmentTree<AddM> st(*max_element(all(ta))+1);
        vll ret(ta.size(), 0);

        mrep(i, (ll)ta.size()-1){
            if(ta[i] > 0) ret[i] = st.get(0, ta[i]);
            st.change(ta[i], st.get(ta[i])+1);
        }

        return ret;
    }
    vll cnt_left_lt(vll& a){
        vll ta = shift_minus(a);

        SegmentTree<AddM> st(*max_element(all(ta))+1);
        vll ret(ta.size(), 0);

        rep(i, a.size()){
            if(ta[i] > 0) ret[i] = st.get(0, ta[i]);
            st.change(ta[i], st.get(ta[i])+1);
        }

        return ret;
    }
    vll find_right_lt(vll& a){
        vll ta = shift_minus(a);

        SegmentTree<MinM> st(*max_element(all(ta))+1);
        vll ret(ta.size(), inf);

        mrep(i, (ll)a.size()-1){
            // 以下にする場合は st.get(0, ta[i]+1) にすればいい
            if(ta[i] > 0) ret[i] = st.get(0, ta[i]);
            st.change(ta[i], i);
        }

        return ret;
    }
    vll find_left_lt(vll& a){
        vll ta = shift_minus(a);

        SegmentTree<MaxM> st(*max_element(all(ta))+1);
        vll ret(ta.size(), -inf);

        rep(i, a.size()){
            // 以下にする場合は st.get(0, ta[i]+1) にすればいい
            if(ta[i] > 0) ret[i] = st.get(0, ta[i]);
            st.change(ta[i], i);
        }

        return ret;
    }
}
