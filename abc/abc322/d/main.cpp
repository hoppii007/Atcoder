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

//便利系
//vectorをjoinするやつ
template<typename T>
string v_join(vector<T> &a,string c){
	string ret = "";
	rep(i,a.size()-1) ret += (to_string(a[i]) + c);
	return ret + to_string(a[a.size()-1]);
}

class P{
public:
	vvc grid;
	int state;
	P(vs a){
		grid = vvc(a.size(),vc(a[0].size()));
		rep(i,a.size()) rep(j,a[i].size()) grid[i][j] = a[i][j];
		state = 0;
		SetLeftUp();
	}
	P(){}

	//一番左のインデックスを返す
	int GetLeftest(){
		rep(j,4){
			rep(i,4) if(grid[i][j] == '#') return j;
		}
		return -1;
	}
	//一番右のインデックスを返す
	int GetRightest(){
		rep(j,4){
			rep(i,4) if(grid[i][4-j-1] == '#') return 4-j-1;
		}
		return -1;
	}
	//一番上のインデックスを返す
	int GetUpest(){
		rep(i,4){
			rep(j,4) if(grid[i][j] == '#') return i;
		}
		return -1;
	}
	//一番下のインデックスを返す
	int GetDownest(){
		rep(i,4){
			rep(j,4) if(grid[4-i-1][j] == '#') return 4-i-1;
		}
		return -1;
	}

	//左に行けるか
	bool CanMoveLeft(){
		int l = GetLeftest();
		return l > 0;
	}
	//上に行けるか
	bool CanMoveUp(){
		int u = GetUpest();
		return u > 0;
	}
	//右に行けるか
	bool CanMoveRight(){
		int r = GetRightest();
		return r < 3;
	}
	//下に行けるか
	bool CanMoveDown(){
		int d = GetDownest();
		return d < 3;
	}

	//左に行けるときは左いく
	void ShiftLeft(){
		if(CanMoveLeft() == false) return;
		rep(i,4) rep(j,3){
			grid[i][j] = grid[i][j+1];
		}
		rep(i,4) grid[i][3] = '.';
	}

	//上に行けるときは上に行く
	void ShiftUp(){
		if(CanMoveUp() == false) return;
		rep(i,3) rep(j,4){
			grid[i][j] = grid[i+1][j];
		}
		rep(j,4) grid[3][j] = '.';
	}

	//右に行けるときは右に行く
	void ShiftRight(){
		if(CanMoveRight() == false) return;
		rep(i,4) for(int j=3;j>0;j--){
			grid[i][j] = grid[i][j-1];
		}
		rep(i,4) grid[i][0] = '.';
	}

	//下に行けるときは下に行く
	void ShiftDown(){
		if(CanMoveDown() == false) return;
		for(int i=3;i>0;i--) rep(j,4){
			grid[i][j] = grid[i-1][j];
		}
		rep(j,4) grid[0][j] = '.';
	}

	//右回転
	void Rotate(){
		vvc tmp(4,vc(4));
		rep(i,4) rep(j,4){
			//0,0 -> 0,4  0,1 -> 1,4 1,0 -> 0,3
			tmp[j][3-i] = grid[i][j];
		}
		rep(i,4) rep(j,4) grid[i][j] = tmp[i][j];
		SetLeftUp();
	}

	//左上にそろえる
	void SetLeftUp(){
		rep(i,4){
			ShiftUp();
			ShiftLeft();
		}
	}

	void Show(){
		cout<<"Show"<<endl;
		rep(i,4){
			rep(j,4) cout<<grid[i][j];
			cout<<endl;
		}
		cout<<endl;
	}

	void Next(){
		if(state == 15){
			Rotate();
			state = 0;
			return;
		}
		if(state %4 == 3){
			rep(i,4) ShiftLeft();
			ShiftDown();
			state++;
			return;
		}
		ShiftRight();
		state++;
	}
};

int main(void){
	cinvs(s,4);
	cinvs(t,4);
	cinvs(u,4);
	P p1(s);
	P p2(t);
	P p3(u);

	rep(i,16*4){
		rep(j,16 * 4){
			rep(k,16 * 4){
				vvc tmp(4,vc(4,'.'));
				bool isOK = true;
				rep(ii,4){
					rep(jj,4){
						int c = 0;
						if(p1.grid[ii][jj] == '#') c++;
						if(p2.grid[ii][jj] == '#') c++;
						if(p3.grid[ii][jj] == '#') c++;
						if(c != 1){
							isOK = false;
							break;
						}
					}
					if(isOK == false) break;
				}
				if(isOK == true){
					cout<<"Yes"<<endl;
					return 0;
				}
				p3.Next();
			}
			p2.Next();
		}
		p1.Next();
	}
	cout<<"No"<<endl;

	return 0;
}


