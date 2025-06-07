
struct Frac {
    ll ue, sita;
    Frac() :ue(0), sita(1) {}
    Frac(ll n) :ue(n), sita(1) {}
    Frac(ll a, ll b) :ue(a), sita(b) { if (ue != 0) yakubun(); else sita = 1; }
    void yakubun() {
        ll gc = gcd(abs(ue), abs(sita));
        ue /= gc;
        sita /= gc;
        if (sita < 0) {
            ue *= -1;
            sita *= -1;
        }
    }
    Frac operator+(const Frac& a) {
        return Frac(this->ue*a.sita + a.ue*this->sita, this->sita*a.sita);
    }
    Frac operator-(const Frac& a) {
        return Frac(this->ue*a.sita - a.ue*this->sita, this->sita*a.sita);
    }
    Frac operator*(const Frac& a) {
        return Frac(this->ue*a.ue, this->sita*a.sita);
    }
    Frac operator/(const Frac& a) {
        return Frac(this->ue*a.sita, this->sita*a.ue);
    }
    bool operator==(const Frac& a) {
        return (this->ue == a.ue) && (this->sita == a.sita);
    }
    bool operator!=(const Frac& a) {
        return !(*this == a);
    }
    bool operator<(const Frac& a) {
        return (__int128)this->ue*a.sita < (__int128)a.ue*this->sita;
    }
    bool operator<=(const Frac& a) {
        return (__int128)this->ue*a.sita <= (__int128)a.ue*this->sita;
    }
    bool operator>(const Frac& a) {
        return (__int128)this->ue*a.sita > (__int128)a.ue*this->sita;
    }
    bool operator>=(const Frac& a) {
        return (__int128)this->ue*a.sita >= (__int128)a.ue*this->sita;
    }
    const Frac operator=(const Frac& a) {
        this->ue = a.ue;
        this->sita = a.sita;
        return *this;
    }
};
