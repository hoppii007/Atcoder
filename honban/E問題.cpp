// 考察
// 「JOI, IOI, JOIG の組み合わせでSが作れるか？」と読み替えられるので、先頭から順次チェックしていけばいい

#include <iostream>
using namespace std;

int main(){
	string s;
	cin >> s;

	for(int i=0;i<s.size();i++){
		// 先頭が JOIG になっていれば続ける ※JOIのチェックを先に書くとダメなので注意
		if(i+3 < s.size() && s[i] == 'J' && s[i+1] == 'O' && s[i+2] == 'I' && s[i+3] == 'G'){
			// JOIG の G までチェックしたことにする
			i += 3;
			continue;
		}
		// 先頭が JOI でも続ける
		if(i+2 < s.size() && s[i] == 'J' && s[i+1] == 'O' && s[i+2] == 'I'){
			i += 2;
			continue;
		}
		// 先頭が IOI でも続ける
		if(i+2 < s.size() && s[i] == 'I' && s[i+1] == 'O' && s[i+2] == 'I'){
			i += 2;
			continue;
		}

		// 3 つチェックしてどれでもなければ一発アウト
		cout << "No" << endl;
		return 0;
	}

	// ループが最後まで回ったということは、OKだったということなので、Yesを表示
	cout << "Yes" << endl;
	return 0;
}

