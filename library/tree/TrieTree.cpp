//文字列の検索がmax(s_i.size())で可能
//共通接頭辞の数を数えたりできる
//char_size:今回考える文字が全部で何個あるか（普通26）
//base : idが0の文字は何か
//例：A ~ Zを使う場合 : TrieTree<26,'A'>
template<ll char_size, ll base>
class TrieTree{
public:
    class Node {
    public:
        vll nexts;      //nexts[i] := idがiの文字に行くときのNodeのインデックス
        ll c;           //baseからの距離（文字ID的な）
        ll wordCount;   //この下にいくつ単語があるか(自分も含む)
        bool isWord;    //ここで止まる単語があるか
        Node(ll c,bool w){
            nexts = vll(char_size,-1);
            isWord = w;
            wordCount = 0;
            this->c = c;
        }
        Node(){
            nexts = vll(char_size,-1);
            isWord = false;
            wordCount = 0;
            this->c = -1;
        }
    };
    
    vector<Node> nodes;
    ll root = -1;
    TrieTree(){
        nodes.push_back(Node(root,false));
    }
    
    ll getId(ll c){
        return c-base;
    }
    
    void insert(const string &word){
        ll index = 0;
        Node now;
        rep(i,word.size()){
            ll id = getId(word[i]);
            if(nodes[index].nexts[id] == -1){
                nodes[index].nexts[id] = nodes.size();
                index = nodes.size();
                nodes.push_back(Node(id,false));
            }
            else{
                index = nodes[index].nexts[id];
            }
            nodes[index].wordCount++;
        }
        nodes[index].isWord = true;
    }
    
    bool find(const string &word){
        ll index = 0;
        rep(i,word.size()){
            ll id = getId(word[i]);
            if(nodes[index].nexts[id] == -1) return false;
            index = nodes[index].nexts[id];
        }
        return nodes[index].isWord;
    }
    
    ll countPrefix(const string &word){
        ll index = 0;
        rep(i,word.size()){
            ll id = getId(word[i]);
            if(nodes[index].nexts[id] == -1) return 0;
            index = nodes[index].nexts[id];
        }
        return nodes[index].wordCount;
    }
};