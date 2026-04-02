
//一回の行き先が決まってる状態から、K回先を計算する O(N log K)
//要素数:n
//各状態(0 ~ n)から、2^60回までの遷移先を求める(10^18ちょいくらい)
// dub[i][j] := iから2^j回遷移した先
//一回の行き先をnextsに入れとく
//行き先がないやつは-1
vvll doubling(vll &nexts){
    vvll dub(nexts.size(), vll(61));
    rep(i,nexts.size()) dub[i][0] = nexts[i];
    rep(j,1,61) rep(i,nexts.size()){
        if(dub[i][j-1] == -1) dub[i][j] = -1;
        else dub[i][j] = dub[dub[i][j-1]][j-1];
    }
    return dub;
}
