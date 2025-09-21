struct UnionFind {
public:
    vector<int> par;        // par[i] : iの親の番号　(例) par[3] = 2 : 3の親が2
    vector<int> count_size; //count_size[i] := iを根とした部分技のサイズ
    int count_group;        //全体として何個の集合があるか

    //最初は全てが根として初期化
    UnionFind(int n) : par(n),count_size(n),count_group(n) { 
        for(int i=0;i<n;i++){
            par[i] = i;
            count_size[i] = 1;
        }
    }

    // xが属すグループの根を返す関数
    int root(int x) { 
        if (par[x] == x) return x;
        return par[x] = root(par[x]);
    }
    // xが属すグループと、yが属すグループを合併する関数
    void unite(int x, int y) {
        //x、yの根が同じ（既に同じグループ）ときは何もしない
        int rx = root(x);
        int ry = root(y);
        if (rx == ry) return;

        //yが属すグループを主として、xが属すグループをくっつける
        par[rx] = ry;
        count_size[ry] += count_size[rx];
        count_group--;
    }
    // x, yが同じグループに属すか？を返す関数
    bool is_same(int x, int y) { 
        int rx = root(x);
        int ry = root(y);
        return rx == ry;
    }
    // xが属すグループの要素数を返す関数
    int size(int x){
        return count_size[root(x)];
    }
    // グループの種類数を返す関数
    int size(){
        return count_group;
    }
};

