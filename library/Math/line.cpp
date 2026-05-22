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