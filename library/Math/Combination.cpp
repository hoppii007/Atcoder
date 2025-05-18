
//コンビネーションを列挙 -> v_combs
//コンビネーションを出すとき、どこを使ったかの情報のみを列挙 -> vb_combs
class Comb{
private:
    //vは最初全部falseにしといてね
    static void vb_combs_sub(vvb &vv, vb &v, int now_count, int now, int k){
        if(now_count == k) return;
        //ちょうど最後の一個を担当するとき
        if(now_count == k-1){
            for(int i=now;i<v.size();i++){
                v[i] = true;
                vv.push_back(v);
                v[i] = false;
            }
            return;
        }
        //まだまだいけるとき
        for(int i=now;i<v.size();i++){
            v[i] = true;
            vb_combs_sub(vv,v,now_count+1,i+1,k);
            v[i] = false;
        }
    }
public:
    Comb(){}
    //vector<bool>の中でk本trueが立つようにする
    //最終的にはコンビネーションの二次元配列を作る
    //bsのサイズは、nCk * nとなる
    static vvb vb_combs(int n,int k){
        vb b = vb(n,false);
        vvb bs;
        vb_combs_sub(bs,b,0,0,k);
        return bs;
    }

    //T型の配列aのコンビネーションを列挙する
    //O(nCk * n)
    template<typename T>
    vector<vector<T>> v_combs(vector<T> &a, int k){
        vvb bs = vb_combs(a.size(),k);
        vector<vector<T>> ret;
        rep(i, bs.size()){
            vector<T> tmp;
            rep(j, bs[i].size()) if(bs[i][j]) tmp.push_back(a[j]);
            ret.push_back(tmp);
        }
        return ret;
    }
};
