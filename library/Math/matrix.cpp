
//Tは乗算単位元と加算単位元が必要
//掛け算と足し算も必要( += とか *= はなくてもOK)
//^0は計算できない
template<typename T>
class Matrix{
public:
    ll h;
    ll w;
    vector<vector<T>> g;
    Matrix(ll th,ll tw){
        this->h = th;
        this->w = tw;
        this->g = vector<vector<T>>(h,vector<T>(w));
    }
    Matrix(vector<vector<T>> tg){
        this->h = tg.size();
        this->w = tg[0].size();
        this->g = vector<vector<T>>(h,vector<T>(w));
        rep(i,h) rep(j,w) this->g[i][j] = tg[i][j];
    }
    Matrix(){
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
    friend Matrix<T> operator*(const Matrix<T> &a, const Matrix<T> &b){
        if(a.w != b.h) return Matrix<T>();
        Matrix<T> ret = Matrix<T>(a.h,b.w);
        rep(i,a.h) rep(j,b.w) rep(k,a.w) ret.g[i][j] = ret.g[i][j]+a.g[i][k]*b.g[k][j];
        return ret;
    }
    friend Matrix<T> operator*(const Matrix<T> &a, const ll &b){
        Matrix<T> ret(a.g);
        rep(i,ret.h) rep(j,ret.w) ret.g[i][j] = ret.g[i][j]*b;
        return ret;
    }
    friend Matrix<T> operator*(const ll &a, const Matrix<T> &b){
        Matrix<T> ret(b.g);
        rep(i,ret.h) rep(j,ret.w) ret.g[i][j] = ret.g[i][j]*a;
        return ret;
    }
    friend Matrix<T> operator^(const Matrix<T> &a, const ll &b){
        if(a.h != a.w) return Matrix<T>();
        if(b == 1) return a;
        if(b %2 == 0){
            return (a*a)^(b/2);
        }
        else return a*(a^(b-1));
    }
    friend void operator*=(Matrix<T> &a, const Matrix<T> &b){ a = a*b; }
    friend void operator*=(Matrix<T> &a, const ll &b){ a = a*b; }
    friend void operator^=(Matrix<T> &a, const ll &b){ a = a^b; }
};
