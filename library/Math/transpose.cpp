#pragma once
/**
 * @brief 2 次元配列の転置
 *
 *   transpose(a) … h×w の a を転置した w×h の配列を返す  O(hw)
 *
 * @note a は左辺値で渡す。空なら空を返す
 *
 * @code
 * vvll a = {{1, 2, 3}, {4, 5, 6}};
 * vvll b = transpose(a);  // {{1, 4}, {2, 5}, {3, 6}}
 * @endcode
 */
template<typename T>
vector<vector<T>> transpose(vector<vector<T>> &a){
    if(a.empty()) return {};
    ll h = a.size();
    ll w = a[0].size();
    
    vector<vector<T>> ret(w, vector<T>(h));
    for(int i=0;i<h;i++){
        for(int j=0;j<w;j++){
            ret[j][i] = a[i][j];
        }
    }
    return ret;
}
