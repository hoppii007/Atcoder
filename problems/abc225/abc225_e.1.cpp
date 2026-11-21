// 復元: git f47536d 時点の honban/main.cpp（ノート記録時の版。最終提出と同一かは未確認）
#include "template.hpp"  // 先頭に置くこと（プリコンパイル済みヘッダが効く条件）
#include "library/Math/arg_comp.cpp"
#include "library/Math/coordinate_compression.cpp"
#include "library/interval_scheduling.cpp"

int main(){
	cinll(n);
	cinvpll(p, n);

	vpll ps;
	rep(i, n) {
		ps.pb({p[i].fst-1, p[i].snd});
		ps.pb({p[i].fst, p[i].snd-1});
	}
	sort(all(ps), arg_comp);

	// 偏角ソート後のインデックスを保存
	auto idx = coordinate_compression(ps, arg_comp);

	vpll t;
	rep(i, n){
		t.pb({idx[{p[i].fst, p[i].snd-1}], idx[{p[i].fst-1, p[i].snd}]});
	}

	cout << interval_scheduling(t).size() << endl;

	
	return 0;
}