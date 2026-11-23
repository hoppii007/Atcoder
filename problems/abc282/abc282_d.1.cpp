#include "template.hpp"  // 先頭に置くこと（プリコンパイル済みヘッダが効く条件）
#include "library/Graph/split_connected_graph.cpp"


// color[now]に色を塗り伝播させていく。ダメならfalseを返す
bool sub_bipartite_graph(vvll& nexts, vb& visit, vb& color, ll now, bool prev){
    visit[now] = true;
    color[now] = !prev;
    for(ll nxt: nexts[now]){
        if(visit[nxt] == false) {
            if(sub_bipartite_graph(nexts, visit, color, nxt, color[now]) ==false) return false;
        }
        else if(color[nxt] == color[now]) return false;
    }
    return true;
}
// 二部グラフで色分けができるか？できるなら、色分けた時の各頂点の色(true, false)を返す
// nextsは連結グラフを仮定
pair<bool,vb> bipartite_graph(vvll& nexts){
    vb ret(nexts.size());
    vb visit(nexts.size(), false);
    if(sub_bipartite_graph(nexts, visit, ret, 0, false) == false) return {false, ret};
    return {true, ret};
}

int main(){
    cinll(n, m);
    vvll rnexts(n);
    rep(i, m){
        cinll(u, v);
        u--; v--;
        rnexts[u].pb(v);
        rnexts[v].pb(u);
    }
    vector<vvll> s = split_connected_graph(rnexts);
    ll ans = 0;
    for(vvll& nexts: s){
        auto [can_nibu, nibu] = bipartite_graph(nexts);
        if(can_nibu == false) {
            cout << 0 << endl;
            return 0;
        }
        ll gc0 = count(all(nibu), false);
        ll gc1 = count(all(nibu), true);
        // 各頂点に対して、その頂点と繋がっている違う色の個数を出す
        rep(i, nexts.size()){
            ll cnt = 0;
            for(ll nxt: nexts[i]) if(nibu[i] != nibu[nxt]) cnt++;
            if(nibu[i]) ans += gc0-cnt;
            else ans += gc1-cnt;
        }
    }
    ans /= 2;

    vll r(s.size());
    // r[i] := (i,n-1]の合計
    r[s.size()-1] = 0;
    mrep(i, (ll)s.size()-2) {
        r[i] = r[i+1]+s[i+1].size();
        ans += (s[i].size() * r[i]);
    }

    
    cout << ans << endl;


    return 0;
}