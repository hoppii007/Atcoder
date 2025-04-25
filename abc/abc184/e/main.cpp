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

struct Pos{
public:
	ll i,j;
	Pos(ll i,ll j): i(i),j(j){}
	Pos(){}
	bool operator ==(const Pos &p)const {
		return this->i == p.i && this->j == p.j;
	}
	bool operator <(const Pos &p)const{
		if(this->i != p.i) return this->i < p.i;
		return this->j < p.j;
	}
	bool operator <=(const Pos &p)const{
		if(this->i != p.i) return this->i <= p.i;
		return this->j <= p.j;
	}
	bool CanUp(vs &a){ return i > 0 && a[i-1][j] != '#'; }
	bool CanDown(vs &a){ return i < a.size()-1 && a[i+1][j] != '#'; }
	bool CanRight(vs &a){ return j < a[0].size()-1 && a[i][j+1] != '#'; }
	bool CanLeft(vs &a){ return j > 0 && a[i][j-1] != '#'; }
	Pos Up(){ return Pos(this->i-1,this->j); }
	Pos Down(){ return Pos(this->i+1,this->j); }
	Pos Left(){ return Pos(this->i,this->j-1); }
	Pos Right(){ return Pos(this->i,this->j+1); }
};

bool isCanWarp(char c){
	return c != 'S' && c != 'G' && c != '.' && c != '#';
}

int main(void){
	cinll2(h,w);
	cinvs(a,h);
	Pos startPos,goalPos;
	map<char,set<Pos>> canWarp;
	for(char c:"abcdefghijklmnopqrstuvwxyz"){
		canWarp[c] = set<Pos>();
	}
	rep(i,h) rep(j,w){
		if(a[i][j] == 'S') startPos = Pos(i,j);
		if(a[i][j] == 'G') goalPos = Pos(i,j);
		if(isCanWarp(a[i][j])){
			canWarp[a[i][j]].insert(Pos(i,j));
		}
	}
	vvll scores(h,vll(w,-1));
	queue<Pos> q;
	q.push(startPos);
	scores[startPos.i][startPos.j] = 0;
	while(!q.empty()){
		Pos now = q.front();
		q.pop();
		vector<Pos> nexts;
		//上に行ける場合
		if(now.CanUp(a)) nexts.push_back(now.Up());
		//下に行ける場合
		if(now.CanDown(a)) nexts.push_back(now.Down());
		//右
		if(now.CanRight(a)) nexts.push_back(now.Right());
		//左
		if(now.CanLeft(a)) nexts.push_back(now.Left());
		//ワープできる場合
		if(isCanWarp(a[now.i][now.j])){
			vector<Pos> delList;
			for(Pos p : canWarp[a[now.i][now.j]]){
				if(scores[p.i][p.j] != -1){
					delList.push_back(p);
					continue;
				}
				nexts.push_back(p);
			}
			for(Pos p: delList) canWarp[a[now.i][now.j]].erase(p);
		}
		for(Pos next: nexts){
			if(scores[next.i][next.j] != -1) continue;
			scores[next.i][next.j] = scores[now.i][now.j]+1;
			q.push(next);
		}
	}
	cout<<scores[goalPos.i][goalPos.j]<<endl;

	return 0;
}
