
//原点中心に反時計回りに回転（ラジアン）
//xy平面で
pair<double, double> double_rotate(pair<double,double> p, double theta){
    double nx = cos(theta)*p.first - sin(theta)*p.second;
    double ny = sin(theta)*p.first + cos(theta)*p.second;
    return {nx, ny};
}