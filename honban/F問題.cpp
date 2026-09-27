// 考察
// 初め、全員にアイスクリームを配ったことにしておいて、「チョコに変更したら得する人はチョコに切り替える」ということを考える
// 注意点は 2 つで、切り替えるのは「出来るだけ得になる数値が大きい順」に切り替えることと
// アイスとチョコの個数の制約を考慮すること
// 答えが大きくなりすぎるので、int ではなく long long を使う

#include <iostream>
#include <vector>
#include <algorithm> // 並び替えに必要
using namespace std;

int main(){
	long long n, x, y;
	cin >> n >> x >> y;
	vector<long long> a(n);
	vector<long long> b(n);
	for(int i=0;i<n;i++){
		cin >> a[i];
	}
	for(int i=0;i<n;i++){
		cin >> b[i];
	}

	// 全てアイスだった時の答え
	long long ans = 0;
	for(int i=0;i<n;i++){
		ans += a[i];
	}

	// アイスからチョコに切り替えた時の「お得度」を計算しておく
	vector<int> c(n);
	for(int i=0;i<n;i++){
		c[i] = b[i]-a[i];
	}

	// お得度が大きい順に並べ替える
	sort(c.rbegin(), c.rend());

	// アイスは X 個しかないので、N-X個は強制的にチョコにしておく
	int i = 0;
	while(i < n-x){
		ans += c[i];
		i++;
	}

	// その後、チョコが残っていてかつお得度がプラスの間、チョコに切り替え続ける
	while(i < y && c[i] > 0){
		ans += c[i];
		i++;
	}

	cout << ans << endl;

	return 0;
}