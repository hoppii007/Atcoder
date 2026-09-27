// 考察
// 5 回戦までのトーナメントをごり押しで計算してみる
// 初めは32チームあるので長さ 32 の配列を用意しておき、1回戦がすべて終わったら16チーム残るので、長さ16の配列を用意し...
// というふうに長さ32, 16, 8, 4, 2 の配列をそれぞれ用意し、何回戦でどのチームが勝ち上がったかを記録していく
// 最終的に勝ち残った 2 チームの中で弱い方の番号が答え

#include <iostream>
#include <vector>
using namespace std;

int main(){
	// i 番目のチームの強さを power[i] に保存しておく
	vector<int> power(32);
	for(int i=0;i<32;i++){
		cin >> power[i];
	}

	// 初めは全チームが順番に並んでいる
	vector<int> a(32);
	for(int i=0;i<32;i++){
		a[i] = i;
	}

	// 1 回戦で隣同士 (チームa[i] と チームa[i+1]) で戦い、勝った方が2回戦に進む
	vector<int> b(16);
	for(int i=0;i<32;i+=2){
		// チーム a[i+1] が勝ったとき
		if(power[a[i]] < power[a[i+1]]){
			b[i/2] = a[i+1];
		}
		// チーム a[i] が勝ったとき
		else{
			b[i/2] = a[i];
		}
	}

	// 2 回戦で隣同士 (チームb[i] と チームb[i+1]) で戦い、勝った方が3回戦に進む
	vector<int> c(8);
	for(int i=0;i<16;i+=2){
		// チーム b[i+1] が勝ったとき
		if(power[b[i]] < power[b[i+1]]){
			c[i/2] = b[i+1];
		}
		// チーム b[i] が勝ったとき
		else{
			c[i/2] = b[i];
		}
	}

	// 以下同様に5 回戦までやる
	vector<int> d(4);
	for(int i=0;i<8;i+=2){
		if(power[c[i]] < power[c[i+1]]){
			d[i/2] = c[i+1];
		}
		else{
			d[i/2] = c[i];
		}
	}
	vector<int> e(2);
	for(int i=0;i<4;i+=2){
		if(power[d[i]] < power[d[i+1]]){
			e[i/2] = d[i+1];
		}
		else{
			e[i/2] = d[i];
		}
	}

	// 決勝戦で負けた方を表示する
	if(power[e[0]] < power[e[1]]){
		cout << e[0]+1 << endl;
	}
	else{
		cout << e[1]+1 << endl;
	}
	return 0;
}