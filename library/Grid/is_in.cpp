
// [0,h) x [0,w) に入っているかのチェック
bool is_in(ll h, ll w, ll i, ll j){
	if(i < 0 || h <= i) return false;
	if(j < 0 || w <= j) return false;
	return true;
}