#pragma once
/**
 * @brief 区間の集合（半開区間 [l, r) を set で管理し、重なり・隣接は自動で統合）
 *
 *   IntervalSet is                … 空集合で初期化
 *   is.insert(l, r)               … [l, r) を追加し、新たに覆われた長さを返す  ならし O(log N)
 *   is.erase(l, r)                … [l, r) を削除し、削除された長さを返す      ならし O(log N)
 *   is.include(x)                 … x がどれかの区間に含まれるか  O(log N)
 *   is.have_intersection({l, r})  … [l, r) と重なる区間があるか   O(log N)
 *   is.intersection(l, r)         … [l, r) と重なる部分を [l, r) で切り取って列挙
 *   is.lb(x)                      … x を含む区間、無ければ x より右の最初の区間のイテレータ
 *   is.sets                       … 区間 {l, r} の set
 *
 * @code
 * IntervalSet is;
 * is.insert(1, 5);            // 4
 * is.insert(3, 8);            // 3   （[1, 8) に統合）
 * is.erase(2, 4);             // 2   （[1, 2), [4, 8)）
 * bool b = is.include(3);     // false
 * vpll v = is.intersection(0, 5);  // {{1, 2}, {4, 5}}
 * @endcode
 */
struct IntervalSet{
    set<pll> sets = {};

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

    bool have_intersection(pll p){
        auto it = lb(p.fst);
        if(it == sets.end()) return false;
        if((*it).fst <= p.fst) return true;
        if((*it).fst < p.snd) return true;
        return false;
    }

    bool include(ll x){
        return have_intersection({x, x+1});
    }

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
