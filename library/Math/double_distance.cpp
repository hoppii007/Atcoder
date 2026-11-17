#pragma once
/**
 * @brief 平面上の2点間のユークリッド距離
 *
 *   euclid_distance(p1, p2) … 点 p1, p2 (x, y) の距離  O(1)
 *
 * @code
 * double d = euclid_distance({0, 0}, {3, 4});  // 5.0
 * @endcode
 */
double euclid_distance(pair<double,double> p1, pair<double,double> p2){
    double dx = p1.first - p2.first;
    double dy = p1.second - p2.second;
    return sqrt(dx*dx + dy*dy);
}
