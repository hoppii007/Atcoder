#pragma once
/**
 * @brief Union-Find（素集合データ構造）
 *
 *   union_find uf(n)  … 要素 0..n-1 で初期化
 *   uf.root(x)       … x のグループの根  ほぼ O(1)
 *   uf.unite(x, y)   … x と y のグループを併合  ならし O(log N)
 *   uf.is_same(x, y) … x と y が同じグループか  ほぼ O(1)
 *   uf.size(x)       … x のグループの要素数  ほぼ O(1)
 *   uf.size()        … グループ数  O(1)
 *   uf.get_nodes(x)  … x のグループの要素一覧（const vll& で受ける）
 *
 * @note get_nodes で受けた参照の中身は以降の unite で変わりうる
 *
 * @code
 * union_find uf(n);
 * uf.unite(0, 1);
 * bool same = uf.is_same(0, 1);  // true
 * const vll& g = uf.get_nodes(0);
 * @endcode
 */
struct union_find {
public:
	vll par;
	vll count_size;
	ll count_group;
	vvll nodes;

	union_find(int n) : par(n), count_size(n), count_group(n), nodes(n) {
		for (int i = 0; i < n; i++) {
			par[i] = i;
			count_size[i] = 1;
			nodes[i].pb(i);
		}
	}

	ll root(ll x) {
		if (par[x] == x) return x;
		return par[x] = root(par[x]);
	}

	void unite(ll x, ll y) {
		ll rx = root(x);
		ll ry = root(y);
		if (rx == ry) return;

		// マージてく
		// まずは x が小さい、 y が大きいとしておく
		ll l = x;
		ll g = y;
		ll rl = rx;
		ll rg = ry;
		if(count_size[rg] < count_size[rl]){
			swap(l, g);
			swap(rl, rg);
		}

		// rl を rg に結合
		par[rl] = rg;
		count_size[rg] += count_size[rl];
		for(ll t: nodes[rl]){
			nodes[rg].pb(t);
		}
		nodes[rl].clear();

		// グループをひとつ減らす
		count_group--;
	}

	bool is_same(ll x, ll y) {
		return root(x) == root(y);
	}

	ll size(ll x) {
		return count_size[root(x)];
	}

	ll size() {
		return count_group;
	}

	const vll& get_nodes(ll x) {
		return nodes[root(x)];
	}
};
