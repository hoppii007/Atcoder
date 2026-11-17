// @no-include（main の外に貼るスニペット。区間和のモノイド）
ll op_sum(ll a, ll b){ return a + b; }
ll e_sum(){ return 0; }
using sum_monoid = monoid<ll, op_sum, e_sum>;
