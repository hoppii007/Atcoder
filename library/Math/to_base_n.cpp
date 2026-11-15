#pragma once
//xをn進数に変換
//右から読む感じ　6を2進数表記で　0 1 1
vll toNsinsu(ll x, ll n) {
	ll cnt = 0;
	ll xx = x;
	while (xx > 0) {
		xx /= n;
		cnt++;
	}
	vll ret(cnt, 0);
	rep(i, cnt) {
		ret[i] = x%n;
		x /= n;	}
	return ret;
}