#pragma once
#include <cassert>
/**
 * @brief Trie 木（単語の個数・接頭辞の個数を数える）
 *
 *   trie_tree tr                  … 空の Trie
 *   tr.insert(w)                 … 単語 w を追加  O(|w|)
 *   tr.size()                    … 追加した単語の総数  O(1)
 *   tr.count_word(w)              … w と一致する単語の個数  O(|w|)
 *   tr.count_word_future(w)        … w を接頭辞にもつ単語の個数（w 自身を含む）  O(|w|)
 *   tr.count_word_future(w, false) … 同上（w 自身を除く）
 *   tr.count_prefix(w)            … w の接頭辞（w 自身を含む）になっている単語の個数  O(|w|)
 *   tr.count_prefix(w, false)     … 同上（同じ単語の重複は 1 個と数える）
 *
 * @note 文字は char_set（既定は英小文字）に含まれること（含まれない文字は assert で停止）。英大文字なら char_set を差し替える
 *
 * @code
 * trie_tree tr;
 * tr.insert("at");
 * tr.insert("atcoder");
 * ll a = tr.count_word_future("at");     // 2
 * ll b = tr.count_prefix("atcoder");    // 2
 * @endcode
 */
class trie_tree{
public:
    static inline const string char_set = "abcdefghijklmnopqrstuvwxyz";
    //static inline const string char_set = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    static const ll root = -1;

    class node {
    public:
        vll next_idx = vll(char_set.size(), -1);
        ll char_id = -1;
        ll word_count_future = 0;
        ll word_count = 0;

        node(ll c){ char_id = c; }
        node(){}
    };
    
    
    vector<node> nodes;
    map<char,ll> char_to_id;

    trie_tree(){
        nodes.push_back(node(root));
        rep(i,char_set.size()) char_to_id[char_set[i]] = i;
    }

    ll char_id(char c){
        auto it = char_to_id.find(c);
        assert(it != char_to_id.end());  // char_set に無い文字
        return it->second;
    }
    
    void insert(const string &word){
        //rootは必ず0番目になる
        ll index = 0;

        //rootについてもFutureをカウントしとく（全体でのsizeを出すため）
        nodes[index].word_count_future++;

        rep(i,word.size()){
            ll cid = char_id(word[i]);
            //今のindexから文字cidに行くパスがなければ追加する
            if(nodes[index].next_idx[cid] == -1){
                nodes[index].next_idx[cid] = nodes.size();
                index = nodes.size();
                nodes.push_back(node(cid));
            }
            //cidに行ければ行く
            else{
                index = nodes[index].next_idx[cid];
            }
            //ここを通ったということは、この先にあるということ
            nodes[index].word_count_future++;
        }
        nodes[index].word_count++;
    }

    ll size(){
        return nodes[0].word_count_future;
    }

    ll count_word(const string &word){
        ll index = 0;
        rep(i,word.size()){
            ll cid = char_id(word[i]);
            if(nodes[index].next_idx[cid] == -1) return 0;
            index = nodes[index].next_idx[cid];
        }
        return nodes[index].word_count;
    }

    ll count_word_future(const string &word, bool include_me=true){
        ll index = 0;
        rep(i,word.size()){
            ll cid = char_id(word[i]);
            if(nodes[index].next_idx[cid] == -1) return 0;
            index = nodes[index].next_idx[cid];
        }
        if(include_me) return nodes[index].word_count_future;
        else return nodes[index].word_count_future - nodes[index].word_count;
    }

    ll count_prefix(const string &word, bool is_count_duplication=true) {
        ll index = 0;
        ll ret = 0;
        rep(i,word.size()){
            ll cid = char_id(word[i]);
            if(nodes[index].next_idx[cid] == -1) return ret;
            index = nodes[index].next_idx[cid];
            if(is_count_duplication) ret += nodes[index].word_count;
            else ret += (nodes[index].word_count>0?1:0);
        }
        return ret;
    }
};
