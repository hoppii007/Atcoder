#pragma once
#include "library/Math/gcds.cpp"
/**
 * @brief 整数係数の直線 ax + by + c = 0（正規化済みで set / map のキーにできる）
 *
 *   L(a, b, c)                     … 直線 ax + by + c = 0（gcd で割り、最初の非零係数を正に揃える）
 *   l1 == l2, l1 != l2, l1 < l2    … 直線としての一致判定 / 順序
 *   make_line(p1, p2)              … 2 点 p1, p2 を通る直線
 *   perpendicular_bisector(p1, p2) … p1, p2 の垂直二等分線
 *   perpendicular_line(p, l)       … 点 p を通り l に垂直な直線
 *   is_parallel(l1, l2)            … 平行か（一致する 2 直線も true）
 *   is_perpendicular(l1, l2)       … 垂直か
 *
 * @note a, b の少なくとも一方は非零。p1 != p2 であること
 * @note perpendicular_bisector は座標の 2 乗を使うのでオーバーフロー注意
 *
 * @code
 * set<L> lines;
 * lines.insert(make_line({0, 0}, {2, 2}));
 * lines.insert(make_line({1, 1}, {3, 3}));  // 同じ直線なので増えない
 * bool p = is_parallel(make_line({0, 0}, {1, 0}), L(0, 1, -5));  // true
 * @endcode
 */
struct L {
    ll a, b, c;
    L(){}
    L(ll ta, ll tb, ll tc): a(ta), b(tb), c(tc) {
        ll g = gcds({abs(a),abs(b),abs(c)});
        a /= g;
        b /= g;
        c /= g;

        // 最初に出てくる 0 じゃないやつが - なら -1 をかけて符号を統一
        for(ll t: {a,b,c}) if(t != 0){
            if(t < 0){
                a *= -1;
                b *= -1;
                c *= -1;
            }
            break;
        }
    }
    bool operator<(const L& l) const {
        return tie(a,b,c) < tie(l.a, l. b, l.c);
    }
    bool operator==(const L& l) const {
        return tie(a,b,c) == tie(l.a, l.b, l.c);
    }
    bool operator!=(const L& l) const {
        return !(*this == l);
    }
};

L make_line(pll p1, pll p2){
    ll dx = p2.fst - p1.fst;
    ll dy = p2.snd - p1.snd;
    return L(dy, -dx, p1.snd*dx - p1.fst*dy);
}

L perpendicular_bisector(pll p1, pll p2){
    ll a = 2 * (p2.fst - p1.fst);
    ll b = 2 * (p2.snd - p1.snd);
    ll c = p1.fst*p1.fst + p1.snd*p1.snd - p2.fst*p2.fst - p2.snd*p2.snd;
    return L(a, b, c);
}

bool is_parallel(const L& l1, const L& l2){
    return l1.a * l2.b - l2.a * l1.b == 0;
}

bool is_perpendicular(const L& l1, const L& l2){
    return l1.a * l2.a + l1.b * l2.b == 0;
}

L perpendicular_line(pll p, const L& l){
    return L(-l.b, l.a, l.b*p.fst - l.a*p.snd);
}