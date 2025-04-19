
//尺取り法
//区間の長さ$1(配列の要素数)
//区間の伸び縮みによって状態が変化する時に使える
//O(N)で条件を満たす全区間[0,n)を走査
//[l,r)でもdiscoveryが呼ばれることに注意（でも[n,n)では呼ばれない）
//区間[l,r)
ll l = 0;
ll r = 0;
//現状の区間[l,r)が条件を満たすか？
std::function<bool()> syaku_check = [&](){
    //区間がない時はtrueにする
    if(l == r) return true;
    
};
//rを追加する処理（r++は別でやるから、状態変化のみ）
std::function<void()> add_r = [&](){
    
};
//lを削除する処理（状態変化のみ）
std::function<void()> remove_l = [&](){
    
};
//OKな区間を見つけた時に（その状態の時に）やる処理
std::function<void()> discovery = [&](){
    
};
while(l < $1){                                                                                                                                                                           
    //rをなるたけ進める
    while(r < $1 && syaku_check()){
        //まずOKな列を一つ発見
        discovery();

        //rを区間に入れる処理
        add_r();

        //実際に区間を広げる
        r++;
    }

    //rを伸ばさなくてもOK or rを伸ばし切ってまだOKの時にここに来る
    if(syaku_check()) discovery();

    //一旦縮める
    remove_l();
    l++;
    while(r < l) r++;

    //lをokになるまで縮める(空の時はOK)
    while(!syaku_check()){
        remove_l();
        l++;
    }
    //これはなくて良いと思うけど一応
    while(r < l) r++;
}
