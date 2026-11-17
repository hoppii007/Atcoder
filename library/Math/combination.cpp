#pragma once
/**
 * @brief 組合せ列挙（n 個から k 個選ぶ全通り）
 *
 *   combination::vb_combs(n, k) … 選んだ位置を true にした vb を全通り返す（static）  O(nCk * n)
 *   combination().v_combs(a, k) … 配列 a から k 個選んだ部分列を全通り返す  O(nCk * n)
 *
 * @note k = 0 なら「何も選ばない」1 通り、k > n なら 0 通り
 *
 * @code
 * vvb bs = combination::vb_combs(4, 2);  // 6 通り
 * vll a = {1, 2, 3};
 * combination cb;
 * vvll cs = cb.v_combs(a, 2);  // {1,2} {1,3} {2,3}
 * @endcode
 */
class combination{
private:
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
    combination(){}
    static vvb vb_combs(int n,int k){
        vb b = vb(n,false);
        if(k == 0) return vvb(1, b);
        vvb bs;
        vb_combs_sub(bs,b,0,0,k);
        return bs;
    }

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
