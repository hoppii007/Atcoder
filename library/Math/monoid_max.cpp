// @no-include（main の外に貼るスニペット。区間最大のモノイド）
ll op_max(ll a, ll b){ return max(a, b); }
ll e_max(){ return -inf; }
using max_monoid = monoid<ll, op_max, e_max>;
