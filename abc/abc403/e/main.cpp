#include<bits/stdc++.h>
//入力系
#define cinll(...) ll __VA_ARGS__; input(__VA_ARGS__);
#define cinill(...) ll __VA_ARGS__; input_index(__VA_ARGS__);
#define cinint(...) int __VA_ARGS__; input(__VA_ARGS__);
#define ciniint(...) int __VA_ARGS__; input_index(__VA_ARGS__);
#define cinstr(...) string __VA_ARGS__; input(__VA_ARGS__);
#define cinchar(...) char __VA_ARGS__; input(__VA_ARGS__);
#define cinvll(a,n) vll a(n); rep(i,n) cin>>a[i];
#define cinvvll(a,n,m) vvll a(n,vll(m)); rep(i,n) rep(j,m) cin>>a[i][j];
#define cinvs(a,n) vs a(n); rep(i,n) cin>>a[i];
//繰り返し系
#define rep1(n) for(ll i=0;i<n;i++)
#define rep2(i,n) for(ll i=0;i<n;i++)
#define rep3(i,a,n) for(ll i=a;i<n;i++)
#define rep4(i,a,n,b) for(ll i=a;i<n;i+=b)
#define overload4(a,b,c,d,e,...) e
#define rep(...) overload4(__VA_ARGS__,rep4,rep3,rep2,rep1)(__VA_ARGS__)
#define mrep1(n) for(ll i=n;i>=0;i--)
#define mrep2(i,n) for(ll i=n;i>=0;i--)
#define mrep3(i,n,a) for(ll i=n;i>=a;i--)
#define mrep4(i,n,a,b) for(ll i=n;i>=a;i-=b)
#define mrep(...) overload4(__VA_ARGS__,mrep4,mrep3,mrep2,mrep1)(__VA_ARGS__)
//iterator系
#define all(a) a.begin(),a.end()
using namespace std;
//型系
using ll = long long;
using vll = vector<long long>;
using vvll = vector<vector<long long>>;
using vi = vector<int>;
using vvi = vector<vector<int>>;
using vb = vector<bool>;
using vvb = vector<vector<bool>>;
using vc = vector<char>;
using vvc = vector<vector<char>>;
using vs = vector<string>;
using pll = pair<long long,long long>;
using vpll = vector<pair<long long,long long>>;

const ll mod = 998244353LL;
const ll inf = 100100100100100100LL;
const double PI=3.1415926535897932384626433832795028841971;

//便利系
//vectorをjoinするやつ
template<typename T>
string join(const vector<T> &a,string c){
    string ret = "";
    if(a.size()==0) return ret;
    
    //もともとstringなら
    if constexpr (std::is_same_v<T, std::string> || std::is_same_v<T, char>) {
        rep(i,a.size()-1) ret += a[i]+c;
        return ret + a[a.size()-1];
    }
    //もともとstringじゃなければto_stringして結合
    else{
        rep(i,a.size()-1) ret += to_string(a[i]) + c;
        return ret + to_string(a[a.size()-1]);
    }
    
}
//空白区切り表示
template<class T, class... Ts>
void out(const T& a,const Ts&... b){
    cout<<a;
    (cout<< ... << (cout << ' ',b));
    cout<<endl;
}
template<class T>
void out(const vector<T> &a){
    out(join(a," "));
}
//入力
template<class... T>
void input(T&... a){
    (cin >> ... >> a);
}

template<class... T>
void input_index(T&... a){
    (cin >> ... >> a);
    ((a--), ...);
}


//文字列の検索がmax(s_i.size())で可能
//共通接頭辞の数を数えたりできる
class TrieTree{
public:
    ////////////////////////////////
    // 大文字小文字とかの区別はここ //
    ////////////////////////////////
    //mutable型じゃないやつ（stringとか）をconstで初期化しようと思ったらinlineがいるらしい
    static inline const string char_set = "abcdefghijklmnopqrstuvwxyz";
    //string char_set = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    //根っこの文字IDは-1にしとく（そのほかはchar_setのindex）
    static const ll root = -1;

    //Trie木の各頂点
    class Node {
    public:
        vll nextIdx = vll(char_set.size(), -1);      //nextIdx[i] := idがiの文字に行くときのNodeのインデックス（存在しない場合は-1）
        ll charId = -1;           //今のnodeにある文字ID（'a'なら0みたいな）
        ll wordCountFuture = 0;   //この下にいくつ単語があるか(自分も含む)
        ll wordCount = 0;    //ここで止まる単語がいくつあるか

        //自分含め、この下に生きているyがいくつあるか？
        ll aliveY = 0;
        bool isX = false;

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

    //xにprefixがいれば無視
    ll insertX(const string &word){
        bool havePrefix = this->havePrefixX(word);
        ll index = 0;
        nodes[index].wordCountFuture++;
        vll hist;
        rep(i,word.size()){
            ll cid = charToId[word[i]];
            if(nodes[index].nextIdx[cid] == -1){
                nodes[index].nextIdx[cid] = nodes.size();
                index = nodes.size();
                nodes.push_back(Node(cid));
            }
            else{
                index = nodes[index].nextIdx[cid];
            }
            nodes[index].wordCountFuture++;
            hist.push_back(index);
        }
        nodes[index].wordCount++;
        nodes[index].isX = true;
        if(havePrefix) return 0;
        ll ret = nodes[index].aliveY;
        mrep(i, (ll)hist.size()-1) nodes[hist[i]].aliveY -= ret;
        return ret;
    }

    //yを追加するときは、xにprefixがあるかどうか？
    //prefixがあるなら、そのprefixの一番上のやつが消してる個数を一つ増やす
    void insertY(const string &word){
        bool havePrefix = this->havePrefixX(word);
        ll index = 0;
        nodes[index].wordCountFuture++;
        if(havePrefix == false) nodes[index].aliveY++;
        for(char c: word){
            ll cid = charToId[c];
            if(nodes[index].nextIdx[cid] == -1){
                nodes[index].nextIdx[cid] = nodes.size();
                index = nodes.size();
                nodes.push_back(Node(cid));
            }
            else{
                index = nodes[index].nextIdx[cid];
            }
            nodes[index].wordCountFuture++;
            if(havePrefix == false) nodes[index].aliveY++;
        }
        nodes[index].wordCount++;
    }
    
    //wordがあるか？
    bool haveWord(const string &word){
        ll index = 0;
        rep(i,word.size()){
            ll cid = charToId[word[i]];
            if(nodes[index].nextIdx[cid] == -1) return false;
            index = nodes[index].nextIdx[cid];
        }
        return nodes[index].wordCount > 0;
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

    // xの要素をprefixに持つかだけ
    bool havePrefixX(const string &word){
        ll index = 0;
        rep(i,word.size()){
            ll cid = charToId[word[i]];
            if(nodes[index].nextIdx[cid] == -1) return false;
            index = nodes[index].nextIdx[cid];
            if(nodes[index].isX) return true;
        }
        return false;
    }

};


int main(void){
    cinll(q);
    TrieTree x,y;
    ll del = 0;
    ll yc = 0;
    while(q--){
        cinll(t);
        if(t == 1){
            cinstr(s);
            //if(s == "j") cout<<"ngo"<<endl;
            //Xにこれのprefixがいた時はそいつが消してるはずだから何もしない
            del += x.insertX(s);
        }
        else{
            cinstr(s);
            yc++;
            //もしxの要素であるようなsのprefixが存在したら
            if(x.havePrefixX(s) > 0){
                del++;
            }
            x.insertY(s);
        }
        cout<<yc-del<<endl;

    }
    return 0;
}
