
//縦0以上h未満、横0以上w未満の(i,j)の4近傍をとる
vpll kinbo4(ll h,ll w, ll i,ll j){
    vpll ret;
    if(i-1 >= 0) ret.push_back({i-1,j});
    if(j-1 >= 0) ret.push_back({i,j-1});
    if(i+1 < h) ret.push_back({i+1,j});
    if(j+1 < w) ret.push_back({i,j+1});
    return ret;
}

//縦0以上h未満、横0以上w未満の(i,j)の8近傍を取る
//順番は左上から普通の二重ループみたいな
vpll kinbo8(ll h,ll w, ll i,ll j){
    vpll ret;
    if(i-1 >= 0 && j-1 >= 0) ret.push_back({i-1, j-1});
    if(i-1 >= 0) ret.push_back({i-1, j});
    if(i-1 >= && j+1 < w) ret.push_back({i-1, j+1});
    if(j-1 >= 0) ret.push_back({i, j-1});
    if(j+1 < w) ret.push_back({i, j+1});
    if(i+1 < h && j-1 >= 0) ret.push_back({i+1, j-1});
    if(i+1 < h) ret.push_back({i+1, j});
    if(i+1 < h && j+1 < w) ret.push_back({i+1, j+1});
    return ret;
}