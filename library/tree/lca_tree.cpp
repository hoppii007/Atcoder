#pragma once
#include "library/doubling.cpp"
#include "library/tree/tree_depth.cpp"
#include "library/tree/tree_parents.cpp"

/**
 * @brief LCA（最近共通祖先）
 *
 *   lca_tree lt(root, nexts) … 構築  O(N log N)
 *   lt.lca(u, v)            … u と v の LCA  O(log N)
 *   lt.dist(u, v)           … u-v 間の辺数  O(log N)
 *   lt.back(u, k)           … u から k 回親に遡った頂点（根を越えたら root）  O(log N)
 *   lt.parents[v]           … v の親（根は -1）
 *   lt.dist_to_root[v]        … 根からの深さ
 *
 * @note 頂点は 0-indexed。nexts は無向木の隣接リスト（両方向に辺を張る）
 *
 * @code
 * lca_tree lt(0, nexts);
 * ll w = lt.lca(u, v);
 * ll d = lt.dist(u, v);
 * @endcode
 */
struct lca_tree {
public:
    ll root;

    vll parents;

    vll dist_to_root;

    vvll back_dub;

    lca_tree(ll r, vvll &nexts){
        //parentsとrootをセット
        root = r;
        parents = tree_parents(root, nexts);

        //rootからの距離をセット
        dist_to_root = tree_depth(root, nexts);

        //遡る用のdubling配列を用意
        back_dub = doubling(parents).dub;
    }

    ll back(ll u, ll n){
        ll t = 0;
        while((1LL << t) <= n){
            if((1LL << t) & n){
                u = back_dub[u][t];
                if(u == -1) return root;
            }
            t++;
        }
        return u;
    }

    ll lca(ll u, ll v){
        //高さを揃える
        if(dist_to_root[u] < dist_to_root[v]) v = back(v, dist_to_root[v]-dist_to_root[u]);
        else if(dist_to_root[u] > dist_to_root[v]) u = back(u, dist_to_root[u]-dist_to_root[v]);

        if(u == v) return u;

        //今のu,vからx遡ったら同じになる、最小のxを求める
        //dublingっぽくやる(2^i進んでも違うなら進む、をiが大きい順に繰り返す)
        mrep(i,60) if(back_dub[u][i] != back_dub[v][i]) {
            u = back_dub[u][i];
            v = back_dub[v][i];
        }
        return parents[u];

        //にぶんたんでやる方法もある
    }

    ll dist(ll u, ll v){
        ll l = this->lca(u,v);
        return dist_to_root[u] + dist_to_root[v] - 2*dist_to_root[l];
    }
    
};
