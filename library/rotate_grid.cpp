#pragma once
/**
 * @brief グリッドの回転（正方形グリッドを右に 90 度回す）
 *
 *   rotate_grid(s)  … s を時計回りに 90 度回したグリッド  O(N^2)
 *
 * @note s は N×N の正方形であること
 *
 * @code
 * vvll s = {{1, 2}, {3, 4}};
 * vvll t = rotate_grid(s);   // {{3, 1}, {4, 2}}
 * @endcode
 */
template<typename T>
vector<vector<T>> rotate_grid(vector<vector<T>> &s){
    vector<vector<T>> ret(s.size(), vector<T>(s.size()));
    rep(j,s.size()) rep(i,s.size()) ret[j][i] += s[(ll)s.size()-1-i][j];
    return ret;
}
