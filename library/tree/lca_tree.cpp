
//一回の行き先が決まってる状態から、K回先を計算する O(N log K)
//要素数:n
//各状態(0 ~ n)から、2^60回までの遷移先を求める(10^18ちょいくらい)
// dub[i][j] := iから2^j回遷移した先
//一回の行き先をnextsに入れとく
//行き先がないやつは-1
vvll dubling(vll &nexts){
    vvll dub(nexts.size(), vll(61));
    rep(i,nexts.size()) dub[i][0] = nexts[i];
    rep(j,1,61) rep(i,nexts.size()){
        if(dub[i][j-1] == -1) dub[i][j] = -1;
        else dub[i][j] = dub[dub[i][j-1]][j-1];
    }
    return dub;
}

//木の中で、rootからの距離を入れる
//O(N)
vll treeDepth(ll root, vvll &nexts){
    vll ret(nexts.size(), inf);
    std::function<void(ll,ll)> subDfs = [&](ll now, ll d){
        ret[now] = d;
        for(ll next: nexts[now]) if(ret[next] == inf) subDfs(next, d+1);
    };
    subDfs(root, 0);
    return ret;
}

//木の中で、rootを指定して、各頂点のparentsを設定する
//親の親は自分自身にしておく
//O(N)
vll treeParents(ll root, vvll &nexts){
    vll ret(nexts.size(), inf);
    std::function<void(ll,ll)> subDfs = [&](ll now, ll parent){
        ret[now] = parent;
        for(ll next: nexts[now]) if(ret[next] == inf) subDfs(next, now);
    };
    //親の親を-1にしたりしたいときはここの第二引数を変更
    //infにするときは注意
    subDfs(root, -1);
    return ret;
}

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
        parents = treeParents(root, nexts);

        //rootからの距離をセット
        distToRoot = treeDepth(root, nexts);

        //遡る用のdubling配列を用意
        backDub = dubling(parents);
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
