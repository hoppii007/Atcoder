#pragma once
#include <cassert>
/**
 * @brief 行列（積・スカラー倍・累乗）
 *
 *   matrix<T> m(h, w)   … h×w の零行列（要素は T() で初期化）
 *   matrix<T> m(g)      … vector<vector<T>> から構築
 *   m.g[i][j], m.h, m.w … 要素 / 行数 / 列数
 *   a * b, a *= b       … 行列積（a.w == b.h）  O(h·w·k)
 *   a * k, k * a        … スカラー倍（k は ll）
 *   a ^ k, a ^= k       … 累乗（正方行列、k >= 0。k = 0 は単位行列）  O(n^3 log k)
 *   m.show()            … 標準出力に表示
 *
 * @tparam T 要素型（+ と * が必要。T() が 0、T(1) が 1 になること。modll など）
 * @note サイズ不一致の積・非正方の累乗は assert で落ちる
 *
 * @code
 * // フィボナッチ（mod）
 * matrix<modll> m({{1, 1}, {1, 0}});
 * matrix<modll> p = m ^ n;
 * modll fib_n = p.g[0][1];
 * @endcode
 */
template<typename T>
class matrix{
public:
    ll h;
    ll w;
    vector<vector<T>> g;
    matrix(ll th,ll tw){
        this->h = th;
        this->w = tw;
        this->g = vector<vector<T>>(h,vector<T>(w));
    }
    matrix(vector<vector<T>> tg){
        this->h = tg.size();
        this->w = tg[0].size();
        this->g = vector<vector<T>>(h,vector<T>(w));
        rep(i,h) rep(j,w) this->g[i][j] = tg[i][j];
    }
    matrix(){
        this->h = 1;
        this->w = 1;
        this->g = vector<vector<T>>(this->h,vector<T>(this->w));
    }
    void show(){
        rep(i,h){
            rep(j,w) cout<<g[i][j]<<" ";
            cout<<endl;
        }
    }
    friend matrix<T> operator*(const matrix<T> &a, const matrix<T> &b){
        assert(a.w == b.h);
        matrix<T> ret = matrix<T>(a.h,b.w);
        rep(i,a.h) rep(j,b.w) rep(k,a.w) ret.g[i][j] = ret.g[i][j]+a.g[i][k]*b.g[k][j];
        return ret;
    }
    friend matrix<T> operator*(const matrix<T> &a, const ll &b){
        matrix<T> ret(a.g);
        rep(i,ret.h) rep(j,ret.w) ret.g[i][j] = ret.g[i][j]*b;
        return ret;
    }
    friend matrix<T> operator*(const ll &a, const matrix<T> &b){
        matrix<T> ret(b.g);
        rep(i,ret.h) rep(j,ret.w) ret.g[i][j] = ret.g[i][j]*a;
        return ret;
    }
    friend matrix<T> operator^(const matrix<T> &a, const ll &b){
        assert(a.h == a.w && b >= 0);
        if(b == 0){
            matrix<T> id(a.h, a.w);
            rep(i, a.h) id.g[i][i] = T(1);
            return id;
        }
        if(b == 1) return a;
        if(b %2 == 0){
            return (a*a)^(b/2);
        }
        else return a*(a^(b-1));
    }
    friend void operator*=(matrix<T> &a, const matrix<T> &b){ a = a*b; }
    friend void operator*=(matrix<T> &a, const ll &b){ a = a*b; }
    friend void operator^=(matrix<T> &a, const ll &b){ a = a^b; }
};
