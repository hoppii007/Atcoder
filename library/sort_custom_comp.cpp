////////////////////////////////////////////////////
///priority_queueのcompと昇順降順が逆だから注意！！！！
////////////////////////////////////////////////////

//trueだったら第1引数が手前に来る
//p1 < p2として作ると昇順になる
//p2 < p1として作ると降順になる
bool compf(const pll &p1,const pll &p2) {
    if(p2.second == p1.second) return p2.first < p1.first;
    return p2.second < p1.second;
}

int main(void){
    vpll v;
    v.push_back({0,2});
    v.push_back({3,1});
    v.push_back({2,-1});
    sort(all(v),compf);
	return 0;
}