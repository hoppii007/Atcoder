
//Nを適当な心数表記したやつ（最上位桁が一番左）
vll digits = {2,4,0,1};
//dp[i桁目まで考えて（0桁目は最上位の上）][状態がjで][smaller]なやつの値
ll stateNum =70;
mll dp[digits.size()+1][stateNum][2];

//初期化
rep(i,digits.size()+1) rep(j,stateNum) rep(smaller, 2) dp[i][j][smaller] = 0;

//初期値の設定
//だいたい最上位の上を0固定にして、Nと完全一致が一つある、としてスタートすればいい
dp[0][/*最上位の上に0が一つある状態のstate*/0][0] = 1;

//digits[0]から始めるけど、dp[1]から更新、みたいな感じ
rep(i,digits.size()) rep(state,stateNum) rep(smaller,2){
    //何進数かでここは変わる
    ll lim = (smaller==1)?1:digits[i];
    rep(nd, lim+1){
        //nsmallerの設定はだいたいこれ
        ll nsmaller = smaller || nd < digits[i];
        //nstateの設定は問題ごとにadhoc
        //stateでndを選んだ時の新しいstate
        ll nstate = state + nd;
        //stateが変になる時、つまり遷移できない・しても意味がない時はcontinueでoK
        if(nstate >= stateNum) continue;

        //現状の一つのstateと、その遷移先が定まったから、あとは遷移
        //dpをmllで作っとけばmodとらなくてOK
        //遷移は桁和だったり、個数だったり総和だったりするから適切に足していく
        //総和の場合は個数用DPと組み合わせたりする
        //とにかく答えを出そうとして、そのために必要なSubDPを揃えていく感じ
        dp[i+1][nstate][nsmaller] += dp[i][state][smaller];
    }
}
