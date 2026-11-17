#pragma once
/**
 * @brief 接尾辞配列（Suffix Array）と LCP 配列
 *
 *   suffix_array(s)  … sa[i] := 辞書順で i 番目の接尾辞の開始位置（サイズ |s|）  O(N log N)
 *   lcp_array(s, sa) … lcp[i] := 接尾辞 sa[i] と sa[i+1] の最長共通接頭辞長（サイズ |s|-1）  O(N)
 *
 * @note s は文字コード 0, 1 の文字を含まないこと（番兵に使う）
 * @note 接尾辞 sa[a], sa[b] (a < b) の LCP は min(lcp[a..b-1])
 *
 * @code
 * string s = "abracadabra";
 * vll sa = suffix_array(s);
 * vll lcp = lcp_array(s, sa);
 * @endcode
 */
vll suffix_array(string s){
    s += (char)1;              // 番兵（文字列中の任意の文字より小さいと仮定）
    ll n = s.size();
    ll alphabet = 256;
    vll p(n), c(n), cnt(max(alphabet, n), 0);   // p:順位順の位置, c:各位置の順位クラス

    // --- 初期ソート：先頭1文字での順位を計数ソートで求める ---
    rep(i,n) cnt[(unsigned char)s[i]]++;
    rep(i,1,alphabet) cnt[i] += cnt[i-1];
    rep(i,n) p[--cnt[(unsigned char)s[i]]] = i;
    c[p[0]] = 0;
    ll classes = 1;            // 現在の異なる順位クラス数
    rep(i,1,n){
        if(s[p[i]] != s[p[i-1]]) classes++;
        c[p[i]] = classes - 1;
    }

    // --- ダブリング：2^k 文字 → 2^(k+1) 文字 へ順位を更新 ---
    vll pn(n), cn(n);
    for(ll h=0; (1LL<<h) < n; h++){
        // pn[i] := 「後半 2^h 文字始まりが p[i]」となる前半の開始位置（第2キーで既に整列済み）
        rep(i,n){
            pn[i] = p[i] - (1LL<<h);
            if(pn[i] < 0) pn[i] += n;
        }
        // 第1キー c[pn[i]] で安定な計数ソート
        fill(cnt.begin(), cnt.begin()+classes, 0);
        rep(i,n) cnt[c[pn[i]]]++;
        rep(i,1,classes) cnt[i] += cnt[i-1];
        mrep(i,n-1,0) p[--cnt[c[pn[i]]]] = pn[i];
        // ペア (c[p[i]], c[p[i]+2^h]) で新しい順位クラスを振り直す
        cn[p[0]] = 0;
        classes = 1;
        rep(i,1,n){
            pll cur  = {c[p[i]],   c[(p[i]  +(1LL<<h))%n]};
            pll prev = {c[p[i-1]], c[(p[i-1]+(1LL<<h))%n]};
            if(cur != prev) classes++;
            cn[p[i]] = classes - 1;
        }
        c.swap(cn);
        if(classes == n) break; // 全順位が確定したら打ち切り
    }

    return vll(p.begin()+1, p.end());   // 先頭は番兵始まりの回転なので捨てる
}

vll lcp_array(const string &s, const vll &sa){
    ll n = s.size();
    if(n <= 1) return vll();
    vll rnk(n);                       // rnk[i] := 接尾辞 i が sa 上で何番目か
    rep(i,n) rnk[sa[i]] = i;
    vll lcp(n-1);
    ll h = 0;
    rep(i,n){
        if(rnk[i] == 0){ h = 0; continue; } // sa 内で最小の接尾辞は前が無い
        ll j = sa[rnk[i]-1];          // 辞書順で1つ前の接尾辞の開始位置
        while(i+h < n && j+h < n && s[i+h] == s[j+h]) h++;
        lcp[rnk[i]-1] = h;
        if(h > 0) h--;                // 次の i では先頭1文字ぶん減るだけ
    }
    return lcp;
}
