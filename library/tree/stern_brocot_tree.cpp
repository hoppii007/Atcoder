#pragma once
#include "library/Math/fraction.cpp"

// Stern-Brocot木
// ・有理数を小さい順に並べられる
// (0 ~ 1)の有理数を小さい順に並べるのはファレイ数列（Stern-Brocot木の部分木になる

//分子分母がn以下でlx/ly <= d <= rx/ryとなるx,yを求める
//{lx,ly,rx,ry}が帰ってくる
//頂点はla/lbからra/rb)の区間
//単純な二分探索だと最悪O(N)になるので、指数探索を使って効率化する
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


