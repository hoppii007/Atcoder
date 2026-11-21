// 復元: git ea57d10 時点の honban/main.cpp（ノート記録時の版。最終提出と同一かは未確認）
#include "template.hpp"  // 先頭に置くこと（プリコンパイル済みヘッダが効く条件）
#include "library/Math/random.cpp"

ll greedy(ll n, ll m, vll& a){
	//bit全探索
	//要素数n）
	ll ans = -inf;
	rep(state,1LL<<n){
		ll total = 0;
		ll idx = 0;
		rep(bitj,n){
			//1のところ
			if(state&(1LL<<bitj)){
				idx++;
				total += idx*a[bitj];
			}
			//0のところ
			else{
	
			}
		}
		if(idx == m) chmax(ans, total);
	}
	return ans;
}

ll solve(ll n, ll m, vll& a){
	vvll dp(n+1, vll(m+1, -inf));
	rep(i, n) dp[i+1][0] = 0;
	dp[0][0] = 0;
	ll ans = -inf;
	rep(i, n) rep(j, 1, m+1){
		if(i+1 < j) continue;
		dp[i+1][j] = max(dp[i][j], dp[i][j-1]+j*a[i]);
		if(j == m) chmax(ans, dp[i+1][j]);
	}
	return ans;

}



int main(){
	cinll(n, m);
	cinvll(a, n);
	cout << solve(n, m, a) << endl;

	// ll q = 10;
	// while(q--){
	// 	ll n = randll(1, 2);
	// 	ll m = randll(1, n+1);
	// 	vll a(n);
	// 	rep(i, n) a[i] = randll(-20, 21);
	// 	cout << "input" << endl;
	// 	cout << n << " " << m << endl;
	// 	cout << a << endl;
	// 	cout << "output" << endl;
	// 	cout << greedy(n, m, a) << " " << solve(n, m, a) << endl;
	// 	cout << endl;
	// }

	

	

	return 0;
}