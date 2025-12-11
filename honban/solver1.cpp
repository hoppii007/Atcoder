
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using vll = vector<ll>;

// 制約 : n = 3
// 解法
// ・1人と2人で分けるのが最小。
// ・一番高い点数をラインとすればいい（一人しかいなければ上クラス一人になるし、2人いる場合は上クラスを二人にするしかない）
void bubun1(ll n, vll &a){
    sort(a.begin(), a.end());
    cout << *a.rbegin() << endl;
}

// 制約 : a[i] == 500 || a[i] == 800 || a[i] == 1000
// 解法
// ・三種類しかないから、ラインにすべきは800か1000のどれか。
// 　500にしたり1001以上にしたりしたら全員が同じクラスになる
// 　510とか810とかにするなら、800とか1000にしちゃえばいい
void bubun2(ll n, vll &a){
    // ラインを800にする場合
    // 800未満の人数と、800以上の人数を計算
    ll ijo_800 = 0;
    ll miman_800 = 0;
    for(int i=0;i<a.size();i++){
        if(a[i] >= 800) ijo_800++;
        else miman_800++;
    }
    // ライン800でのスコア
    ll score_800 = abs(ijo_800 - miman_800);

    // ライン1000にする場合
    ll ijo_1000 = 0;
    ll miman_1000 = 0;
    for(int i=0;i<a.size();i++){
        if(a[i] >= 1000) ijo_1000++;
        else miman_1000++;
    }
    ll score_1000 = abs(ijo_1000 - miman_1000);

    //スコアに差があるなら、そこを採用
    if(score_1000 < score_800){
        cout << 1000 << endl;
    }
    else if(score_800 < score_1000){
        cout << 800 << endl;
    }
    // スコアに差がないなら、進学クラスの人数が少なくなる方
    else{
        if(ijo_1000 < ijo_800) cout << 1000 << endl;
        else cout << 800 << endl;
    }
}

// 制約 : a[i] != a[j]
// 解法
// ・答えの種類がn種類あるから、ラインの候補は最低点以外のすべて
// ・全ての点に対して、それ以上の人数と、それ以下の人数がすぐわかる状態になればいい
// ・これは、ソートしておくとわかる
void bubun3(ll n, vll &a){
    sort(a.begin(), a.end());

    ll ans_score = 10000000000LL;
    ll ans_ijo = 10000000000LL;
    ll ans = -1;

    // 答えになりうる全てのラインを試す
    for(int i=1;i<a.size();i++){
        // a[i]未満の人数はi人
        ll miman = i;
        ll ijo = n - miman;
        ll score = abs(miman-ijo);

        if(score < ans_score){
            ans_score = score;
            ans_ijo = ijo;
            ans = a[i];
        }
        else if(score == ans_score && ijo < ans_ijo){
            ans_score = score;
            ans_ijo = ijo;
            ans = a[i];
        }
    }

    cout << ans << endl;
}

// 満点解法
// ・今回も答えのラインとしてあり得るのはaに出てくるやつら
// ・つまり、各点数に対してそれ未満の人数とそれ以上の人数がパッと出せればいける
// ・これは、aをソートしておいて、「同じ数字に対しては最初の場所だけを考える」といい
void solve(ll n, vll &a){
    sort(a.begin(), a.end());

    ll ans_score = 10000000000LL;
    ll ans_ijo = 10000000000LL;
    ll ans = -1;

    for(int i=0;i<n;i++){
        ll miman = i;
        ll ijo = n - miman;
        ll score = abs(miman - ijo);
        
        if(score < ans_score){
            ans_score = score;
            ans_ijo = ijo;
            ans = a[i];
        }
        else if(score == ans_score && ijo < ans_ijo){
            ans_score = score;
            ans_ijo = ijo;
            ans = a[i];
        }

        // 同じ間はスキップ
        while(i+1 < n && a[i] == a[i+1]) i++;
    }

    cout << ans << endl;
}

int main(){
    ll n;
    cin >> n;

    vll a(n);
    for(int i=0;i<n;i++) cin >> a[i];
    return 0;
}