#pragma once
/**
 * @brief 区間スケジューリング（互いに重ならない区間を最大個数選ぶ）
 *
 *   interval_scheduling(intervals)         … 選んだ区間の列を返す（端点の接触は OK）  O(N log N)
 *   interval_scheduling(intervals, false)  … 端点の接触も重なりとみなす版
 *
 * @param intervals 区間 {開始, 終了} の列（開始 <= 終了）。値渡しなので呼び出し側は並べ替わらない
 * @param is_pittari_ok true なら「前の終了 == 次の開始」を重ならないとみなす（閉区間で端点共有 OK / 半開区間）
 * @return 選んだ区間（終了の昇順）。個数が答えなら .size()
 * @note 終了が早い順に貪欲に取る。開始・終了は負でもよい
 * @note 偏角などを区間にするときは、比較で同値なもの（同じ偏角）が同じ番号になるよう
 *       coordinate_compression(a, comp) で番号付けすること（ソート後の位置を使うと順序が不定になる）
 *
 * @code
 * vpll iv = {{1, 3}, {3, 5}, {2, 4}};
 * ll a = interval_scheduling(iv).size();         // 2  ({1,3}, {3,5})
 * ll b = interval_scheduling(iv, false).size();  // 1
 * @endcode
 */
vpll interval_scheduling(vpll intervals, bool is_pittari_ok = true){
    sort(all(intervals), [](const pll &a, const pll &b){
        if(a.snd == b.snd) return a.fst < b.fst;
        return a.snd < b.snd;
    });

    vpll ret;
    ll t = -inf;  // 最後に選んだ区間の終了
    for(auto it = intervals.begin(); it != intervals.end(); it++){
        if(is_pittari_ok ? t <= it->fst : t < it->fst){
            ret.pb(*it);
            t = it->snd;
        }
    }
    return ret;
}
