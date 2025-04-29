
//文字列の検索がmax(s_i.size())で可能
//共通接頭辞の数を数えたりできる
class TrieTree{
public:
    ////////////////////////////////
    // 大文字小文字とかの区別はここ //
    ////////////////////////////////
    //mutable型じゃないやつ（stringとか）をconstで初期化しようと思ったらinlineがいるらしい
    static inline const string char_set = "abcdefghijklmnopqrstuvwxyz";
    //static inline const string char_set = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    //根っこの文字IDは-1にしとく（そのほかはchar_setのindex）
    static const ll root = -1;

    //Trie木の各頂点
    class Node {
    public:
        vll nextIdx = vll(char_set.size(), -1);      //nextIdx[i] := idがiの文字に行くときのNodeのインデックス（存在しない場合は-1）
        ll charId = -1;           //今のnodeにある文字ID（'a'なら0みたいな）
        ll wordCountFuture = 0;   //この下にいくつ単語があるか(自分も含む)
        ll wordCount = 0;    //ここで止まる単語がいくつあるか

        Node(ll c){ charId = c; }
        Node(){}
    };
    
    
    vector<Node> nodes;     // 頂点集合
    map<char,ll> charToId;  // 各文字をIDに変換する

    //rootの追加と、charToIdの設定
    TrieTree(){
        nodes.push_back(Node(root));
        rep(i,char_set.size()) charToId[char_set[i]] = i;
    }
    
    //単語の追加
    void insert(const string &word){
        //rootは必ず0番目になる
        ll index = 0;

        //rootについてもFutureをカウントしとく（全体でのsizeを出すため）
        nodes[index].wordCountFuture++;

        rep(i,word.size()){
            ll cid = charToId[word[i]];
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

    //全部で単語がいくつあるか？
    ll size(){
        return nodes[0].wordCountFuture;
    }

    //この単語がいくつあるか？
    ll countWord(const string &word){
        ll index = 0;
        rep(i,word.size()){
            ll cid = charToId[word[i]];
            if(nodes[index].nextIdx[cid] == -1) return 0;
            index = nodes[index].nextIdx[cid];
        }
        return nodes[index].wordCount;
    }

    //これをprefixにもつ単語がいくつあるか？（自分含めて）
    ll countWordFuture(const string &word, bool includeMe=true){
        ll index = 0;
        rep(i,word.size()){
            ll cid = charToId[word[i]];
            if(nodes[index].nextIdx[cid] == -1) return 0;
            index = nodes[index].nextIdx[cid];
        }
        if(includeMe) return nodes[index].wordCountFuture;
        else return nodes[index].wordCountFuture - nodes[index].wordCount;
    }

    //この文字列の接頭辞がいくつあるか？（自分含め）
    //atcoderに対してatやatcなどがいくつあるか？（デフォルトは重複も数える）
    ll countPrefix(const string &word, bool isCountDuplication=true) {
        ll index = 0;
        ll ret = 0;
        rep(i,word.size()){
            ll cid = charToId[word[i]];
            if(nodes[index].nextIdx[cid] == -1) return ret;
            index = nodes[index].nextIdx[cid];
            if(isCountDuplication) ret += nodes[index].wordCount;
            else ret += (nodes[index].wordCount>0?1:0);
        }
        return ret;
    }
};
