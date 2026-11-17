// @no-include（main の中に貼るスニペット。#include では使えない）
//三分探索
//目的関数
//整数値関数ならf(mid) > f(mid+1)となる最大を見つける二分探索でいい
//sanbun_fは真に単調な下に凸な関数である必要がある
std::function<double(double)> sanbun_f = [&](double san_x){
    return san_x + p/pow(2,san_x/1.5);
};
double san_l = 0;
double san_r = inf;
rep(san_i, 500){
    double c1 = (san_l*2+san_r) / 3;
    double c2 = (san_l+san_r*2) / 3;
    if(sanbun_f(c1) > sanbun_f(c2)) san_l = c1;
    else san_r = c2;
}
//sanbun_f(san_l)とかが答えになる
