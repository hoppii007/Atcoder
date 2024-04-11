//trueだったら第1引数が手前に来る
//p2 < p1として作ると降順になる
//p1 < p2として作ると昇順になる

///////////////////////////////////////
//priority_queueは後ろからとることに注意//
///////////////////////////////////////

struct comp{
    bool operator()(const pair<int,int> &p1,const pair<int,int> &p2) const{
        if(p1.first == p2.first) return p1.second > p2.second;
        return p1.second < p2.second;
    }
};

int main(void){
	priority_queue<pair<int,int>,vector<pair<int,int>>,comp> q;
	q.push({1,2});
	q.push({2,3});
	cout<<q.top().first<<" "<<q.top().second<<endl;
	return 0;
}
