
/**
 * @brief Union-Find（素集合データ構造）
 * @note 各操作 ほぼ O(1)（経路圧縮あり）
 *
 * メソッド:
 *   root(x)       - xが属すグループの根を返す
 *   unite(x, y)   - xとyのグループを合併
 *   is_same(x, y) - x, yが同じグループか判定
 *   size(x)       - xが属すグループの要素数
 *   size()        - グループの種類数
 *   get_nodes(x)  - xが属すグループの要素配列の参照（vll& a = のように、参照型で受け取る）
 */
struct UnionFind {
public:
	vll par;
	vll count_size;
	ll count_group;
	// その頂点を親としたグループの頂点一覧
	vvll nodes;

	UnionFind(int n) : par(n), count_size(n), count_group(n), nodes(n) {
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
		for(int t: nodes[rl]){
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
