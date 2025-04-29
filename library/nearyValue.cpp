
//昇順sortされたaの中から、最も近い値をO(N)で返す
template<typename T>
pair<T,ll> nearyValue(vector<T> &a, T t){
    auto it = lower_bound(all(a), t);
    //以上のやつが一個もないとき（すべてtより小さいとき）
    if(it == a.end()) return {a[(ll)a.size()-1], (ll)a.size()-1};
    //一個前もあればそれと今の比較
    if(it != a.begin()){
        auto pit = it;
        pit--;
        //一個前の方がitよりtに近ければそれを返す
        if(abs(*pit-t) <= abs(*it-t)) return {*pit, pit-a.begin()};
        else return {*it, it-a.begin()};
    }
    return {*it, it-a.begin()};
}
