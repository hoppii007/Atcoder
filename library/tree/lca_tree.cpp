#pragma once
#include "library/doubling.cpp"
#include "library/tree/tree_depth.cpp"
#include "library/tree/tree_parents.cpp"

/**
 * @brief LCA（最近共通祖先）
 *
 *   LcaTree lt(root, nexts) … 構築  O(N log N)
 *   lt.lca(u, v)            … u と v の LCA  O(log N)
 *   lt.dist(u, v)           … u-v 間の辺数  O(log N)
 *   lt.back(u, k)           … u から k 回親に遡った頂点（根を越えたら root）  O(log N)
 *   lt.parents[v]           … v の親（根は -1）
 *   lt.distToRoot[v]        … 根からの深さ
 *
 * @note 頂点は 0-indexed。nexts は無向木の隣接リスト（両方向に辺を張る）
 *
 * @code
 * LcaTree lt(0, nexts);
 * ll w = lt.lca(u, v);
 * ll d = lt.dist(u, v);
 * @endcode
 */
struct LcaTree {
public:
    ll root;

    vll parents;

    vll distToRoot;

    vvll backDub;

    LcaTree(ll r, vvll &nexts){
        //parentsとrootをセット
        root = r;
        parents = tree_parents(root, nexts);

        //rootからの距離をセット
        distToRoot = tree_depth(root, nexts);

        //遡る用のdubling配列を用意
        backDub = Doubling(parents).dub;
    }

    ll back(ll u, ll n){
        ll t = 0;
        while((1LL << t) <= n){
            if((1LL << t) & n){
                u = backDub[u][t];
                if(u == -1) return root;
            }
            t++;
        }
        return u;
    }

    ll lca(ll u, ll v){
        //高さを揃える
        if(distToRoot[u] < distToRoot[v]) v = back(v, distToRoot[v]-distToRoot[u]);
        else if(distToRoot[u] > distToRoot[v]) u = back(u, distToRoot[u]-distToRoot[v]);

        if(u == v) return u;

        //今のu,vからx遡ったら同じになる、最小のxを求める
        //dublingっぽくやる(2^i進んでも違うなら進む、をiが大きい順に繰り返す)
        mrep(i,60) if(backDub[u][i] != backDub[v][i]) {
            u = backDub[u][i];
            v = backDub[v][i];
        }
        return parents[u];

        //にぶんたんでやる方法もある
    }

    ll dist(ll u, ll v){
        ll l = this->lca(u,v);
        return distToRoot[u] + distToRoot[v] - 2*distToRoot[l];
    }
    
};
