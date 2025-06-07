
// 有向グラフに対して、O（N)でサイクルを一つ返す
// rootから先でサイクルがあるかを見るので、連結じゃないとか、rootより前でループしてたら判定できない
// あくまで一つを返すだけ
// 帰ってきた配列cycle[0] -> cycle[1] -> cycle[2] ... -> cycle[cycle.size()-1] -> cycle[0]のようにループしてる
vll findCycle(vvll &nexts, ll root){
    vpll ret(nexts.size(), {-1,-1});
    ll order = 0;
    vll cycle;
    std::function<ll(ll)> dfs = [&](ll now){
        ret[now].fst = order++;
        for(ll next: nexts[now]){
            if(ret[next].fst == -1){
                ll result = dfs(next);
                //ループを見つけて帰ってきた時
                if(result != -1){
                    //ループ元に辿り着いてなければ自分もループの仲間
                    if(result != inf) cycle.push_back(now);
                    if(result == now) return inf;
                    else return result;
                }
            }
            else{
                //きたことはあって終わってないところに来たらループ！
                if(ret[next].snd == -1){
                    cycle.push_back(now);
                    return next;
                }
            }
        }
        ret[now].snd = order;
        return -1LL;
    };
    dfs(root);
    //最後についたところから保存されるので、ループ順にするには逆にしないといけない
    reverse(all(cycle));
    return cycle;
}
