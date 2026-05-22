/**
 * @brief セグメント木（点更新・区間取得）
 *
 * @tparam M モノイド型（library/Math/monoid.cpp の Monoid を想定）
 * @note 構築 O(N), 更新 O(log N), 取得 O(log N)
 *
 * @code
 * ll op_sum(ll a, ll b){ return a + b; }
 * using Sum = Monoid<ll, op_sum, 0>;
 *
 * vll a = {1, 2, 3, 4, 5};
 * SegmentTree<Sum> seg(a);
 * seg.change(2, 10);          // a[2] を 10 に
 * ll s = seg.get(1, 3);       // a[1] + a[2] + a[3] = 16
 * @endcode
 */
template<typename M>
class SegmentTree{
    using T = typename M::value_type;
    int n;
    vector<T> array;
public:
    SegmentTree(vector<T> &v){
        int sz = v.size();
        this->n = 1;
        while(n < sz) n *= 2;
        this->array = vector<T>(2*this->n-1, M::e);
        for(int i=0;i<sz;i++) change(i,v[i]);
    }
    SegmentTree(int a){
        this->n = 1;
        while(n < a) n*= 2;
        this->array = vector<T>(2*this->n-1, M::e);
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
            this->array[parent] = M::merge(this->array[l_child(parent)],this->array[r_child(parent)]);
            parent = get_parent(parent); 
        }
    }
    T get(int l,int r,int k=0,int nowL=-1,int nowR=-1){
        if(k == 0){
            nowL = 0;
            nowR = this->array.size()/2;
        }
        if(r < nowL || nowR < l) return M::e;
        if(l <= nowL && nowR <= r) return this->array[k];
        T vl = get(l,r,l_child(k),nowL,(nowL+nowR)/2);
        T vr = get(l,r,r_child(k),(nowL+nowR)/2+1,nowR);
        return M::merge(vl,vr);
    }
};

