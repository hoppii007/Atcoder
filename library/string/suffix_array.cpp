/**
 * @brief 接尾辞配列（Suffix Array）をダブリング法で構築する
 *
 * 文字列 s の全接尾辞を辞書順にソートした「開始位置の配列」を返す。
 * 各接尾辞を先頭 2^k 文字での順位(rank)で表し、k を 0,1,2,... と倍にしながら
 * ペア (rank[i], rank[i + 2^k]) で並べ替えることで、文字列比較を整数比較に落とす。
 * 先頭 2^(k+1) 文字 = [前半 2^k 文字][後半 2^k 文字] なので、このペアが順位を完全に決める。
 * 各ラウンドの並べ替えを計数ソート(counting sort)で行うため O(N log N)。
 *
 * 実装上は末尾に番兵(全文字より小さい値)を足した巡回シフトを並べ替え、
 * 先頭(番兵始まりの回転)を捨てることで接尾辞配列を得る（番兵が文字列終端の役割を果たす）。
 *
 * @param s 対象文字列。番兵より小さい文字を含まないこと（小文字英字などを想定）。
 * @return sa。sa[i] := 辞書順で i 番目に小さい接尾辞の開始位置（0-indexed, サイズ |s|）。
 * @note 計算量 O(N log N)（N = |s|）
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

/**
 * @brief LCP配列（隣接接尾辞の最長共通接頭辞長）を Kasai 法で構築する
 *
 * lcp[i] := sa[i] の接尾辞と sa[i+1] の接尾辞の最長共通接頭辞(LCP)の長さ（サイズ |s|-1）。
 * 辞書順で1つ前の接尾辞との LCP を、元の位置 i を昇順に走査しながら求める。
 * i を1つ増やすと LCP は高々1しか減らない（先頭1文字ぶんずれるだけ）という性質を使い、
 * h を持ち越すことで全体 O(N) で計算する。
 *
 * 任意の2接尾辞 sa[a], sa[b] (a<b) の LCP は min(lcp[a..b-1]) で得られる（区間min）。
 *
 * @param s  対象文字列
 * @param sa suffix_array(s) の結果
 * @return lcp。lcp[i] := LCP(sa[i], sa[i+1])（サイズ |s|-1）。|s|<=1 のときは空。
 * @note 計算量 O(N)（N = |s|）
 */
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
