/**
 * @brief 座標圧縮
 * @details 与えられた座標列を、値の大小関係を保ったまま 0-indexed の連番に変換する。
 *          重複する値は同一の圧縮後座標に対応する。
 * @param a 圧縮対象の座標列（コピー渡し）
 * @return 元の座標 → 圧縮後座標（0-indexed）の map
 * @note 計算量: O(N log N)
 */
map<ll,ll> coordinate_compression(vll a){
    sort(all(a));
    a.erase(unique(all(a)), a.end());
    map<ll,ll> za;
    rep(zaatui, a.size()) za[a[zaatui]] = zaatui;
    return za;
}