#pragma once
#include "library/Math/monoid.cpp"
#include "library/tree/segment_tree.cpp"

// a[i]より右の中で、a[i]より小さい数を数える
// a は重複があってもOK
// a は-があってもOKだけど、max{a} - min{a}が10^5とかじゃないと無理
namespace Tentosu {
    ll my_add(ll a, ll b){ return a+b; }
    ll my_max(ll a, ll b){ return max(a, b); }
    ll my_min(ll a, ll b){ return min(a, b); }
    using AddM = Monoid<ll, my_add, 0>;
    using MaxM = Monoid<ll, my_max, -inf>;
    using MinM = Monoid<ll, my_min, inf>;
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
            if(ta[i]-1 >= 0) ret[i] = st.get(0, ta[i]-1);
            st.change(ta[i], st.get(ta[i], ta[i])+1);
        }

        return ret;
    }
    vll cnt_left_lt(vll& a){
        vll ta = shift_minus(a);

        SegmentTree<AddM> st(*max_element(all(ta))+1);
        vll ret(ta.size(), 0);

        rep(i, a.size()){
            if(ta[i]-1 >= 0) ret[i] = st.get(0, ta[i]-1);
            st.change(ta[i], st.get(ta[i], ta[i])+1);
        }

        return ret;
    }
    vll find_right_lt(vll& a){
        vll ta = shift_minus(a);

        SegmentTree<MinM> st(*max_element(all(ta))+1);
        vll ret(ta.size(), inf);

        mrep(i, (ll)a.size()-1){
            // 以下にする場合は ta[i]-1 を ta[i] にすればいい
            if(ta[i]-1 >= 0) ret[i] = st.get(0, ta[i]-1);
            st.change(ta[i], i);
        }

        return ret;
    }
    vll find_left_lt(vll& a){
        vll ta = shift_minus(a);

        SegmentTree<MaxM> st(*max_element(all(ta))+1);
        vll ret(ta.size(), -inf);

        rep(i, a.size()){
            // 以下にする場合は ta[i]-1 を ta[i] にすればいい
            if(ta[i]-1 >= 0) ret[i] = st.get(0, ta[i]-1);
            st.change(ta[i], i);
        }

        return ret;
    }
}
