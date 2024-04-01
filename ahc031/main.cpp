#include<bits/stdc++.h>
#include<random>
#include<chrono>
//入力系
#define cinll(...) ll __VA_ARGS__; input(__VA_ARGS__);
#define cinint(...) int __VA_ARGS__; input(__VA_ARGS__);
#define cinstr(...) string __VA_ARGS__; input(__VA_ARGS__);
#define cinchar(...) char __VA_ARGS__; input(__VA_ARGS__);
#define cinvll(a,n) vll a(n); rep(i,n) cin>>a[i];
#define cinvs(a,n) vs a(n); rep(i,n) cin>>a[i];
//繰り返し系
#define rep1(n) for(int i=0;i<n;i++)
#define rep2(i,n) for(int i=0;i<n;i++)
#define rep3(i,a,n) for(int i=a;i<n;i++)
#define rep4(i,a,n,b) for(int i=a;i<n;i+=b)
#define overload4(a,b,c,d,e,...) e
#define rep(...) overload4(__VA_ARGS__,rep4,rep3,rep2,rep1)(__VA_ARGS__)
#define mrep1(n) for(int i=n;i>=0;i--)
#define mrep2(i,n) for(int i=n;i>=0;i--)
#define mrep3(i,n,a) for(int i=n;i>=a;i--)
#define mrep4(i,n,a,b) for(int i=n;i>=a;i-=b)
#define mrep(...) overload4(__VA_ARGS__,mrep4,mrep3,mrep2,mrep1)(__VA_ARGS__)
//iterator系
#define all(a) a.begin(),a.end()
using namespace std;
//型系
using ll = long long;
using vll = vector<long long>;
using vvll = vector<vector<long long>>;
using vvvll = vector<vector<vector<long long>>>;
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

//便利系
//vectorをjoinするやつ
template<typename T>
string v_join(vector<T> &a,string c){
	string ret = "";
	rep(i,a.size()-1) ret += (to_string(a[i]) + c);
	return ret + to_string(a[a.size()-1]);
}
//空白区切り表示
template<class T, class... Ts>
void out(const T& a,const Ts&... b){
	cout<<a;
	(cout<< ... << (cout << ' ',b));
	cout<<endl;
}
//入力
template<class... T>
void input(T&... a){
	(cin >> ... >> a);
}

ll w;
//日数
ll d;
//一日の予約数
ll n;
//各日の各予約の所望面積
vvll a;

double time_limit = 3000;

//score
ll score(const vvvll &ans){
	ll ret = 0;
	//バリケード
	vvb r(vvb(w,vb(w,false)));
	rep(i,d){
		rep(j,n){
			//面積分のスコア
			ll s = (ans[i][j][2]-ans[i][j][0]) * (ans[i][j][3]-ans[i][j][1]);
			if(a[i][j] > s) ret += (a[i][j]-s)*100;

			// //バリケードのスコア
			// vvb nr(vvb(w,vb(w,false)));
			// //上横
			// if(ans[i][j][0] > 0) rep(k,ans[i][j][1],ans[i][j][3]+1){
			// 	nr[ans[i][j][0]][k] = true;
			// }
			// //右縦
			// if(ans[i][j][3] < w) rep(k,ans[i][j][0],ans[i][j][2]+1){
			// 	nr[k][ans[i][j][3]] = true;
			// }
			// //下横
			// if(ans[i][j][2] < w) rep(k,ans[i][j][1],ans[i][j][3]+1){
			// 	nr[ans[i][j][2]][k] = true;
			// }
			// //左縦
			// if(ans[i][j][1] > 0) rep(k,ans[i][j][0],ans[i][j][2]+1){
			// 	nr[k][ans[i][j][1]] = true;
			// }
			// rep(k,w) rep(l,w){
			// 	if(nr[k][l] != r[k][l]){
			// 		r[k][l] = nr[k][l];
			// 		ret++;
			// 	}
			// }
		}
	}
	return ret;
}

void input_problem(){
	cin>>w>>d>>n;
	a = vvll(d,vll(n,0));
	rep(i,d) rep(j,n){
		cin>>a[i][j];
	}
}

//[a,b]の乱数を整数値で返す
ll rand(ll a,ll b){
	random_device rnd;
	mt19937 mt(rnd());
	uniform_int_distribution<> randD(a,b);
	return randD(mt);
}

void create_problem(){
	w = 1000;
	d = rand(5,50);
	n = rand(5,50);
	a = vvll(d,vll(n,0));
	double e = rand(500,5000) / 10000.0;
	int E = round(w*w*e*e);
	rep(i,d){
		int td = rand((int)w*(int)w-floor((3*E)/2.0),(int)w*(int)w-floor(E/2.0));
		vll s(2);
		s[0] = 0;
		s[1] = td;
		rep(j,n-1){
			s.push_back(rand(1,td-1));
		}
		sort(s.begin(),s.end());
		rep(j,n){
			a[i][j] = s[j+1]-s[j];
		}
	}
}

void show_ans(const vvvll &ans){
	rep(i,d){
		rep(j,n){
			cout<<ans[i][j][0]<<" "<<ans[i][j][1]<<" "<<ans[i][j][2]<<" "<<ans[i][j][3]<<endl;
		}
	}
}



namespace Yakinamashi{

	vll create_first_solution(){
		vll hs(n+1,0);
		hs[0] = 0;
		hs[n] = 1000;
		for(int i=n-1;i>=1;i--){
			hs[i] = rand(i,hs[i+1]-1);
		}
		return hs;
	}

	vvvll create_ans_from_solution(const vll &solution){
		vvvll ans(d,vvll(n,vll(4,0)));
		rep(i,d) rep(j,n){
			ans[i][j][0] = solution[j];
			ans[i][j][1] = 0;
			ans[i][j][2] = solution[j+1];
			ans[i][j][3] = w;
		}
		return ans;
	}

	ll eval(const vll &solution){
		vvvll ans = create_ans_from_solution(solution);
		return score(ans);
	}

	

}


//方針1
//高さ10、横幅1000のやつに分割する
//左上からとる
vvvll solve1(){
	ll dh = 10;
	ll dw = 1000;
	vvvll ans(d,vvll(n,vll(4,0)));

	//i日目（全部同じ）
	rep(i,d){
		rep(j,n){
			ans[i][j][0] = j*dh;
			ans[i][j][1] = 0;
			ans[i][j][2] = (j+1)*dh;
			ans[i][j][3] = dw;
		}
	}
	return ans;
}

//方針2
//高さ20、横幅1000にする（経験則）
vvvll solve2(){
	ll dh = 20;
	ll dw = 1000;
	vvvll ans(d,vvll(n,vll(4,0)));

	//i日目（全部同じ）
	rep(i,d){
		rep(j,n){
			ans[i][j][0] = j*dh;
			ans[i][j][1] = 0;
			ans[i][j][2] = (j+1)*dh;
			ans[i][j][3] = dw;
		}
	}
	return ans;
}

//方針3
//高さを20に固定して、全体の最大面積(min 20000)以上の最小の面積になるようにする
vvvll solve3(){
	vvvll ans(d,vvll(n,vll(4,0)));
	//全体の中で最大を求める
	ll maxi = 0;
	rep(i,d) maxi = max(maxi,*max_element(a[i].begin(),a[i].end()));
	maxi = min(maxi,20000LL);

	ll dh = 20;
	ll dw = 1;
	while(dw*dh <= maxi && dw <= 1000) dw++;
	dw--;

	//i日目（全部同じ）
	rep(i,d){
		rep(j,n){
			ans[i][j][0] = j*dh;
			ans[i][j][1] = 0;
			ans[i][j][2] = (j+1)*dh;
			ans[i][j][3] = dw;
		}
	}
	return ans;
}

//方針4
//方針3の最大面積にフィットさせる奴を日ごとにやる
vvvll solve4(){
	vvvll ans(d,vvll(n,vll(4,0)));

	//i日目（全部同じ）
	rep(i,d){
		//日ごとの最大を求める
		ll maxi = *max_element(a[i].begin(),a[i].end());
		maxi = min(maxi,20000LL);

		ll dh = 20;
		ll dw = 1;
		while(dw*dh <= maxi && dw <= 1000) dw++;
		dw--;
		rep(j,n){
			ans[i][j][0] = j*dh;
			ans[i][j][1] = 0;
			ans[i][j][2] = (j+1)*dh;
			ans[i][j][3] = dw;
		}
	}
	return ans;
}

//方針5
//種類ごとの最大を出しておけばいいのでは？
vvvll solve5(){
	vvvll ans(d,vvll(n,vll(4,0)));
	//種類ごとの最大を求める
	vll maxis(n,0);
	rep(i,n){
		rep(j,d) maxis[i] = max(maxis[i],a[j][i]);
	}

	//種類ごとの幅を求めておく
	vll dws(n,1);
	ll dh = w / n;
	rep(i,n){
		while(dws[i]*dh <= maxis[i] && dws[i] <= 1000) dws[i]++;
		dws[i]--;
	}
	

	//i日目（全部同じ）
	rep(i,d){
		rep(j,n){
			ans[i][j][0] = j*dh;
			ans[i][j][1] = 0;
			ans[i][j][2] = (j+1)*dh;
			ans[i][j][3] = dws[j];
		}
	}
	return ans;
}

//方針6
//縦の長さも調節して、全部入るようにする
//5回目の提出はdhs[i]--とかdws[j]--の部分がミスってたから6回目で提出しなおし
vvvll solve6(){
	vvvll ans(d,vvll(n,vll(4,0)));
	//種類ごとの最大を求める
	vll maxis(n,0);
	rep(i,n){
		rep(j,d) maxis[i] = max(maxis[i],a[j][i]);
	}

	//種類ごとの幅を求めておく
	vll dws(n,0);
	vll dhs(n,0);
	ll dhs_sum = 0;
	//種類iのやつの高さをdhs[i]にした時、dhw[i]を1000まで伸ばせばmaxisに届くdhs[i]を求める
	rep(i,n){
		while(dhs[i]*1000 < maxis[i] && dhs_sum + dhs[i]+(n-i-1) < 1000) dhs[i]++;
		if(dhs[i] > 1 && dhs_sum + dhs[i]+(n-i-1) > 1000) dhs[i]--;
		dhs_sum += dhs[i];
	}
	//各種類jについて、面積が要望以上で最小になるdws[j]を求める
	rep(j,n){
		while(dws[j]*dhs[j] < maxis[j] && dws[j] < 1000) dws[j]++;
		if(dws[j] > 0 && dws[j] > 1000) dws[j]--;
	}
	

	//i日目（全部同じ）
	rep(i,d){
		ll dhs_sum = 0;
		rep(j,n){
			ans[i][j][0] = dhs_sum;
			ans[i][j][1] = 0;
			ans[i][j][2] = dhs_sum+dhs[j];
			ans[i][j][3] = dws[j];
			dhs_sum += dhs[j];
		}
	}
	return ans;
}

//方針7
//横は1000固定で縦のみ変える
//バリケードの設置に割とスコアがとられちゃうっぽい？
vvvll solve7(){
	vvvll ans(d,vvll(n,vll(4,0)));
	//種類ごとの最大を求める
	vll maxis(n,0);
	rep(i,n){
		rep(j,d) maxis[i] = max(maxis[i],a[j][i]);
	}

	//種類ごとの縦幅を求めておく
	vll dhs(n,0);
	ll dhs_sum = 0;
	//種類iのやつの高さをdhs[i]にした時、dhw[i]を1000まで伸ばせばmaxisに届くdhs[i]を求める
	rep(i,n){
		while(dhs[i]*1000 < maxis[i] && dhs_sum + dhs[i]+(n-i-1) < 1000) dhs[i]++;
		if(dhs[i] > 1 && dhs_sum + dhs[i]+(n-i-1) > 1000) dhs[i]--;
		dhs_sum += dhs[i];
	}
	

	//i日目（全部同じ）
	rep(i,d){
		ll dhs_sum = 0;
		rep(j,n){
			ans[i][j][0] = dhs_sum;
			ans[i][j][1] = 0;
			ans[i][j][2] = dhs_sum+dhs[j];
			ans[i][j][3] = 1000;
			dhs_sum += dhs[j];
		}
	}
	return ans;
}

//方針8
//横幅は1000固定して縦を焼きなましで探す
//一旦山登りで作ろうかしら
vvvll solve8(){
	chrono::system_clock::time_point start,end;
	start = chrono::system_clock::now();
	end = chrono::system_clock::now();
	double run_time = static_cast<double>(chrono::duration_cast<chrono::microseconds>(end-start).count()/1000.0);

	vvvll ans(d,vvll(n,vll(4,0)));
	//初期解法を一つ決める(ランダムにするか)
	vll solution = Yakinamashi::create_first_solution();	
	ll score = Yakinamashi::eval(solution);

	ll dh = 2;

	ll count = 0;

	//2.8秒までならやっちゃってOK
	while(run_time <= 2800){
		//現在の解をちょいずらして、新しい解を生成
		ll rand_ind = rand(1,n-1);
		ll next_value = rand(max(solution[rand_ind]-dh,solution[rand_ind-1]+1),min(solution[rand_ind]+dh,solution[rand_ind+1]-1));
		ll prev_value = solution[rand_ind];
		//solution[rand_ind] = next_value;
		vll next_solution = Yakinamashi::create_first_solution();
		ll next_score = Yakinamashi::eval(next_solution);
		if(next_score < score){
			//solution = next_solution;
			score = next_score;
		}
		end = chrono::system_clock::now();
		run_time = static_cast<double>(chrono::duration_cast<chrono::microseconds>(end-start).count()/1000.0);

		count++;
	}

	return Yakinamashi::create_ans_from_solution(solution);

}


namespace Solution9{

	ll eval(const vvll &solution){
		vvvll ans(d,vvll(n,vll(4,0)));
		rep(i,d) rep(j,n) rep(k,4) ans[i][j][k] = solution[j][k];
		return score(ans);
	}

	vvll create_first_solution(){
		//正方形を斜めに並べるやつ
		// vvll ret(n,vll(4,0));
		// rep(i,n){
		// 	ret[i][0] = (w/n*i);
		// 	ret[i][1] = (w/n*i);
		// 	ret[i][2] = (w/n*(i+1));
		// 	ret[i][3] = (w/n*(i+1));
		// }
		// return ret;

		//最初をsolution6にしちゃう
		// vvvll ans = solve6();
		// vvll ret (n,vll(4));
		// rep(i,n) rep(j,4){
		// 	ret[i][j] = ans[0][i][j];
		// }
		// return ret;

		//横幅を1000に固定して、高さを各種類の最大の割合にする
		//種類ごとの最大を求める
		vll maxis(n,0);
		rep(i,n){
			rep(j,d) maxis[i] = max(maxis[i],a[j][i]);
		}
		ll sum = 0;
		rep(i,n) sum += maxis[i];
		vector<double> ratio(n);
		rep(i,n) ratio[i] = (double)maxis[i]/sum;
		vvll ret(n,vll(4));
		ll prev_h = 0;
		rep(i,n-1){
			ll dh = (ll)(ratio[i]*w);
			if(dh == 0) dh = 1;
			ret[i][0] = prev_h;
			ret[i][1] = 0;
			ret[i][2] = prev_h + dh;
			ret[i][3] = 1000;
			prev_h += dh;
		}
		ret[n-1][0] = prev_h;
		ret[n-1][1] = 0;
		ret[n-1][2] = w;
		ret[n-1][3] = w;
		return ret;
	}

	bool isCross(const vll &a,const vll &b){
		//横方向に重なりがある
		bool is_cross_w = min(a[2],b[2])-max(a[0],b[0]) > 0;
		//縦方向に重なりがある
		bool is_cross_h = min(a[3],b[3])-max(a[1],b[1]) > 0;
		// cout<<"is_cross:("<<a[0]<<","<<a[1]<<","<<a[2]<<","<<a[3]<<") ("<<b[0]<<","<<b[1]<<","<<b[2]<<","<<b[3]<<")"<<endl;
		// cout<<"is_cross_w:"<<is_cross_w<<endl;
		// cout<<"is_cross_h:"<<is_cross_h<<endl;
		return is_cross_w && is_cross_h;
	}

	bool isCross(const vvll &solution){
		rep(i,solution.size()) rep(j,solution.size()) {
			if(i == j) continue;
			if(isCross(solution[i],solution[j]) == true){
				return true;
			}
		}
		return false;
	}

	bool isOutGrid(const vvll &solution){
		rep(i,solution.size()){
			if(solution[i][0] < 0) return true;
			if(solution[i][1] < 0) return true;
			if(solution[i][2] > w) return true;
			if(solution[i][3] > w) return true;
		}
		return false;
	}

	bool isSizeLessThanEquallZero(const vvll &solution){
		rep(i,solution.size()){
			if(solution[i][2]-solution[i][0] <= 0) return true;
			if(solution[i][3]-solution[i][1] <= 0) return true;
		}
		return false;
	}

	void set_move_next_solution(vvll &solution, int index, double temperature){
		//動かす距離
		//温度が0 -> 200、温度が3000 -> 0
		ll first_limit = 100;
		ll last_limit = 0;
		ll d_move_limit = (ll)((last_limit-first_limit)/time_limit*temperature+first_limit);
		ll d_move = rand(0,d_move_limit);
		//方向を決める
		//0:上、1:右、2:下、3:左
		ll r = rand(0,3);
		if(r == 0){
			solution[index][0] -= d_move;
			solution[index][2] -= d_move;
		}
		else if(r == 1){
			solution[index][1] += d_move;
			solution[index][3] += d_move;
		}
		else if(r == 2){
			solution[index][0] += d_move;
			solution[index][2] += d_move;
		}
		else{
			solution[index][1] -= d_move;
			solution[index][3] -= d_move;
		}
	}

	void set_change_size_next_solution(vvll &solution, int index, double temperature){
		//変更するサイズ
		ll first_limit = 1;
		ll last_limit = 1;
		ll d_size_limit = (ll)((last_limit-first_limit)/10000.0*temperature+first_limit);
		//方向を決める
		ll d_size = rand(0,d_size_limit);
		//伸ばすか縮めるか
		d_size *= (rand(0,1) == 1 ? -1:1);
		//方向を決める
		//0:上、1:右、2:下、3:左
		// ll r = rand(0,3);
		// if(r == 0){
		// 	solution[index][0] -= d_size;
		// }
		// else if(r == 1){
		// 	solution[index][3] += d_size;
		// }
		// else if(r == 2){
		// 	solution[index][2] += d_size;
		// }
		// else{
		// 	solution[index][1] -= d_size;
		// }

		//今は上下のサイズ変更のみ
		ll r = rand(0,1);
		if(r == 0){
			solution[index][0] -= d_size;
		}
		else{
			solution[index][2] += d_size;
		}
	}
	
	vvll create_next_solution(const vvll &solution, double temperature){
		//まずはコピー
		vvll next_solution(n,vll(4,0));
		rep(i,n) rep(j,4) next_solution[i][j] = solution[i][j];

		//何番目をやるか
		ll rand_ind = rand(0,n-1);

		// //0:動かす、1:サイズ変更
		// ll r1 = rand(0,1);
		// if(r1 == 0){
		// 	set_move_next_solution(next_solution, rand_ind, temperature);
		// }
		// else{
		// 	set_change_size_next_solution(next_solution, rand_ind, temperature);
		// }

		set_change_size_next_solution(next_solution, rand_ind, temperature);

		//三つの関門のチェックを潜り抜ければOK
		if(isCross(next_solution) == true) return solution;
		if(isOutGrid(next_solution) == true) return solution;
		if(isSizeLessThanEquallZero(next_solution) == true) return solution;
		return next_solution;
	}

	vvvll create_ans_from_solution(const vvll &solution){
		vvvll ans(d,vvll(n,vll(4,0)));
		rep(i,d) rep(j,n) rep(k,4) ans[i][j][k] = solution[j][k];
		return ans;
	}

	
}

//方針9
//w/n*w/nの正方形を左上から斜めにn個作って、それを各方向に伸ばす・縮める・移動を繰り返して焼きなまし
//一旦山登りで作ろうかしら
vvvll solve9(){
	//return Solution9::create_ans_from_solution(Solution9::create_first_solution());
	chrono::system_clock::time_point start,end;
	start = chrono::system_clock::now();
	end = chrono::system_clock::now();
	double run_time = static_cast<double>(chrono::duration_cast<chrono::microseconds>(end-start).count()/1000.0);

	vvvll ans(d,vvll(n,vll(4,0)));
	//初期解法を一つ決める(ランダムにするか)
	vvll solution = Solution9::create_first_solution();	
	ll score = Solution9::eval(solution);

	ll count = 0;

	//2.8秒までならやっちゃってOK
	while(run_time <= time_limit-100){
		vvll next_solution = Solution9::create_next_solution(solution, run_time);
		ll next_score = Solution9::eval(next_solution);
		//スコアの許容も温度に応じて行う
		ll first_capa = 200000LL;
		ll last_capa = 0LL;
		ll capacity = (ll)((last_capa-first_capa)/(time_limit-100)*run_time + first_capa);
		double temperature = (time_limit-run_time)*1000 + 400000;
		//cout<<"temperature:"<<temperature<<" "<<exp((score-next_score)/temperature)<<" "<<(score-next_score)/temperature<<endl;
		//cout<<"capacity:"<<capacity<<" "<<run_time<<endl;
		if(next_score < score+capacity){
			solution = next_solution;
			score = next_score;
		}
		// if(next_score <= score){
		// 	solution = next_solution;
		// 	score = next_score;
		// }
		// else{
		// 	std::mt19937_64 mt64(0);  //もとにする整数型の乱数は別途必要
  		// 	std::uniform_real_distribution<double> uni(0, 1);
		// 	if(uni(mt64) <= exp((score-next_score)/temperature)){
		// 		solution = next_solution;
		// 		score = next_score;
		// 	}
		// }
		end = chrono::system_clock::now();
		run_time = static_cast<double>(chrono::duration_cast<chrono::microseconds>(end-start).count()/1000.0);

		count++;
	}

	//cout<<"count:"<<count<<endl;

	return Solution9::create_ans_from_solution(solution);
}

//日ごとに高さを調節
//一旦割合にする
vvvll solve10(){
	vvvll ans(d,vvll(n,vll(4)));
	rep(i,d){
		vector<double> ratio(n);
		double sum = 0;
		rep(j,n) sum += a[i][j];
		rep(j,n) ratio[j] = a[i][j]/sum;
		ll prev_h = 0;
		rep(j,n){
			ll dh = (ll)(ratio[j]*w);
			if(dh == 0) dh = 1;
			ans[i][j][0] = prev_h;
			ans[i][j][1] = 0;
			ans[i][j][2] = (j == n-1) ? w:(prev_h + dh);
			ans[i][j][3] = w;
			prev_h += dh;
		}
	}
	return ans;
}

//日ごとの計算を絶対入る奴にする
vvvll solve11(){
	vvvll ans(d,vvll(n,vll(4)));
	rep(i,d){
		ll dhs_sum = 0;
		rep(j,n){
			ll dh = 0;
			while(dh*1000 < a[i][j] && dhs_sum + dh+(n-i-1) < 1000) dh++;
			if(dh > 1 && dhs_sum + dh+(n-i-1) > 1000) dh--;
			ans[i][j][0] = dhs_sum;
			ans[i][j][1] = 0;
			ans[i][j][2] = (j == n-1) ? w:(dhs_sum+dh);
			ans[i][j][3] = w;
			dhs_sum += dh;
		}
	}
	return ans;
}

void test(int epocs){

	double avl_score = 0;
	
	rep(epoc,epocs){
		cout<<"epoc:"<<epoc<<endl;
		create_problem();
		vvvll ans = solve10();
		avl_score += score(ans) / (double)epocs;
	}

	
	cout<<"avl_score :"<<avl_score<<endl;

}

void solve(){
		
	input_problem();

	vvvll ans = solve11();

	show_ans(ans);

}

//なぜか5のテスト結果が異様にいいから5でいったん出す
//↑これはなんかスコアの計算ミスってるかも？
//やっぱ6が忘れられないから6で出す！！！

//初期解を「割合」での大きさにする
//定期的に高さを1にする

//初期会を割合にしたやつ、一回出してみる価値あり

//帰ったらそのままだす

//最初に作るんじゃなくて、毎日最適解を変えてみる
//毎日割合で縦軸を変えて出すようにしてみる
//日ごとにするならmaxs使うやつで必ず入るようにしてもいいかも

int main(void){
	//test(1000);
	solve();
	
	return 0;
	
}
