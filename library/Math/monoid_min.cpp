// @no-include（main の外に貼るスニペット。区間最小のモノイド）
ll op_min(ll a, ll b){ return min(a, b); }
ll e_min(){ return inf; }
using min_monoid = monoid<ll, op_min, e_min>;
