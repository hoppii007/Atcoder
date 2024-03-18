#include<bits/stdc++.h>
//入力系
#define cinll(...) ll __VA_ARGS__; input(__VA_ARGS__);
#define cinint(...) int __VA_ARGS__; input(__VA_ARGS__);
#define cinstr(...) string __VA_ARGS__; input(__VA_ARGS__);
#define cinchar(...) char __VA_ARGS__; input(__VA_ARGS__);
#define cinvll(a,n) vll a(n); rep(i,n) cin>>a[i];
#define cinvs(a,n) vs a(n); rep(i,n) cin>>a[i];
//繰り返し系
#define rep1(n) for(int i=0;i<n;i++)
#define rep2(i,n) for(int i=0;i<n;i++)
#define rep3(i,a,n) for(int i=a;i<n;i++)
#define rep4(i,a,n,b) for(int i=a;i<n;i+=b)
#define overload4(a,b,c,d,e,...) e
#define rep(...) overload4(__VA_ARGS__,rep4,rep3,rep2,rep1)(__VA_ARGS__)
#define mrep1(n) for(int i=n;i>=0;i--)
#define mrep2(i,n) for(int i=n;i>=0;i--)
#define mrep3(i,n,a) for(int i=n;i>=a;i--)
#define mrep4(i,n,a,b) for(int i=n;i>=a;i-=b)
#define mrep(...) overload4(__VA_ARGS__,mrep4,mrep3,mrep2,mrep1)(__VA_ARGS__)
//iterator系
#define all(a) a.begin(),a.end()
using namespace std;
//型系
using ll = long long;
using vll = vector<long long>;
using vvll = vector<vector<long long>>;
using vi = vector<int>;
using vvi = vector<vector<int>>;
using vb = vector<bool>;
using vvb = vector<vector<bool>>;
using vc = vector<char>;
using vvc = vector<vector<char>>;
using vs = vector<string>;
using pll = pair<long long,long long>;
using vpll = vector<pair<long long,long long>>;


#include<bits/stdc++.h>
//入力系
#define cinll(...) ll __VA_ARGS__; input(__VA_ARGS__);
#define cinint(...) int __VA_ARGS__; input(__VA_ARGS__);
#define cinstr(...) string __VA_ARGS__; input(__VA_ARGS__);
#define cinchar(...) char __VA_ARGS__; input(__VA_ARGS__);
#define cinvll(a,n) vll a(n); rep(i,n) cin>>a[i];
#define cinvs(a,n) vs a(n); rep(i,n) cin>>a[i];
//繰り返し系
#define rep1(n) for(int i=0;i<n;i++)
#define rep2(i,n) for(int i=0;i<n;i++)
#define rep3(i,a,n) for(int i=a;i<n;i++)
#define rep4(i,a,n,b) for(int i=a;i<n;i+=b)
#define overload4(a,b,c,d,e,...) e
#define rep(...) overload4(__VA_ARGS__,rep4,rep3,rep2,rep1)(__VA_ARGS__)
#define mrep1(n) for(int i=n;i>=0;i--)
#define mrep2(i,n) for(int i=n;i>=0;i--)
#define mrep3(i,n,a) for(int i=n;i>=a;i--)
#define mrep4(i,n,a,b) for(int i=n;i>=a;i-=b)
#define mrep(...) overload4(__VA_ARGS__,mrep4,mrep3,mrep2,mrep1)(__VA_ARGS__)
//iterator系
#define all(a) a.begin(),a.end()
using namespace std;
//型系
using ll = long long;
using vll = vector<long long>;
using vvll = vector<vector<long long>>;
using vi = vector<int>;
using vvi = vector<vector<int>>;
using vb = vector<bool>;
using vvb = vector<vector<bool>>;
using vc = vector<char>;
using vvc = vector<vector<char>>;
using vs = vector<string>;
using pll = pair<long long,long long>;
using vpll = vector<pair<long long,long long>>;


#include "../Math/Frac.cpp";



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


