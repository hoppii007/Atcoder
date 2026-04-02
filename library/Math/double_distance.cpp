
//平面座標の2点間の距離
double euclid_distance(pair<double,double> p1, pair<double,double> p2){
    double dx = p1.first - p2.first;
    double dy = p1.second - p2.second;
    return sqrt(dx*dx + dy*dy);
}
