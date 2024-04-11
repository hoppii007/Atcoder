//trueだったら第1引数が手前に来る
//p2 < p1として作ると降順になる
//p1 < p2として作ると昇順になる
struct comp{
    bool operator()(const pair<int,int> &p1,const pair<int,int> &p2) const{
        if(p1.first == p2.first) return p1.second < p2.second;
        return p1.first < p2.first;
    }
};

int main(void){
  map<pll,ll,comp> count;
  count[{2,3}]++;
  count[{3,2}] += 3;
  for(pair<pll,ll> p:count) cout<<p.first.first<<" "<<p.first.second<<" "<<p.second<<endl;
  
  set<pll,comp> s;
  s.insert({2,3});
  s.insert({3,5});
  for(pll p:s) cout<<p.first<<" "<<p.second<<endl;
	return 0;
}
