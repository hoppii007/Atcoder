
//座標圧縮
//元々の数値列はソートされてユニークされるので注意
sort(all($1));
$1.erase(unique(all($1)), $1.end());
map<ll,ll> z$1;
rep(zaatui, $1.size()) z$1[$1[zaatui]] = zaatui;
