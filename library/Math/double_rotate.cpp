#pragma once
/**
 * @brief 点の回転（原点中心・反時計回り）
 *
 *   double_rotate(p, theta) … 点 p (x, y) を原点中心に theta [rad] 回した点  O(1)
 *
 * @code
 * pd q = double_rotate({1, 0}, PI / 2);  // ≒ (0, 1)
 * @endcode
 */
pair<double, double> double_rotate(pair<double,double> p, double theta){
    double nx = cos(theta)*p.first - sin(theta)*p.second;
    double ny = sin(theta)*p.first + cos(theta)*p.second;
    return {nx, ny};
}