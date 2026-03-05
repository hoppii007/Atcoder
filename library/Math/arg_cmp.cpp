
// arg(p0) < arg(p1)
bool arg_cmp(pll p0, pll p1) {
    // 上半分か？
    function<bool(pll)> is_upper = [](pll p){ return (p.snd > 0 || (p.snd == 0 && p.fst > 0)); };

    if(is_upper(p0) && !is_upper(p1)) return true;
    if(!is_upper(p0) && is_upper(p1)) return false;
    ll cross = p0.fst*p1.snd-p0.snd*p1.fst;
    // 角度が同じ時は常にfalse
    if(cross == 0) return false;
    else return cross > 0;
}
