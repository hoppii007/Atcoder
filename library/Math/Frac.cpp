#include<bits/stdc++.h>
//入力系
#define cinll(...) ll __VA_ARGS__; input(__VA_ARGS__);
#define cinint(...) int __VA_ARGS__; input(__VA_ARGS__);
#define cinstr(...) string __VA_ARGS__; input(__VA_ARGS__);
#define cinchar(...) char __VA_ARGS__; input(__VA_ARGS__);
#define cinvll(a,n) vll a(n); rep(i,n) cin>>a[i];
#define cinvs(a,n) vs a(n); rep(i,n) cin>>a[i];
//繰り返し系
#define rep1(n) for(int i=0;i<n;i++)
#define rep2(i,n) for(int i=0;i<n;i++)
#define rep3(i,a,n) for(int i=a;i<n;i++)
#define rep4(i,a,n,b) for(int i=a;i<n;i+=b)
#define overload4(a,b,c,d,e,...) e
#define rep(...) overload4(__VA_ARGS__,rep4,rep3,rep2,rep1)(__VA_ARGS__)
#define mrep1(n) for(int i=n;i>=0;i--)
#define mrep2(i,n) for(int i=n;i>=0;i--)
#define mrep3(i,n,a) for(int i=n;i>=a;i--)
#define mrep4(i,n,a,b) for(int i=n;i>=a;i-=b)
#define mrep(...) overload4(__VA_ARGS__,mrep4,mrep3,mrep2,mrep1)(__VA_ARGS__)
//iterator系
#define all(a) a.begin(),a.end()
using namespace std;
//型系
using ll = long long;
using vll = vector<long long>;
using vvll = vector<vector<long long>>;
using vi = vector<int>;
using vvi = vector<vector<int>>;
using vb = vector<bool>;
using vvb = vector<vector<bool>>;
using vc = vector<char>;
using vvc = vector<vector<char>>;
using vs = vector<string>;
using pll = pair<long long,long long>;
using vpll = vector<pair<long long,long long>>;

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

