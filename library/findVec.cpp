
//fr$1[i] := iより右にある最初のhogeのindex
vll fr$1($1.size(), $1.size());
mrep(i, $1.size()-2){
    fr$1[i] = fr$1[i+1];
    if($1[i+1] == hoge) fr$1[i] = i+1;
}


//fl$1[i] := iより左にある最初のhogeのindex
vll fl$1($1.size(), $1.size());
rep(i,1,$1.size()){
    fl$1[i] = fl$1[i-1];
    if($1[i-1] == hoge) fl$1[i] = i-1;
}