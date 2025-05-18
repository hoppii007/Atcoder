
//割り算とかするならmodは素数前提
//modを変えたい時はmll::mod = 10みたいな感じ
struct mll{
public:
    static ll mod;
    ll value;
    mll(){
        this->value = 0;
    }
    mll(ll v){
        this->value = v%mll::mod;
        while(this->value < 0) this->value += mll::mod;
    }
    friend ostream& operator << (ostream& os, const mll& p){
        os << p.value;
        return os;
    }
    mll operator =(const ll &a){
        value = a;
        return *this;
    }
    friend mll operator -(const mll &a, const mll &b){ return mll(a.value-b.value); }
    friend mll operator -(const mll &a, const ll &b){ return mll(a.value-(b%mll::mod)); }
    friend mll operator -(const ll &a, const mll &b){ return mll((a%mll::mod)-b.value); }
    friend mll operator +(const mll &a, const mll &b){ return mll(a.value+b.value); }
    friend mll operator +(const mll &a, const ll &b){ return mll(a.value+(b%mll::mod)); }
    friend mll operator +(const ll &a, const mll &b){ return mll((a%mll::mod)+b.value); }
    friend mll operator *(const mll &a, const mll &b){ return mll(a.value*b.value); }
    friend mll operator *(const mll &a, const ll &b){ return mll(a.value*(b%mll::mod)); }
    friend mll operator *(const ll &a, const mll &b){ return mll((a%mll::mod)*b.value); }
    friend mll operator ^(const mll &a, const ll &b){
        if(b == 0) return mll(1);
        if(b%2 == 0) return (a*a)^(b/2);
        else return a*(a^(b-1));
    }
    friend mll operator /(const mll &a, const mll &b){ return a*(b^(mll::mod-2)); }
    friend mll operator /(const mll &a, const ll &b){ return a*(mll(b)^(mll::mod-2)); }
    friend mll operator /(const ll &a, const mll &b){ return a*(b^(mll::mod-2)); }
    friend bool operator <(const mll &a, const mll &b){ return a.value < b.value; }
    friend bool operator <(const mll &a, const ll &b){ return a.value < b; }
    friend bool operator <(const ll &a, const mll &b){ return a < b.value; }
    friend bool operator <=(const mll &a, const mll &b){ return a.value <= b.value; }
    friend bool operator <=(const mll &a, const ll &b){ return a.value <= b; }
    friend bool operator <=(const ll &a, const mll &b){ return a <= b.value; }
    friend bool operator >(const mll &a, const mll &b){ return a.value > b.value; }
    friend bool operator >(const mll &a, const ll &b){ return a.value > b; }
    friend bool operator >(const ll &a, const mll &b){ return a > b.value; }
    friend bool operator >=(const mll &a, const mll &b){ return a.value >= b.value; }
    friend bool operator >=(const mll &a, const ll &b){ return a.value >= b; }
    friend bool operator >=(const ll &a, const mll &b){ return a >= b.value; }
    friend bool operator ==(const mll &a, const mll &b){ return a.value == b.value; }
    friend bool operator ==(const mll &a, const ll &b){ return a.value == b; }
    friend bool operator ==(const ll &a, const mll &b){ return a == b.value; }
    friend bool operator !=(const mll &a, const mll &b){ return a.value != b.value; }
    friend bool operator !=(const mll &a, const ll &b){ return a.value != b; }
    friend bool operator !=(const ll &a, const mll &b){ return a != b.value; }
    friend void operator +=(mll &a, const mll &b){ a = a+b; }
    friend void operator +=(mll &a, const ll &b){ a = a+b; }
    friend void operator +=(ll &a, const mll &b){ a = (a+b).value; }
    friend void operator -=(mll &a, const mll &b){ a = a-b; }
    friend void operator -=(mll &a, const ll &b){ a = a-b; }
    friend void operator -=(ll &a, const mll &b){ a = (a-b).value; }
    friend void operator *=(mll &a, const mll &b){ a = a*b; }
    friend void operator *=(mll &a, const ll &b){ a = a*b; }
    friend void operator *=(ll &a, const mll &b){ a = (a*b).value; }
    friend void operator /=(mll &a, const mll &b){ a = a/b; }
    friend void operator /=(mll &a, const ll &b){ a = a/b; }
    friend void operator /=(ll &a, const mll &b){ a = (a/b).value; }
    friend void operator ++(mll &a, int){ a += 1; }
    friend void operator ++(mll &a){ a += 1; }
    friend void operator --(mll &a, int){ a -= 1; }
    friend void operator --(mll &a){ a -= 1; }
};
ll mll::mod = 998244353LL;
