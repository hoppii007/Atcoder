
//r$1[i][j] := g[0][0] ~ g[i-1][j-1]までの合計
//[i,j] ~ (i+a,j+b)の合計を出すには、r$1[i+a][j+b]-r$1[i+a-1][j+b]-r$1[i+a][j+b-1]+r$1[i][j]
vvll r$1(r$1.size()+1, vll($1[0].size()+1, 0));
rep(i,$1.size()) rep(j,$1[i].size()) r$1[i+1][j+1] = r$1[i+1][j]+r$1[i][j+1]-r$1[i][j] + $1[i][j];
//[i,j] ~ 長さ(a,b)の面積
std::function<ll(ll,ll,ll,ll)> r$1f = [&](ll r$1i, ll r$1j, ll r$1a, ll r$1b){
    return r$1[r$1i+r$1a][r$1j+r$1b]-r$1[r$1i][r$1j+r$1b]-r$1[r$1i+r$1a][r$1j]+r$1[r$1i][r$1j];
};
