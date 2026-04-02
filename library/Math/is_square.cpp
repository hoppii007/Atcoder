
//四つの頂点の情報をもらい、正方形かどうかを判別する
//overlapIsOutがtrueなら、ぴったりなってる時ダメにする
bool is_square(pll a,pll b,pll c,pll d, bool overlapIsOut = true){
    vpll ps = {a,b,c,d};
    sort(all(ps));
    swap(ps[2], ps[3]);
    //長さチェック
    ll dx = ps[3].first-ps[0].first;
    ll dy = ps[3].second-ps[0].second;
    ll len = dx*dx + dy*dy;
    if(overlapIsOut && len == 0) return false; 
    rep(i,ps.size()){
        dx = ps[(i+1)%4].first - ps[i].first;
        dy = ps[(i+1)%4].second - ps[i].second;
        if(dx*dx + dy*dy != len) return false;
    }
    //角度チェック
    rep(i,ps.size()){
        ll dx1 = ps[(i+1)%4].first - ps[i].first;
        ll dy1 = ps[(i+1)%4].second - ps[i].second;
        ll dx2 = ps[(i+2)%4].first - ps[(i+1)%4].first;
        ll dy2 = ps[(i+2)%4].second - ps[(i+1)%4].second;
        if(dy1 * dy2 != -dx1 * dx2) return false;
    }
    return true;
}
