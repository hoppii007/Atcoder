
//ax^2+bx+c=0を満たす整数解を返す
//整数解がなければ空配列が帰ってくる
//重解は長さ1で帰ってくる
//0 < |a| <= 1e9
//|b| <= 1e9
//|c| <= 1e18
//|解| <= 1e9
//O(log 1e9)
vll nijihotei(ll a, ll b, ll c){
    std::function<bool(ll)> isZero = [&](ll x){
        if((-b*x-c)%a != 0) return false;
        return x*x == (-b*x-c)/a;
    };

    //一点0があるとき
    if(b%(2*a) == 0 && isZero(-b/(2*a))){
        return {-b/(2*a)};
    }
    
    //↓以降は実数解が二つ or ない

    //二分探索のジャッジ用のやつ
    // ax^2 + bx + c <= 0 ? 
    auto nibutan_judge = [&](ll mid){
        return mid*mid <= (-b*mid-c)/a;
    };

    vll ret;
    //ax^2+bx+c <= 0の中で最大を出す
    //まずは極値点の左側
    ll ok,ng;
    if(a > 0){
        ok = (-b/(2*a));
        ng = -1000000000LL;
    }
    else{
        ng = -1000000000LL;
        ok = (-b/(2*a));
    }
    while(abs(ok-ng) > 1){
        ll mid = (ok+ng)/2;
        if(nibutan_judge(mid)) ok = mid;
        else ng = mid;
    }
    //ピッタリ0ならOK
    if(isZero(ok)) ret.push_back(ok);
    
    //極値点の右側
    if(a > 0){
        ok = (-b/(2*a));
        if(b%(2*a) != 0) ok++;
        ng = 1000000000LL;
    }
    else{
        ng = 1000000000LL;
        ok = (-b/(2*a));
        if(b%(2*a) != 0) ok++;
    }
    while(abs(ok-ng) > 1){
        ll mid = (ok+ng)/2;
        if(nibutan_judge(mid)) ok = mid;
        else ng = mid;
    }
    if(isZero(ok)) ret.push_back(ok);

    return ret;
}
