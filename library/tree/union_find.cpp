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
 */
struct UnionFind {
public:
    vector<int> par;
    vector<int> count_size;
    int count_group;

    UnionFind(int n) : par(n), count_size(n), count_group(n) {
        for (int i = 0; i < n; i++) {
            par[i] = i;
            count_size[i] = 1;
        }
    }

    int root(int x) {
        if (par[x] == x) return x;
        return par[x] = root(par[x]);
    }

    void unite(int x, int y) {
        int rx = root(x);
        int ry = root(y);
        if (rx == ry) return;
        par[rx] = ry;
        count_size[ry] += count_size[rx];
        count_group--;
    }

    bool is_same(int x, int y) {
        return root(x) == root(y);
    }

    int size(int x) {
        return count_size[root(x)];
    }

    int size() {
        return count_group;
    }
};

