#pragma once
/**
 * @brief グリッドの方向ベクトル {di, dj}
 *
 *   lines4 … 上・右・下・左
 *   lines8 … 周囲 8 方向
 *
 * @code
 * for(auto [di, dj]: lines4){
 *     ll ni = i + di, nj = j + dj;
 * }
 * @endcode
 */
vpll lines8 = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
vpll lines4 = {{-1,0},{0,1},{1,0},{0,-1}};
