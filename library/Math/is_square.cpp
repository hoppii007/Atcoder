
//四つの頂点の情報をもらい、正方形かどうかを判別する
//点が重なっていたらfalse
bool is_square(pll a,pll b,pll c,pll d){
    vpll ps = {a,b,c,d};
    vll ds;
    rep(i,ps.size()) rep(j,i+1,ps.size()){
        ll dx = ps[i].first-ps[j].first;
        ll dy = ps[i].second-ps[j].second;
        ds.push_back(dx*dx + dy*dy);
    }
    sort(all(ds));
    if(ds[0] == 0) return false;
    return ds[1] == ds[0] && ds[2] == ds[0] && ds[3] == ds[0] && ds[4] == ds[0]*2 && ds[5] == ds[0]*2;
}