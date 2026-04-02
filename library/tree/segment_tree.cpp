/**
 * @brief セグメント木（点更新・区間取得）
 * @tparam T 要素の型
 * @note 構築 O(N), 更新 O(log N), 取得 O(log N)
 *
 * コンストラクタ:
 *   SegmentTree<T>(vector<T>&, T (*f)(T,T), T ide)
 *   SegmentTree<T>(int n, T (*f)(T,T), T ide)
 *
 * メソッド:
 *   change(int i, T v) - i番目の要素をvに更新
 *   get(int l, int r)  - [l, r]の区間取得（閉区間）
 */
template<typename T>
class SegmentTree{
    int n;
    T ident;
    vector<T> array;
    T (*op)(T,T);
public:
    SegmentTree(vector<T> &v,T (*f)(T,T),T ide){
        int sz = v.size();
        this->n = 1;
        while(n < sz) n *= 2;
        this->array = vector<T>(2*this->n-1,ide);
        this->ident = ide;
        this->op = f;
        for(int i=0;i<sz;i++) change(i,v[i]);
    }
    SegmentTree(int a,T (*f)(T,T),T ide){
        this->n = 1;
        while(n < a) n*= 2;
        this->array = vector<T>(2*this->n-1,ide);
        this->ident = ide;
        this->op = f;
    }
    SegmentTree(){}
    int l_child(int i){ return i*2+1; }
    int r_child(int i){ return i*2+2; }
    int get_parent(int i){ return (i%2 == 0) ? (i/2-1):(i/2); }
    void change(int i,T v){
        int ind = this->array.size()/2+i;
        this->array[ind] = v;
        int parent = get_parent(ind);
        while(parent != -1){
            this->array[parent] = op(this->array[l_child(parent)],this->array[r_child(parent)]);
            parent = get_parent(parent); 
        }
    }
    T get(int l,int r,int k=0,int nowL=-1,int nowR=-1){
        if(k == 0){
            nowL = 0;
            nowR = this->array.size()/2;
        }
        if(r < nowL || nowR < l) return this->ident;
        if(l <= nowL && nowR <= r) return this->array[k];
        T vl = get(l,r,l_child(k),nowL,(nowL+nowR)/2);
        T vr = get(l,r,r_child(k),(nowL+nowR)/2+1,nowR);
        return op(vl,vr);
    }
};

