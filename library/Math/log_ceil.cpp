//O(log n)で、a^b >= nとなる最小のbを求める（logの切り上げ的な）
ll log_ceil(ll n, ll a){
	ll ret = 0;
	ll now = 1;
	while(now < n){
		now *= a;
		ret++;
	}
	return ret;
}