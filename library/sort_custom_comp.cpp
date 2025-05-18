
//trueだったら第1引数が手前に来る
//p1 < p2として作ると昇順になる
//p2 < p1として作ると降順になる

//sort(all(v), compf);みたいに使ってね
std::function<bool($1&,$1&)> compf = [&](const $1 &p1,const $1 &p2) {
    return p1 < p2;
};
