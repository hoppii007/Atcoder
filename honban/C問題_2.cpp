// 考察
// 最終的に戦う2チームがどういうチームかを考えると、
// 「前半で一番強いチーム」と「後半で一番強いチーム」であることがわかる
// この性質を使えば、よりシンプルに解ける

#include <iostream>
#include <vector>
using namespace std;

int main(){
	// i 番目のチームの強さを power[i] に保存しておく
	vector<int> power(32);
	for(int i=0;i<32;i++){
		cin >> power[i];
	}

	///// 前半で一番強い場所を求める /////

	// 最初は 0 番目が最強ということにしておく
	int zenhan = 0; 
	for(int i=0;i<16;i++){
		// より強い チームが見つかったら、最強候補を入れ替え
		if(power[zenhan] < power[i]){
			zenhan = i;
		}
	}

	///// 後半で一番強い場所を求める /////

	// 最初は 16 番目が最強ということにしておく
	int kohan = 16; 
	for(int i=16;i<32;i++){
		// より強い チームが見つかったら、最強候補を入れ替え
		if(power[kohan] < power[i]){
			kohan = i;
		}
	}

	// 決勝戦で負けた方を表示
	if(power[zenhan] < power[kohan]){
		cout << zenhan+1 << endl;
	}
	else{
		cout << kohan+1 << endl;
	}
	
	return 0;
}