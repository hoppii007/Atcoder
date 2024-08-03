struct UnionFind {
public:
    vll par; // par[i]:iの親の番号　(例) par[3] = 2 : 3の親が2
    vll kosu;
    ll Count; //全体として何個の集合があるか
    
    UnionFind(ll n) : par(n),kosu(n),Count(n) { //最初は全てが根であるとして初期化
        rep(i,n){
            par[i] = i;
            kosu[i] = 1;
        }
    }

    ll root(ll x) { // データxが属する木の根を再帰で得る：root(x) = {xの木の根}
        if (par[x] == x) return x;
        return par[x] = root(par[x]);
    }

    void unite(ll x, ll y) { // xとyの木を併合
        ll rx = root(x); //xの根をrx
        ll ry = root(y); //yの根をry
        if (rx == ry) return; //xとyの根が同じ(=同じ木にある)時はそのまま
        par[rx] = ry; //xとyの根
        kosu[ry] += kosu[rx];
        Count--;
    }

    bool same(ll x, ll y) { // 2つのデータx, yが属する木が同じならtrueを返す
        ll rx = root(x);
        ll ry = root(y);
        return rx == ry;
    }
    
    ll size(ll x){
        return kosu[root(x)];
    }
    ll size(){
        return Count;
    }
};

