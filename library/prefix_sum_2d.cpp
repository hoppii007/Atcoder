#pragma once
/**
 * @brief 2次元累積和の配列を返す  O(HW)
 *
 *   s = prefix_sum_2d(g)              … (H+1) × (W+1)。s[i][j] = g の [0, i) × [0, j) の和
 *   rect_sum(s, x1, y1, x2, y2)       … g の [x1, x2) × [y1, y2) の和（空なら 0）  O(1)
 *
 * @param g 元の2次元配列（H × W）
 * @return 累積和配列 s（(H+1) × (W+1)）
 * @note 区間は半開区間（0-indexed、x が行・y が列）
 *
 * @code
 * vvll g = {{1, 2, 3},
 *           {4, 5, 6}};
 * vvll s = prefix_sum_2d(g);
 * ll x = rect_sum(s, 0, 1, 2, 3);   // [0,2) × [1,3) = 2 + 3 + 5 + 6 = 16
 * @endcode
 */
template<typename T>
vector<vector<T>> prefix_sum_2d(const vector<vector<T>>& g){
    int h = g.size(), w = h ? g[0].size() : 0;
    vector<vector<T>> s(h + 1, vector<T>(w + 1, T(0)));
    for(int i = 0; i < h; i++) for(int j = 0; j < w; j++){
        s[i+1][j+1] = s[i][j+1] + s[i+1][j] - s[i][j] + g[i][j];
    }
    return s;
}

/**
 * @brief 2次元累積和配列 s から長方形領域の和を取る  O(1)
 * @param s prefix_sum_2d で作った累積和配列
 * @return 元配列の [x1, x2) × [y1, y2) の和（x1 >= x2 または y1 >= y2 なら 0）
 */
template<typename T>
T rect_sum(const vector<vector<T>>& s, int x1, int y1, int x2, int y2){
    if(x1 >= x2 || y1 >= y2) return T(0);
    return s[x2][y2] - s[x1][y2] - s[x2][y1] + s[x1][y1];
}
