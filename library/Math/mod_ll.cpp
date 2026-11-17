#pragma once
/**
 * @brief mod 付き整数（演算ごとに自動で mod を取る）
 *
 *   modll a(v), a = v              … 値 v で構築（負も [0, mod) に直す）
 *   a.value                        … 値（0 以上 mod 未満）
 *   a + b, a - b, a * b, a / b     … mod 上の四則演算（片方は ll でも可。/ は mod 素数前提）
 *   a += b, a -= b, a *= b, a /= b … 複合代入（左辺が ll でも可）
 *   a ^ k                          … 累乗 a^k（k < 0 なら逆元の累乗）  O(log k)
 *   ++a, a++, --a, a--             … インクリメント / デクリメント
 *   ==, !=, <, <=, >, >=           … value 同士の比較
 *   cout << a                      … value を出力
 *   modll::mod = p                 … 法を変更（デフォルト 998244353）
 *
 * @note / と負の指数は逆元を b^(mod-2) で求める  O(log mod)
 *
 * @code
 * modll a(10), b(3);
 * modll c = a / b + 5;  // 10 * 3^{-1} + 5
 * c *= a ^ 100;
 * cout << c << endl;
 * modll::mod = 1000000007;  // 法を変える
 * @endcode
 */
struct modll{
public:
    static ll mod;
    ll value;

    modll(){
        this->value = 0;
    }

    modll(ll v){
        this->value = v%modll::mod;
        while(this->value < 0) this->value += modll::mod;
    }

    friend ostream& operator << (ostream& os, const modll& p){
        os << p.value;
        return os;
    }

    modll operator =(const ll &a){
        value = a % modll::mod;
        if(value < 0) value += modll::mod;
        return *this;
    }

    friend modll operator -(const modll &a, const modll &b){ return modll(a.value-b.value); }
    friend modll operator -(const modll &a, const ll &b){ return modll(a.value-(b%modll::mod)); }
    friend modll operator -(const ll &a, const modll &b){ return modll((a%modll::mod)-b.value); }
    friend modll operator +(const modll &a, const modll &b){ return modll(a.value+b.value); }
    friend modll operator +(const modll &a, const ll &b){ return modll(a.value+(b%modll::mod)); }
    friend modll operator +(const ll &a, const modll &b){ return modll((a%modll::mod)+b.value); }
    friend modll operator *(const modll &a, const modll &b){ return modll(a.value*b.value); }
    friend modll operator *(const modll &a, const ll &b){ return modll(a.value*(b%modll::mod)); }
    friend modll operator *(const ll &a, const modll &b){ return modll((a%modll::mod)*b.value); }

    friend modll operator ^(const modll &a, const ll &b){
        if(b < 0) return (1LL / a) ^ (-b);
        if(b == 0) return modll(1);
        if(b%2 == 0) return (a*a)^(b/2);
        else return a*(a^(b-1));
    }

    friend modll operator /(const modll &a, const modll &b){ return a*(b^(modll::mod-2)); }
    friend modll operator /(const modll &a, const ll &b){ return a*(modll(b)^(modll::mod-2)); }
    friend modll operator /(const ll &a, const modll &b){ return a*(b^(modll::mod-2)); }

    friend bool operator <(const modll &a, const modll &b){ return a.value < b.value; }
    friend bool operator <(const modll &a, const ll &b){ return a.value < b; }
    friend bool operator <(const ll &a, const modll &b){ return a < b.value; }
    friend bool operator <=(const modll &a, const modll &b){ return a.value <= b.value; }
    friend bool operator <=(const modll &a, const ll &b){ return a.value <= b; }
    friend bool operator <=(const ll &a, const modll &b){ return a <= b.value; }
    friend bool operator >(const modll &a, const modll &b){ return a.value > b.value; }
    friend bool operator >(const modll &a, const ll &b){ return a.value > b; }
    friend bool operator >(const ll &a, const modll &b){ return a > b.value; }
    friend bool operator >=(const modll &a, const modll &b){ return a.value >= b.value; }
    friend bool operator >=(const modll &a, const ll &b){ return a.value >= b; }
    friend bool operator >=(const ll &a, const modll &b){ return a >= b.value; }
    friend bool operator ==(const modll &a, const modll &b){ return a.value == b.value; }
    friend bool operator ==(const modll &a, const ll &b){ return a.value == b; }
    friend bool operator ==(const ll &a, const modll &b){ return a == b.value; }
    friend bool operator !=(const modll &a, const modll &b){ return a.value != b.value; }
    friend bool operator !=(const modll &a, const ll &b){ return a.value != b; }
    friend bool operator !=(const ll &a, const modll &b){ return a != b.value; }

    friend void operator +=(modll &a, const modll &b){ a = a+b; }
    friend void operator +=(modll &a, const ll &b){ a = a+b; }
    friend void operator +=(ll &a, const modll &b){ a = (a+b).value; }
    friend void operator -=(modll &a, const modll &b){ a = a-b; }
    friend void operator -=(modll &a, const ll &b){ a = a-b; }
    friend void operator -=(ll &a, const modll &b){ a = (a-b).value; }
    friend void operator *=(modll &a, const modll &b){ a = a*b; }
    friend void operator *=(modll &a, const ll &b){ a = a*b; }
    friend void operator *=(ll &a, const modll &b){ a = (a*b).value; }
    friend void operator /=(modll &a, const modll &b){ a = a/b; }
    friend void operator /=(modll &a, const ll &b){ a = a/b; }
    friend void operator /=(ll &a, const modll &b){ a = (a/b).value; }

    friend void operator ++(modll &a, int){ a += 1; }
    friend void operator ++(modll &a){ a += 1; }
    friend void operator --(modll &a, int){ a -= 1; }
    friend void operator --(modll &a){ a -= 1; }
};
ll modll::mod = 998244353LL;
