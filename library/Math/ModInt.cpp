//基本的にはmoが素数前提
class Modll{
public:
	ll value;
	Modll(){
		this->value = 0;
	}
	Modll(ll v){
		this->value = v%mod;
		while(this->value < 0) this->value += mod;
	}
	friend ostream& operator << (ostream& os, const Modll& p){
		os << p.value;
		return os;
	}
	Modll operator =(const ll &a){
		value = a;
		return *this;
	}
	friend Modll operator -(const Modll &a, const Modll &b){ return Modll(a.value-b.value); }
	friend Modll operator -(const Modll &a, const ll &b){ return Modll(a.value-(b%mod)); }
	friend Modll operator -(const ll &a, const Modll &b){ return Modll((a%mod)-b.value); }
	friend Modll operator +(const Modll &a, const Modll &b){ return Modll(a.value+b.value); }
	friend Modll operator +(const Modll &a, const ll &b){ return Modll(a.value+(b%mod)); }
	friend Modll operator +(const ll &a, const Modll &b){ return Modll((a%mod)+b.value); }
	friend Modll operator *(const Modll &a, const Modll &b){ return Modll(a.value*b.value); }
	friend Modll operator *(const Modll &a, const ll &b){ return Modll(a.value*(b%mod)); }
	friend Modll operator *(const ll &a, const Modll &b){ return Modll((a%mod)*b.value); }
	friend Modll operator ^(const Modll &a, const ll &b){
		if(b == 0) return Modll(1);
		if(b%2 == 0) return (a*a)^(b/2);
		else return a*(a^(b-1));
	}
	friend Modll operator /(const Modll &a, const Modll &b){ return a*(b^(mod-2)); }
	friend Modll operator /(const Modll &a, const ll &b){ return a*(Modll(b)^(mod-2)); }
	friend Modll operator /(const ll &a, const Modll &b){ return a*(b^(mod-2)); }
	friend void operator +=(Modll &a, const Modll &b){ a = a+b; }
	friend void operator +=(Modll &a, const ll &b){ a = a+b; }
	friend void operator +=(ll &a, const Modll &b){ a = (a+b).value; }
	friend void operator -=(Modll &a, const Modll &b){ a = a-b; }
	friend void operator -=(Modll &a, const ll &b){ a = a-b; }
	friend void operator -=(ll &a, const Modll &b){ a = (a-b).value; }
	friend void operator *=(Modll &a, const Modll &b){ a = a*b; }
	friend void operator *=(Modll &a, const ll &b){ a = a*b; }
	friend void operator *=(ll &a, const Modll &b){ a = (a*b).value; }
	friend void operator /=(Modll &a, const Modll &b){ a = a/b; }
	friend void operator /=(Modll &a, const ll &b){ a = a/b; }
	friend void operator /=(ll &a, const Modll &b){ a = (a/b).value; }
};

