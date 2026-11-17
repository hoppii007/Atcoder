#pragma once
/**
 * @brief Cartesian Tree（配列から作る最小値ヒープの二分木）
 *
 *   cartesian_tree ct(a) … 構築  O(N)
 *   ct.root             … 根（a の最小値の位置）
 *   ct.left[i]          … i の左の子（無ければ -inf）
 *   ct.right[i]         … i の右の子（無ければ -inf）
 *   ct.parent[i]        … i の親（根は -inf）
 *
 * @note 各部分木は a の連続区間。等しい値は左側が祖先になる
 * @note 最大値ヒープにするにはコンストラクタ内の比較 > を < に変える
 *
 * @code
 * vll a = {3, 1, 4, 1, 5};
 * cartesian_tree ct(a);
 * ll r = ct.root;  // 1
 * @endcode
 */
struct cartesian_tree{
    vll v;
    vll left, right, parent;
    ll root = -inf;
    cartesian_tree(vll &a): v(a), left(a.size(),-inf), right(a.size(),-inf), parent(a.size(),-inf){
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
