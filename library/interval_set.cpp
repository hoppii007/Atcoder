
// 区間をsetで管理するテク(半開区間)
// 区間の追加・削除クエリがO(log N)
// かぶっている区間は勝手に削除したり統合されたりする
// ・追加・削除クエリがQ回あって、毎回生まれる区間の個数が有限個しかないなら、
// 　削除されたり統合されたりする回数も累積でO(Q)回のため、全体の計算量はO(Q*logQ + Q)となる
struct IntervalSet{
    // 最初は空区間が入っている
    set<pll> sets = {};

    // xが入っている区間以降で最初の区間のイテレータを返す
    set<pll>::iterator lb(ll x){
        if(sets.size() == 0) return sets.end();

        auto ret = sets.lower_bound({x, x});
        if(ret == sets.end()){
            auto pre = prev(ret);
            if((*pre).fst <= x && x < (*pre).snd) return pre;
            else return sets.end();
        }
        if((*ret).fst == x) return ret;
        if(ret == sets.begin()) return ret;
        auto pre = prev(ret);
        if((*pre).fst <= x && x < (*pre).snd) return pre;
        return ret;
    }

    // 新たにinsertされた合計の長さを返す
    ll insert(ll l, ll r){
        // 影響の可能性がある最初の区間
        auto itl = lb(l);
        auto itr = lb(r);

        // もしlが入っている以降の最初の区間がないなら、[l,r)をそのまま追加
        if(itl == sets.end()){
            sets.insert({l, r});
            return r-l;
        }
        
        // l,rが微妙かぶりしてる場合、変更しても影響ない範囲でl,rを広げておく
        if((*itl).fst <= l) l = (*itl).fst;
        if(itr != sets.end() && (*itr).fst <= r) r = (*itr).snd;
        
        // 左がピッタリ被りになってる時はそれも結合
        // 右は上のifでピッタリ被りも考慮してる
        if(itl != sets.begin() && (*prev(itl)).snd == l){
            itl--;
            l = (*itl).fst;
        }
        
        // まっさらな状態だと追加される区間の長さの合計はr-l
        ll ret = r-l;

        while(true){
            if((*itl).snd <= l || r <= (*itl).fst) break;
            // 「新たに追加区間」から削除区間分の長さを引く
            ret -= ((*itl).snd-(*itl).fst);
            // itlを削除し、次のイテレータにする
            itl = sets.erase(itl);
            if(itl == sets.end()) break;
        }

        sets.insert({l, r});

        return ret;
    }

    // 削除した区間の長さの合計を返す
    ll erase(ll l, ll r){
        auto itl = lb(l);
        
        // 削除区間が、全ての区間より右にあるならやることはない
        if(itl == sets.end()) return 0;

        ll ret = 0;

        // 一旦消した後、追加する部分
        vpll adds;

        // 削除区間の左端が、itlに入っている時
        if((*itl).fst < l){
            adds.pb({(*itl).fst, l});
        }

        while(true){
            if((*itl).snd <= l || r <= (*itl).fst) break;
            if(r < (*itl).snd) adds.pb({r, (*itl).snd});
            
            ret += ((*itl).snd - (*itl).fst);
            itl = sets.erase(itl);
            if(itl == sets.end()) break;
        }

        for(pll ap: adds){
            ret -= (ap.snd - ap.fst);
            sets.insert(ap);
        }

        return ret;
    }

    // 指定した区間と被りがあるか？
    bool have_intersection(pll p){
        auto it = lb(p.fst);
        if(it == sets.end()) return false;
        if((*it).fst <= p.fst) return true;
        if((*it).fst < p.snd) return true;
        return false;
    }

    // したいした要素が区間たちの中に入っているか？
    bool include(ll x){
        return have_intersection({x, x+1});
    }

    // 被ってる区間を全て返す
    vpll intersection(ll l, ll r){
        auto itl = lb(l);
        
        // 削除区間が、全ての区間より右にあるならやることはない
        if(itl == sets.end()) return {};

        vpll ret;

        // 削除区間の左端が、itlに入っている時
        if((*itl).fst < l){
            ret.pb({l, min(r, (*itl).snd)});
            itl++;
            if(itl == sets.end()) return ret;
        }

        while(true){
            if((*itl).snd <= l || r <= (*itl).fst) break;
            if(r < (*itl).snd) ret.pb({(*itl).fst, r});
            else ret.pb(*itl);
            itl++;
            if(itl == sets.end()) break;
        }
        return ret;

    }

};
