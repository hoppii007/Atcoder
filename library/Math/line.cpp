/**
 * @brief 直線 ax + by + c = 0 を表す構造体
 * @note 同じ直線が同じ (a, b, c) で表現されるよう、以下の正規化を行う:
 *       1. gcd(|a|, |b|, |c|) で割って既約にする
 *       2. a, b, c のうち最初に現れる非零の値が正になるよう符号を揃える
 *       これにより operator< で直線の集合管理（map/set のキー）が可能。
 */
struct L {
    ll a, b, c;
    L(){}
    /**
     * @brief 係数 (ta, tb, tc) から直線を構築する（正規化込み）
     * @param ta x の係数
     * @param tb y の係数
     * @param tc 定数項
     * @note a と b の少なくとも一方が非零であることを前提とする
     */
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
    /**
     * @brief 2 直線が一致するか
     * @note 正規化済みのため、(a, b, c) の一致と直線としての一致が同値になる
     */
    bool operator==(const L& l) const {
        return tie(a,b,c) == tie(l.a, l.b, l.c);
    }
    bool operator!=(const L& l) const {
        return !(*this == l);
    }
};

/**
 * @brief 点 p1 と点 p2 を通る直線を返す
 * @param p1 通る点その1
 * @param p2 通る点その2（p1 と異なること）
 * @return p1, p2 を通る直線 L
 * @note 方向ベクトル (dx, dy) に対し法線ベクトル (dy, -dx) を係数として採用。
 *       定数項は p1 が直線上にある条件から決定。
 */
L make_line(pll p1, pll p2){
    ll dx = p2.fst - p1.fst;
    ll dy = p2.snd - p1.snd;
    return L(dy, -dx, p1.snd*dx - p1.fst*dy);
}

/**
 * @brief 点 p1 と点 p2 の垂直二等分線を返す
 * @param p1 端点その1
 * @param p2 端点その2（p1 と異なること）
 * @return p1, p2 から等距離にある点の集合をなす直線 L
 * @note |P-p1|^2 = |P-p2|^2 を展開して得られる
 *       2(x2-x1)x + 2(y2-y1)y + (x1^2+y1^2-x2^2-y2^2) = 0 を用いる。
 *       係数に座標の 2 乗が現れるため、座標が大きいとオーバーフローに注意。
 */
L perpendicular_bisector(pll p1, pll p2){
    ll a = 2 * (p2.fst - p1.fst);
    ll b = 2 * (p2.snd - p1.snd);
    ll c = p1.fst*p1.fst + p1.snd*p1.snd - p2.fst*p2.fst - p2.snd*p2.snd;
    return L(a, b, c);
}

/**
 * @brief 2 直線が平行かを判定する
 * @param l1 直線その1
 * @param l2 直線その2
 * @return 平行なら true
 * @note 法線ベクトル (a, b) 同士の外積が 0 かで判定する。
 *       一致する 2 直線も平行として true を返すため、
 *       「平行だが異なる」を見たいときは l1 != l2 と併用する。
 */
bool is_parallel(const L& l1, const L& l2){
    return l1.a * l2.b - l2.a * l1.b == 0;
}

/**
 * @brief 2 直線が垂直かを判定する
 * @param l1 直線その1
 * @param l2 直線その2
 * @return 垂直なら true
 * @note 法線ベクトル (a, b) 同士の内積が 0 かで判定する
 */
bool is_perpendicular(const L& l1, const L& l2){
    return l1.a * l2.a + l1.b * l2.b == 0;
}

/**
 * @brief 点 p を通り、直線 l に垂直な直線を返す
 * @param p 通る点
 * @param l 基準となる直線
 * @return p を通り l に垂直な直線 L
 * @note l の法線 (a, b) を法線としない向き、すなわち法線 (-b, a) を採用する。
 *       定数項は p が直線上にある条件から決定。
 */
L perpendicular_line(pll p, const L& l){
    return L(-l.b, l.a, l.b*p.fst - l.a*p.snd);
}