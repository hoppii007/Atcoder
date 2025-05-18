
//三分探索
//目的関数
//整数値関数ならf(mid) > f(mid+1)となる最大を見つける二分探索でいい
//sanbun_fは真に単調な下に凸な関数である必要がある
std::function<double(double)> sanbun_f = [&](double sanX){
    return sanX + p/pow(2,sanX/1.5);
};
double sanL = 0;
double sanR = inf;
rep(sanI, 500){
    double c1 = (sanL*2+sanR) / 3;
    double c2 = (sanL+sanR*2) / 3;
    if(sanbun_f(c1) > sanbun_f(c2)) sanL = c1;
    else sanR = c2;
}
//sanbun_f(sanL)とかが答えになる
