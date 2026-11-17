#pragma once
/**
 * @brief 2 次方程式 ax^2 + bx + c = 0 の整数解
 *
 *   quadratic_equation(a, b, c) … 整数解を昇順で返す（なければ空、重解は 1 個）  O(log 10^9)
 *
 * @note 0 < |a| <= 10^9, |b| <= 10^9, |c| <= 10^18, |解| <= 10^9 の範囲で使う
 *
 * @code
 * vll xs = quadratic_equation(1, -5, 6);  // {2, 3}
 * @endcode
 */
vll quadratic_equation(ll a, ll b, ll c){
    std::function<bool(ll)> is_zero = [&](ll x){
        if((-b*x-c)%a != 0) return false;
        return x*x == (-b*x-c)/a;
    };

    //一点0があるとき
    if(b%(2*a) == 0 && is_zero(-b/(2*a))){
        return {-b/(2*a)};
    }
    
    //↓以降は実数解が二つ or ない

    //二分探索のジャッジ用のやつ
    // ax^2 + bx + c <= 0 ? 
    auto binary_search_judge = [&](ll mid){
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
        if(binary_search_judge(mid)) ok = mid;
        else ng = mid;
    }
    //ピッタリ0ならOK
    if(is_zero(ok)) ret.push_back(ok);
    
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
        if(binary_search_judge(mid)) ok = mid;
        else ng = mid;
    }
    if(is_zero(ok)) ret.push_back(ok);

    return ret;
}
