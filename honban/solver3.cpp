#include <bits/stdc++.h>
using namespace std;
using ll = long long;


// 制約 : n <= 100
// 解法
// ・「JOI」を見つけて「OIJ」に置き換えていく
// ・一回も置き換えなかったら終了
void bubun1(ll n, string s){

    // 置き換えれるように配列にしておく
    vector<char> cs(n);
    for(int i=0;i<n;i++){
        cs[i] = s[i];
    }

    // 「JOI」を見つけて置き換えていく
    while(true){
        bool have_joi = false;
        for(int i=0;i<n-2;i++){
            if(cs[i] == 'J' && cs[i+1] == 'O' && cs[i+2] == 'I'){
                have_joi = true;
                cs[i] = 'O';
                cs[i+1] = 'I';
                cs[i+2] = 'J';
            }
        }
        if(have_joi == false) break;
    }

    // 答えを表示
    for(int i=0;i<n;i++){
        cout << cs[i];
    }
    cout << endl;
}

// 制約：n%3 == 0 で、「JOI」がn/3個並んでる
// 解法
// ・「JOIJOIJOI...」は、「OIOIOI...JJJ...」となる
// ・「J」と「OI」の数を数えればいい（n/3個）
void bubun2(ll n, string s){
    for(int i=0;i<n/3;i++){
        cout << "OI";
    }
    for(int i=0;i<n/3;i++){
        cout << "J";
    }
    cout << endl;
}

// 制約：JJJ...OIOIIOIOのように、先頭にJが並んでいる
// 解法
// ・JJJ...を除いたうしろ「OIOIIOIO」の部分を考えて、この中で「J塊」がどこまで移動できるかを考える
// ・これは、「OI」となっていない「I」を見つけたらそこで途切れる。
// ・今回だと、「OIOII」の最後のIで途切れる
// ・この、最初の「I」までの文字列を、「有効文字列」と呼ぶことにする

// 有効文字列をもらって、そいつをいい感じに処理した後の文字列を返す
string f(string t){
    string ret = "";
    
    // 有効文字列の中のJの数をカウント
    ll cnt_j = 0;
    for(int i=0;i<t.size();i++){
        if(t[i] == 'J') cnt_j++;
    }
    
    // 有効文字列の最後の「I」を除いた中で、J以外を表示
    for(int i=0;i<t.size()-1;i++){
        if(t[i] != 'J') ret += string(1, t[i]);
    }
    // 有効文字列の中のJをすべて表示
    for(int i=0;i<cnt_j;i++){
        ret += string(1, t[i]);
    }
    // 有効文字列の最後の「I」を表示
    ret += "I";
    
    return ret;
}
void bubun3(ll n, string s){
    // 有効文字列を切り出す
    string t;
    for(int i=0;i<n;i++){
        t += string(1, s[i]);
        if(s[i] == 'I' && (i==0 || s[i-1] != 'O')) break;
    }

    cout << f(t) << endl;
}

// 満点解法
// 「有効文字列がたくさん並んでいる」と解釈すればいい
void solve(ll n, string s){
    vector<string> yukos;

    // 有効文字列ごとに区切る
    string tmp = "";
    for(int i=0;i<n;i++){
        tmp += string(1, s[i]);

        // 有効文字列の区切り
        if(s[i] == 'I' && (i==0 || s[i-1] != 'O')){
            yukos.push_back(tmp);
            tmp = "";
        }
    }

    // 各有効文字列ごとに処理して、順番に表示すればいい
    for(int i=0;i<yukos.size();i++){
        cout << f(yukos[i]);
    }
    cout << endl;
}

int main(void){
    ll n;
    string s;
    cin >> n >> s;

    if(n <= 100){
        bubun3(n, s);
    }
}
