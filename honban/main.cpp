#include "template.hpp"  // 先頭に置くこと（プリコンパイル済みヘッダが効く条件）
#include "library/Math/monoid.cpp"
#include "library/tree/segment_tree.cpp"

ll op_min(ll a, ll b){ return min(a, b); }
ll e_min(){ return inf; }
using min_monoid = monoid<ll, op_min, e_min>;

ll op_max(ll a, ll b){ return max(a, b); }
ll e_max(){ return -inf; }
using max_monoid = monoid<ll, op_max, e_max>;

ll op_sum(ll a, ll b){ return a + b; }
ll e_sum(){ return 0; }
using sum_monoid = monoid<ll, op_sum, e_sum>;

int main(){
	cinll(n, q);
	cinvll(a, n);
	segtree<min_monoid> min_st(n);
	segtree<max_monoid> max_st(n);
	segtree<sum_monoid> sum_st(n);
	rep(i, n){
		if(a[i] != -1) {
			min_st.change(i, a[i]);
			max_st.change(i, a[i]);
		}
		else{
			sum_st.change(i, 1);
		}
	}
	while(q--){
		cinll(t, l, r);
		l--;
		if(t == 1){
			if(r == -1){
				min_st.change(l, inf);
				max_st.change(l, -inf);
				sum_st.change(l, 1);
			}
			else{
				min_st.change(l, r);
				max_st.change(l, r);
				sum_st.change(l, 0);
			}
		}
		else if(t == 2){
			if(sum_st.get(l, r) == r-l){
				cout << "Yes" << endl;
				continue;
			}
			if(min_st.get(l, r) == inf) continue;
			if(min_st.get(l,r) == max_st.get(l,r)) cout << "Yes" << endl;
			else cout << "No" << endl;
		}
	}

	return 0;
}
