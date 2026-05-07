/**
 * @brief mod付き整数型（long long ベース）
 * @details 四則演算・累乗・比較を自動で mod を取りながら行う構造体。
 *          割り算（逆元）はフェルマーの小定理を利用するため、mod が素数であることが前提。
 *          mod の変更は modll::mod = 値 で行う。デフォルトは 998244353。
 *
 * @note 使用例:
 *   modll a(10), b(3);
 *   cout << a + b << endl;   // 13
 *   cout << a / b << endl;   // 10 * 3^(mod-2) mod p
 *   modll::mod = 1000000007; // mod を変更
 */
struct modll{
public:
    static ll mod; ///< 法（デフォルト: 998244353）。素数を設定すること。
    ll value;      ///< mod を取った後の値（0 以上 mod 未満）

    /// @brief デフォルトコンストラクタ。値を 0 で初期化。
    modll(){
        this->value = 0;
    }

    /// @brief 値を指定して構築。負の値も正しく [0, mod) に正規化される。
    modll(ll v){
        this->value = v%modll::mod;
        while(this->value < 0) this->value += modll::mod;
    }

    /// @brief 出力ストリームへの書き出し
    friend ostream& operator << (ostream& os, const modll& p){
        os << p.value;
        return os;
    }

    /// @brief ll からの代入
    modll operator =(const ll &a){
        value = a;
        return *this;
    }

    // --- 算術演算（+, -, *, /, ^） ---

    friend modll operator -(const modll &a, const modll &b){ return modll(a.value-b.value); }
    friend modll operator -(const modll &a, const ll &b){ return modll(a.value-(b%modll::mod)); }
    friend modll operator -(const ll &a, const modll &b){ return modll((a%modll::mod)-b.value); }
    friend modll operator +(const modll &a, const modll &b){ return modll(a.value+b.value); }
    friend modll operator +(const modll &a, const ll &b){ return modll(a.value+(b%modll::mod)); }
    friend modll operator +(const ll &a, const modll &b){ return modll((a%modll::mod)+b.value); }
    friend modll operator *(const modll &a, const modll &b){ return modll(a.value*b.value); }
    friend modll operator *(const modll &a, const ll &b){ return modll(a.value*(b%modll::mod)); }
    friend modll operator *(const ll &a, const modll &b){ return modll((a%modll::mod)*b.value); }

    /// @brief 繰り返し二乗法による累乗。a^b mod p を O(log b) で計算。
    friend modll operator ^(const modll &a, const ll &b){
        if(b == 0) return modll(1);
        if(b%2 == 0) return (a*a)^(b/2);
        else return a*(a^(b-1));
    }

    /// @brief 除算。フェルマーの小定理により b^(mod-2) を逆元として計算。mod が素数であること。
    friend modll operator /(const modll &a, const modll &b){ return a*(b^(modll::mod-2)); }
    friend modll operator /(const modll &a, const ll &b){ return a*(modll(b)^(modll::mod-2)); }
    friend modll operator /(const ll &a, const modll &b){ return a*(b^(modll::mod-2)); }

    // --- 比較演算 ---

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

    // --- 複合代入演算 ---

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

    // --- インクリメント・デクリメント ---

    friend void operator ++(modll &a, int){ a += 1; } ///< 後置インクリメント
    friend void operator ++(modll &a){ a += 1; }      ///< 前置インクリメント
    friend void operator --(modll &a, int){ a -= 1; }  ///< 後置デクリメント
    friend void operator --(modll &a){ a -= 1; }       ///< 前置デクリメント
};
ll modll::mod = 998244353LL;
