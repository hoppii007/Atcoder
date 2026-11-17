#pragma once
#include <cassert>
/**
 * @brief Trie 木（単語の個数・接頭辞の個数を数える）
 *
 *   TrieTree tr                  … 空の Trie
 *   tr.insert(w)                 … 単語 w を追加  O(|w|)
 *   tr.size()                    … 追加した単語の総数  O(1)
 *   tr.countWord(w)              … w と一致する単語の個数  O(|w|)
 *   tr.countWordFuture(w)        … w を接頭辞にもつ単語の個数（w 自身を含む）  O(|w|)
 *   tr.countWordFuture(w, false) … 同上（w 自身を除く）
 *   tr.countPrefix(w)            … w の接頭辞（w 自身を含む）になっている単語の個数  O(|w|)
 *   tr.countPrefix(w, false)     … 同上（同じ単語の重複は 1 個と数える）
 *
 * @note 文字は char_set（既定は英小文字）に含まれること（含まれない文字は assert で停止）。英大文字なら char_set を差し替える
 *
 * @code
 * TrieTree tr;
 * tr.insert("at");
 * tr.insert("atcoder");
 * ll a = tr.countWordFuture("at");     // 2
 * ll b = tr.countPrefix("atcoder");    // 2
 * @endcode
 */
class TrieTree{
public:
    static inline const string char_set = "abcdefghijklmnopqrstuvwxyz";
    //static inline const string char_set = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    static const ll root = -1;

    class Node {
    public:
        vll nextIdx = vll(char_set.size(), -1);
        ll charId = -1;
        ll wordCountFuture = 0;
        ll wordCount = 0;

        Node(ll c){ charId = c; }
        Node(){}
    };
    
    
    vector<Node> nodes;
    map<char,ll> charToId;

    TrieTree(){
        nodes.push_back(Node(root));
        rep(i,char_set.size()) charToId[char_set[i]] = i;
    }

    ll char_id(char c){
        auto it = charToId.find(c);
        assert(it != charToId.end());  // char_set に無い文字
        return it->second;
    }
    
    void insert(const string &word){
        //rootは必ず0番目になる
        ll index = 0;

        //rootについてもFutureをカウントしとく（全体でのsizeを出すため）
        nodes[index].wordCountFuture++;

        rep(i,word.size()){
            ll cid = char_id(word[i]);
            //今のindexから文字cidに行くパスがなければ追加する
            if(nodes[index].nextIdx[cid] == -1){
                nodes[index].nextIdx[cid] = nodes.size();
                index = nodes.size();
                nodes.push_back(Node(cid));
            }
            //cidに行ければ行く
            else{
                index = nodes[index].nextIdx[cid];
            }
            //ここを通ったということは、この先にあるということ
            nodes[index].wordCountFuture++;
        }
        nodes[index].wordCount++;
    }

    ll size(){
        return nodes[0].wordCountFuture;
    }

    ll countWord(const string &word){
        ll index = 0;
        rep(i,word.size()){
            ll cid = char_id(word[i]);
            if(nodes[index].nextIdx[cid] == -1) return 0;
            index = nodes[index].nextIdx[cid];
        }
        return nodes[index].wordCount;
    }

    ll countWordFuture(const string &word, bool includeMe=true){
        ll index = 0;
        rep(i,word.size()){
            ll cid = char_id(word[i]);
            if(nodes[index].nextIdx[cid] == -1) return 0;
            index = nodes[index].nextIdx[cid];
        }
        if(includeMe) return nodes[index].wordCountFuture;
        else return nodes[index].wordCountFuture - nodes[index].wordCount;
    }

    ll countPrefix(const string &word, bool isCountDuplication=true) {
        ll index = 0;
        ll ret = 0;
        rep(i,word.size()){
            ll cid = char_id(word[i]);
            if(nodes[index].nextIdx[cid] == -1) return ret;
            index = nodes[index].nextIdx[cid];
            if(isCountDuplication) ret += nodes[index].wordCount;
            else ret += (nodes[index].wordCount>0?1:0);
        }
        return ret;
    }
};
