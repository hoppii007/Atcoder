
//trueだったら第1引数が手前に来る
//p2 < p1として作ると降順になる
//p1 < p2として作ると昇順になる

///////////////////////////////////////
//priority_queueは後ろからとることに注意//
///////////////////////////////////////

//mainに以下をコピー（型を入力）
priority_queue<$1,vector<$1>,comp> q;
//pushとtopとpopを使うからね

struct comp{
    bool operator()(const $1 &p1,const $1 &p2) const{
		return p1 < p2;
    }
};
