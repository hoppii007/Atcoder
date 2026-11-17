#pragma once
#include "library/Math/fraction.cpp"

/**
 * @brief Stern-Brocot 木で d を挟む最良近似分数
 *
 *   SternBrocotTree(d, n) … {la, lb, ra, rb}（分子・分母が n 以下で la/lb <= d <= ra/rb となる最も近い 2 分数）
 *
 * @note 0 <= d <= 1 が前提（0/1 と 1/1 から探索する）
 * @note 0 < d < 1 で d 自体が分子・分母 n 以下で表せるなら la/lb = ra/rb = d
 *
 * @code
 * vll r = SternBrocotTree(Frac(2, 7), 5);  // {1, 4, 1, 3}：1/4 <= 2/7 <= 1/3
 * @endcode
 */
vll SternBrocotTree(Frac d, ll n){
    ll la,lb;
    ll ra,rb;
    la = 0; lb = 1;
    ra = 1; rb = 1;
    while(1){
        ll ma = la + ra;
        ll mb = lb + rb;
        
        if(ma > n || mb > n){
            return {la,lb,ra,rb};
        }
        
        if(d == Frac(ma,mb)){
            return {ma,mb,ma,mb};
        }
        
        
        //左側にあるとき
        if(d < Frac(ma,mb)){
            //潜れる限界まで行く
            ll two = 2;
            ll na = ra + two * la;
            ll nb = rb + two * lb;
            
            while(na <= n && nb <= n && d < Frac(na,nb)){
                ma = na;
                mb = nb;
                two *= 2;
                na = ra + two * la;
                nb = rb + two * lb;
            }
            ra = ma;
            rb = mb;
        }
        else if(d > Frac(ma,mb)){
            //潜れる限界まで行く
            ll two = 2;
            ll na = la + two * ra;
            ll nb = lb + two * rb;
            
            while(na <= n && nb <= n && d > Frac(na,nb)){
                ma = na;
                mb = nb;
                two *= 2;
                na = la + two * ra;
                nb = lb + two * rb;
            }
            
            la = ma;
            lb = mb;
        }
        else{
            return {ma,mb,ma,mb};
        }
    }
    
}


