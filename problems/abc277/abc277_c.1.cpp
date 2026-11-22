#include "template.hpp"  // 先頭に置くこと（プリコンパイル済みヘッダが効く条件）
#include "library/Math/coordinate_compression.cpp"

vb vis;
vvll nexts;

ll dfs(ll now){
    vis[now] = true;
    ll ret = now;
    for(ll nxt: nexts[now]) if(vis[nxt] == false) {
        chmax(ret, dfs(nxt));
    }
    return ret;
}

int main(){
    cinll(n);
    cinvpll(tab, n);
    vll points = {1};
    rep(i, n) {
        points.pb(tab[i].fst);
        points.pb(tab[i].snd);
    }

    auto [z, zp] = coordinate_compression(points);
    nexts.resize(z.size());
    vis.resize(z.size(), false);
    rep(i, n){
        nexts[z[tab[i].fst]].pb(z[tab[i].snd]);
        nexts[z[tab[i].snd]].pb(z[tab[i].fst]);
    }
    cout << zp[dfs(0)] << endl;
    return 0;
}