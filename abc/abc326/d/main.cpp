#include<bits/stdc++.h>
//入力系
#define cinll(a) ll a; cin>>a; 
#define cinll2(a,b) ll a,b; cin>>a>>b;
#define cinll3(a,b,c) ll a,b,c; cin>>a>>b>>c;
#define cinint(a) int a; cin>>a;
#define cinint2(a,b) int a,b; cin>>a>>b;
#define cinint3(a,b,c) int a,b,c; cin>>a>>b>>c;
#define cinstring(s) string s; cin>>s;
#define cinstring2(s,t) string s,t; cin>>s>>t;
#define cinstring3(s,t,u) string s,t,u; cin>>s>>t>>u;
#define cinchar(c) char c; cin>>c;
#define cinvll(a,n) vll a(n); rep(i,n) cin>>a[i];
#define cinvs(a,n) vs a(n); rep(i,n) cin>>a[i];
//繰り返し系
#define rep(i,n) for(int i=0;i<n;i++)
#define rep1(i,n) for(int i=1;i<n;i++)
#define mrep(i,n) for(int i=n;i>=0;i--)
using namespace std;
//型系
using ll = long long;
using vll = vector<long long>;
using vvll = vector<vector<long long>>;
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

//便利系
//vectorをjoinするやつ
template<typename T>
string v_join(vector<T> &a,string c){
	string ret = "";
	rep(i,a.size()-1) ret += (to_string(a[i]) + c);
	return ret + to_string(a[a.size()-1]);
}
//vector<bool>のtrueのところだけを抜き出すやつ
//O(N)
//aとbの要素数は、bool配列の方が長ければOK
template<typename T>
vector<T> extract(vector<T> &a,vector<bool> &b){
	vector<T> ret;
	rep(i,a.size()) if(b[i] == true) ret.push_back(a[i]);
	return ret;
}

//コンボリューションを列挙 -> v_combs
//コンボリューションを出すとき、どこを使ったかの情報のみを列挙 -> vb_combs
class Comb{
private:
	//vは最初全部falseにしといてね
	static void vb_combs_sub(vvb &vv, vb &v, int now_count, int now, int k){
		if(now_count == k) return;
		//ちょうど最後の一個を担当するとき
		if(now_count == k-1){
			for(int i=now;i<v.size();i++){
				v[i] = true;
				vv.push_back(v);
				v[i] = false;
			}
			return;
		}
		//まだまだいけるとき
		for(int i=now;i<v.size();i++){
			v[i] = true;
			vb_combs_sub(vv,v,now_count+1,i+1,k);
			v[i] = false;
		}
	}
public:
	Comb(){}
	//vector<bool>の中でk本trueが立つようにする
	//最終的にはコンボリューションの二次元配列を作る
	static vvb vb_combs(int n,int k){
		vb b = vb(n,false);
		vvb bs;
		vb_combs_sub(bs,b,0,0,k);
		return bs;
	}

	//T型の配列aのコンボリューションを列挙する
	template<typename T>
	vector<vector<T>> v_combs(vector<T> &a, int k){
		vvb bs = vb_combs(a.size(),k);
		vector<vector<T>> ret;
		rep(i,bs.size()) ret.push_back(extract<T>(a,bs[i]));
		return ret;
	}
};

int count(vvc &grid,int j,char c){
	int ret = 0;
	rep(i,grid.size()){
		if(grid[i][j] == c) ret++;
	}
	return ret;
}

char firstColumnChar(vvc &grid, int j){
	rep(i,grid.size()){
		if(grid[i][j] != '.') return grid[i][j];
	}
	return '.';
}
char firstRowChar(vvc &grid, int i){
	rep(j,grid.size()){
		if(grid[i][j] != '.') return grid[i][j];
	}
	return '.';
}

bool checkRow(vc &row,vvc &grid){
	//各列jで最初に出てくる奴がrow[j]じゃないorそもそも.しかないならアウト
	rep(j,grid.size()){
		char c = firstRowChar(grid,j);
		if(c != '.' && c != row[j]) return false;
	}
	return true;
}
bool checkColumn(vc &column, vvc &grid){
	rep(i,grid.size()){
		char c = firstColumnChar(grid,i);
		if(c != '.' && c != column[i]) return false;
	}
	return true;
}

string answer = "No";
vvc ansGrid;

//i行目を設定する
void dfs(vc &row, vc &column, vvb &combs,vvc &grid, int i){
	//最後の行を超えた場合
	if(i == grid.size()){
		// cout<<"ここでチェック"<<endl;
		// rep(ii,grid.size()){
		// 	rep(jj,grid.size()) cout<<grid[ii][jj];
		// 	cout<<endl;
		// }
		// cout<<checkRow(row,grid)<<endl;
		// cout<<checkColumn(column,grid)<<endl;
		answer = "Yes";
		ansGrid = vvc(grid.size(),vc(grid.size()));
		rep(ii,grid.size()) rep(jj,grid.size()) ansGrid[ii][jj] = grid[ii][jj];
		return;
	}
	for(vb c: combs){
		vc abc = {'A','B','C'};
		sort(abc.begin(),abc.end());
		do{
			int ind = 0;
			rep(j,c.size()){
				if(c[j] == true) grid[i][j] = abc[ind++];
				else grid[i][j] = '.';
			}
			//縦方向にaが二つ以上の場所ができたらアウト
			bool isOut = false;
			rep(j,grid.size()){
				if(count(grid,j,'A') > 1){
					isOut = true;
					break;
				}
			}
			if(isOut == true) continue;
			rep(j,grid.size()){
				if(count(grid,j,'B') > 1){
					isOut = true;
					break;
				}
			}
			if(isOut == true) continue;
			rep(j,grid.size()){
				if(count(grid,j,'C') > 1){
					isOut = true;
					break;
				}
			}
			if(isOut == true) continue;
			if(checkRow(row,grid) == false) continue;
			if(checkColumn(column,grid) == false) continue;
			dfs(row,column,combs,grid,i+1);
		}while(next_permutation(abc.begin(),abc.end()));
	}
	rep(j,grid.size()) grid[i][j] = '.';
}

int main(void){
	cinll(n);
	cinstring(r);
	cinstring(c);
	vvc grid(n,vc(n,'.'));
	vvb combs = Comb::vb_combs(n,3);
	vc row;
	rep(i,r.size()) row.push_back(r[i]);
	vc column;
	rep(i,c.size()) column.push_back(c[i]);
	dfs(row,column,combs,grid,0);
	cout<<answer<<endl;
	if(answer == "Yes"){
		rep(i,n){
			rep(j,n) cout<<ansGrid[i][j];
			cout<<endl;
		}
	}
	return 0;
}
