#pragma once
/**
 * @brief 文字列の分割（区切り文字 1 文字）
 *
 *   split(s, c) … s を c で区切った vs  O(|s|)
 *
 * @note 空の要素も残す（"a,,b," → {"a", "", "b", ""}、"" → {""}）
 *
 * @code
 * vs v = split("a,b,,c", ',');  // {"a", "b", "", "c"}
 * @endcode
 */
vs split(string s, char c){
    vs ret;
    s += string(1,c);  // 末尾要素を確実に push するための番兵
    string pre = "";
    rep(i, s.size()){
        if(s[i] == c){
            ret.pb(pre);
            pre = "";
        }
        else{
            pre += string(1, s[i]);
        }
    }
    return ret;
}
