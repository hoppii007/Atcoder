#pragma once
#include "library/doubling.cpp"
#include "library/tree/tree_depth.cpp"
#include "library/tree/tree_parents.cpp"

//各頂点は0 ~ N-1の番号で管理
//共通祖先をO(Log V)で出すやつ
//前計算はO(V Log V)
//nexts : 有向グラフにしておく（親にも子にも行けるから実質無向グラフ）
struct LcaTree {
public:
    //親
    ll root;

    //parents[i] : iの親を示す（親は必ず一人だからvllでいい）
    //親の親は自分自身にする
    vll parents;

    //根からの距離を入れる
    vll distToRoot;

    //iから2^j回遡ったところ
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

    //頂点uからn回遡りたい
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

    //2点間の距離
    ll dist(ll u, ll v){
        ll l = this->lca(u,v);
        return distToRoot[u] + distToRoot[v] - 2*distToRoot[l];
    }
    
};
