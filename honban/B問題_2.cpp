// 考察
// よく問題を観察すると、答えは0,1,2のどれかだとわかる
// （答えを 3 にするためには0,1,2のすべてを取り除く必要があるため）
// 3 パターンしか無いので、if + else if + else で解くことができる

#include <iostream>
using namespace std;

int main(){
	int a, b;
	cin >> a >> b;
	if(a != 0 && b != 0){
		cout << 0 << endl;
	}
	else if(a != 1 && b != 1){
		cout << 1 << endl;
	}
	else{
		cout << 2 << endl;
	}
	return 0;
}