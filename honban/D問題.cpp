// 考察
// 1 以上 N 以下の整数 (i,j) の組み合わせをすべて試し、条件に合うものを数えればいい。
// JOI模試のD問題「差」を解いたことがあれば、ほとんど同じコードで解ける

#include <iostream>
#include <vector>
using namespace std;

int main(){
	int n;
	cin >> n;
	vector<int> a(n);
	for(int i=0;i<n;i++){
		cin >> a[i];
	}

	// 0 以上 N 未満のすべての組み合わせを試す
	// 問題では 1 以上 N 以下となっているが、プログラムでは番目を扱う際「0スタート」で数えるため、問題文の数字と1ずれることがよくある
	int ans = 0;
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			if(i < j && a[i]%a[j] == 0){
				ans++;
			}
		}
	}

	// おまけ
	// i < jという条件は、ループを回す範囲を工夫すれば if でチェックしなくても良い
	// for(int i=0;i<n;i++){
	// 	for(int j=i+1;j<n;j++){
	// 		if(a[i]%a[j] == 0){
	// 			ans++;
	// 		}
	// 	}
	// }

	cout << ans << endl;
	return 0;
}

