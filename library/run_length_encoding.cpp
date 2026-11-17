#pragma once
/**
 * @brief ランレングス圧縮
 *
 *   run_length(a)  … 連続する同じ値をまとめた {個数, 値} の列  O(N)
 *
 * @note 返り値は vector<pair<ll, T>>（first = 個数, second = 値）
 *
 * @code
 * vll a = {1, 1, 2, 2, 2, 1};
 * auto r = run_length(a);   // {{2, 1}, {3, 2}, {1, 1}}
 * @endcode
 */
template <typename T>
vector<pair<ll,T>> run_length(vector<T> &a){
    vector<pair<ll,T>> ret;
    if(a.size() == 0) return ret;

    T pre = a[0];
    ll cnt = 0;
    rep(i,a.size()){
        if(pre != a[i]){
            ret.push_back({cnt, pre});
            cnt = 0;
            pre = a[i];
        }
        cnt++;
    }
    ret.push_back({cnt, pre});

    return ret;
}
