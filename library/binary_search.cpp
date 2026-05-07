//二分探索のジャッジ用のやつ
auto nibutan_judge = [&](ll mid){
    return true;
};
ll ok = inf;
ll ng = 0;
while(abs(ok-ng) > 1){
    ll mid = (ok+ng)/2;
    if(nibutan_judge(mid)) ok = mid;
    else ng = mid;
}