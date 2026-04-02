
// 各頂点に対して、その頂点より左(or右)の区間の中で一番小さいところに辺を張る二分木
// 構築はO(N)
struct CartesianTree{
    vll v;
    vll left, right, parent;
    ll root = -inf;
    CartesianTree(vll &a): v(a), left(a.size(),-inf), right(a.size(),-inf), parent(a.size(),-inf){
        stack<ll> s;
        rep(i, a.size()){
            ll last_pop = -inf;
            // 大きい順のヒープにする時は、ここの右側を<に変更
            while(s.size() > 0 && a[s.top()] > a[i]){
                last_pop = s.top();
                s.pop();
            }
            if(last_pop != -inf){
                if(parent[last_pop] != -inf){
                    right[parent[last_pop]] = i;
                    parent[i] = parent[last_pop];
                }
                parent[last_pop] = i;
                left[i] = last_pop;
            }
            if(s.size() > 0){
                right[s.top()] = i;
                parent[i] = s.top();
            }
            s.push(i);
        }

        rep(i, a.size()) if(parent[i] == -inf) root = i;
    }
};
