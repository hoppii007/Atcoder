#include "template.hpp"  // 先頭に置くこと（プリコンパイル済みヘッダが効く条件）
#include "library/Math/monoid.cpp"
#include "library/tree/segment_tree.cpp"

ll op_sum(ll a, ll b){ return a+b; }
ll e_sum(){ return 0LL; }
using S = Monoid<ll, op_sum, e_sum>;

int main(){
	cinll(n, q);
	cinvll(a, n);
	SegmentTree<S> st(a);
	
	while(q--){
		cinll(t, l, r);
		if(t == 0) st.change(l, st.get(l)+r);
		else cout << st.get(l, r) << endl;
	}
	return 0;
}
