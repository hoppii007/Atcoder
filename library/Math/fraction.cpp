#pragma once
/**
 * @brief 分数（常に約分済み・分母 > 0 で保持）
 *
 *   fraction()  / fraction(n) / fraction(a, b) … 0 / n / a/b を作る
 *   x.ue, x.sita                   … 分子, 分母
 *   x + y, x - y, x * y, x / y     … 四則演算（結果も約分される）
 *   ==, !=, <, <=, >, >=           … 比較（__int128 で交差乗算）
 *
 * @note 0 は 0/1、+∞ は 1/0、-∞ は -1/0（fraction(a, 0) は a の符号で決まる）。∞ を含む四則演算は未考慮
 * @note 四則演算の途中の積は ll なのでオーバーフローに注意
 *
 * @code
 * fraction x(1, 3), y(1, 6);
 * fraction z = x + y;          // 1/2
 * bool lt = y < x;         // true
 * cout << z.ue << "/" << z.sita << endl;
 * @endcode
 */
struct fraction {
    ll ue, sita;
    fraction() :ue(0), sita(1) {}
    fraction(ll n) :ue(n), sita(1) {}
    fraction(ll a, ll b) :ue(a), sita(b) { if(sita==0){ ue=(ue<0 ? -1 : 1); return; } if (ue != 0) yakubun(); else sita = 1; }
    void yakubun() {
        ll gc = gcd(abs(ue), abs(sita));
        ue /= gc;
        sita /= gc;
        if (sita < 0) {
            ue *= -1;
            sita *= -1;
        }
    }
    fraction operator+(const fraction& a) const {
        return fraction(this->ue*a.sita + a.ue*this->sita, this->sita*a.sita);
    }
    fraction operator-(const fraction& a) const {
        return fraction(this->ue*a.sita - a.ue*this->sita, this->sita*a.sita);
    }
    fraction operator*(const fraction& a) const {
        return fraction(this->ue*a.ue, this->sita*a.sita);
    }
    fraction operator/(const fraction& a) const {
        return fraction(this->ue*a.sita, this->sita*a.ue);
    }
    bool operator==(const fraction& a) const {
        return (this->ue == a.ue) && (this->sita == a.sita);
    }
    bool operator!=(const fraction& a) const {
        return !(*this == a);
    }
    bool operator<(const fraction& a) const {
        // ±∞ 同士は交差乗算が 0 == 0 になるので符号で比べる
        if(this->sita == 0 && a.sita == 0) return this->ue < a.ue;
        return (__int128)this->ue*a.sita < (__int128)a.ue*this->sita;
    }
    bool operator<=(const fraction& a) const {
        return !(a < *this);
    }
    bool operator>(const fraction& a) const {
        return a < *this;
    }
    bool operator>=(const fraction& a) const {
        return !(*this < a);
    }
    const fraction operator=(const fraction& a) {
        this->ue = a.ue;
        this->sita = a.sita;
        return *this;
    }
};
