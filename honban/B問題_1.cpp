// 考察
// さすがに100の中には答えがあるだろうとあたりをつけてみる
// 0 ~ 100 まで順番にチェックし、A でも B でもないものを見つけた時点で表示して break すればよい

#include <iostream>
using namespace std;

int main(){
	int a, b;
	cin >> a >> b;
	for(int i=0;i<=100;i++){
		if(i != a && i != b){
			cout << i << endl;
			break;
		}
	}
	return 0;
}