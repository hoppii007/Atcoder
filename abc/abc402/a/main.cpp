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
		ll del;
		ll cx, cy;
		bool haveX,haveY;
        Node(ll c,bool w){
            nexts = vll(char_size,-1);
            isWord = w;
            wordCount = 0;
            this->c = c;
			del = 0;
			cx = cy = 0;
			haveX = haveY = false;
        }
        Node(){
            nexts = vll(char_size,-1);
            isWord = false;
            wordCount = 0;
            this->c = -1;
			cx = cy = 0;
			haveX = haveY = false;
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
    
    void insert(const string &word, bool isX, bool isY){
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
			if(isX) nodes[index].cx++;
			if(isY) nodes[index].cy++;
        }
        nodes[index].isWord = true;
		if(isX) nodes[index].haveX = true;
		if(isY) nodes[index].haveY = true;
    }
    
    bool find(const string &word){
		//0番目は絶対root(-1)が入ってる
        ll index = 0;
        rep(i,word.size()){
            ll id = getId(word[i]);
            if(nodes[index].nexts[id] == -1) return false;
            index = nodes[index].nexts[id];
        }
        return nodes[index].isWord;
    }

	//wordのprefixが存在するか？
	bool findPrefix(const string &word){
		ll index = 0;
        rep(i,word.size()){
            ll id = getId(word[i]);
            if(nodes[index].nexts[id] == -1) return false;
            index = nodes[index].nexts[id];
			if(nodes[index].isWord) return true;
        }
        return nodes[index].isWord;
	}

	//wordのprefixみんなのやつが1増える
	void setDel(const string &word){
		ll index = 0;
		rep(i,word.size()){
			ll id = getId(word[i]);
			if(nodes[index].nexts[id] == -1) return;
			index = nodes[index].nexts[id];
			nodes[index].del++;
		}
	}

	ll getNowDel(const string &word){
		ll index = 0;
		rep(i,word.size()){
			ll id = getId(word[i]);
			if(nodes[index].nexts[id] == -1) return;
			index = nodes[index].nexts[id];
		}
		return nodes[index].del;
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

	ll countYPrefix(const string &word){
		ll index = 0;
        rep(i,word.size()){
            ll id = getId(word[i]);
            if(nodes[index].nexts[id] == -1) return 0;
            index = nodes[index].nexts[id];
        }
        return nodes[index].cy;
	}
	ll countXPrefix(const string &word){
		ll index = 0;
        rep(i,word.size()){
            ll id = getId(word[i]);
            if(nodes[index].nexts[id] == -1) return 0;
            index = nodes[index].nexts[id];
        }
        return nodes[index].cx;
	}
	ll countChildYPrefix(const string &word){
		ll index = 0;
        rep(i,word.size()){
            ll id = getId(word[i]);
            if(nodes[index].nexts[id] == -1) return 0;
            index = nodes[index].nexts[id];
        }
		ll ret = 0;
		for(ll next: nodes[index].nexts) if(next != -1) {
			ret += nodes[next].cy;
		}
        return ret;
	}
	bool findXPrefix(const string &word){
		ll index = 0;
		rep(i,word.size()){
			ll id = getId(word[i]);
			if(nodes[index].nexts[id] == -1) return false;
			index = nodes[index].nexts[id];
		}
		return nodes[index].haveX;
	}

};


int main(void){
	cinll(q);
	TrieTree<26, 'a'> x,y;
	ll del = 0;
	ll yc = 0;
	while(q--){
		cinll(t);
		if(t == 1){
			cinstr(s);
			//これのprefixがいた時はそいつが消してるはずだから何もしない
			if(x.findPrefix(s)) continue;
			//sが誰かのprefixだったときは、消せてないやつを消さないと
			else if(x.countPrefix(s) > 0){
				del += x.countYPrefix(s) - x.countChildYPrefix(s);
			}
			else{
				del += x.countYPrefix(s);
			}
			x.insert(s,true,false);
		}
		else{
			cinstr(s);
			yc++;
			if(x.findXPrefix(s)){
				del++;
			}
			y.insert(s,false,true);
		}
		cout<<yc-del<<endl;

	}
    return 0;
}
