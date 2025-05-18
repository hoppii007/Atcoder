
//trueだったら第1引数が手前に来る
//p2 < p1として作ると降順になる
//p1 < p2として作ると昇順になる

struct set_comp{
    bool operator()(const $1 &p1,const $1 &p2) const{
      return p1 < p2;
    }
};
//mainにコピーしてね（型入力）
set<$1,set_comp> s;
