//区間更新、区間クエリはこいつにおまかせ！
///////注意/////////
//・T,opT,idTがモノイドになっている
//・O,opO,idOがモノイドになっている
//・f(o,(opT(t1,t2))) = opT(f(o,t1),f(o,t2))
//・f(opO(o1,o2),t) = f(o1,f(op2,t))
//・f(idO,t) = t
////////////////////
//コンストラクタ　関数ポインタの部分は普通に関数名書くだけでOK
//LazySegmentTree<T,O>(T配列、T演算、T単位元、O演算、O単位元、更新作用(OxT -> T))
//LazySegmentTree<T,O>(T配列の数、T演算、T単位元、O演算、O単位元、更新作用(OxT -> T))
//関数
//change(int,int,O) : 閉区間を作用素oで更新
//get(int,int)閉区間
template<typename T,typename O>
class LazySegmentTree{
public:
    //要素のモノイド
    T idT;
    T (*opT)(T,T);
    vector<T> arrayT;
    //作用素のモノイド（この値について作用が一つ求まる）
    O idO;
    O (*opO)(O,O);
    vector<O> arrayO;
    //作用素から作用を作り、それをtに適用する
    T (*f)(O,T);
    
public:
    int l_child(int i){ return i*2+1; }
    int r_child(int i){ return i*2+2; }
    int get_parent(int i){ return (i%2 == 0) ? (i/2-1):(i/2); }
    LazySegmentTree(vector<T> &v, T (*opt)(T,T),T idt, O (*opo)(O,O), O ido, T (*tf)(O,T)){
        int sz = v.size();
        int n = 1;
        while(n < sz) n *= 2;
        this->idT = idt;
        this->opT = opt;
        this->idO = ido;
        this->opO = opo;
        this->arrayT = vector<T>(2*n-1,this->idT);
        this->arrayO = vector<O>(2*n-1,this->idO);
        this->f = tf;
        for(int i=0;i<sz;i++) this->arrayT[this->arrayT.size()/2+i] = v[i];
        for(int i=this->arrayT.size()/2-1;i>=0;i--){
            this->arrayT[i] = opT(this->arrayT[l_child(i)],this->arrayT[r_child(i)]);
        }
    }
    LazySegmentTree(int a, T (*opt)(T,T),T idt, O (*opo)(O,O), O ido, T (*tf)(O,T)){
        int n = 1;
        while(n < a) n*= 2;
        this->idT = idt;
        this->opT = opt;
        this->idO = ido;
        this->opO = opo;
        this->arrayT = vector<T>(2*n-1,this->idT);
        this->arrayO = vector<O>(2*n-1,this->idO);
        this->f = tf;
    }
    LazySegmentTree(){}
    void o_update(int k,O o){
        this->arrayT[k] = f(o,this->arrayT[k]);
        this->arrayO[k] = opO(o,this->arrayO[k]);
        //葉なら更新履歴をリセットする
        if(l_child(k) >= this->arrayT.size()) this->arrayO[k] = idO;
    }
    //[l,r] : 更新したい閉区間
    //o : 更新に使いたい作用素（[l,r]のすべてのtをf(o,t)にしたい）
    //k : 自分のインデックス
    //[nowL,nowR] : 自分がカバーしている閉区間
    void change(int l,int r,O o,int k=0,int nowL=-1,int nowR=-1){
        //親は最初から最後までカバー
        if(k == 0){
            nowL = 0;
            nowR = this->arrayT.size()/2;
        }
        //自分のカバー範囲と変更範囲が全く変わらないときは何もしない
        if(r < nowL || nowR < l){ return; }
        //自分のカバー範囲内全部を更新するとき
        if(l <= nowL && nowR <= r){
            //完全にかぶる場合、値と変更履歴を更新しておく
            o_update(k,o);
            return;
        }
        //自分の中のカバー範囲の1部が被ってるとき
        //自分の更新履歴を下に引き継ぐ
        int lc = l_child(k);
        int rc = r_child(k);
        //葉じゃないときは今溜まってる更新履歴を下に伝播
        if(lc < this->arrayT.size()){
            o_update(lc,this->arrayO[k]);
            o_update(rc,this->arrayO[k]);
            this->arrayO[k] = idO;
        }
        change(l,r,o,lc,nowL,(nowL+nowR)/2);
        change(l,r,o,rc,(nowL+nowR)/2+1,nowR);
        //自分の値を更新
        this->arrayT[k] = opT(arrayT[lc],arrayT[rc]);
        return;
    }
    //o : 親までの更新履歴
    T get(int l,int r,int k=0,int nowL=-1,int nowR=-1){
        //親は最初から最後までカバー
        if(k == 0){
            nowL = 0;
            nowR = this->arrayT.size()/2;
        }
        //カバー範囲が全く被らないときは単位元を返す
        if(r < nowL || nowR < l) return this->idT;
        //完全にかぶっているとき（葉の時は必ずここに来る）
        if(l <= nowL && nowR <= r){
            //葉なら更新は終わってるはずだから返すだけ
            if(l_child(k) >= this->arrayT.size()){
                return this->arrayT[k];
            }
            //葉じゃなくても更新が終わってるなら返す
            if(this->arrayO[k] == idO){
                return this->arrayT[k];
            }
            //葉じゃなくて更新も終わってないなら更新して一個下にだけ流して返す
            o_update(l_child(k),this->arrayO[k]);
            o_update(r_child(k),this->arrayO[k]);
            this->arrayO[k] = idO;
            return this->arrayT[k];
        }
        //更新履歴を下に流す
        int lc = l_child(k);
        int rc = r_child(k);
        //葉じゃないときは更新履歴を下に伝播
        if(lc < this->arrayT.size()){
            o_update(lc,this->arrayO[k]);
            o_update(rc,this->arrayO[k]);
            this->arrayO[k] = idO;
        }
        T vl = get(l,r,lc,nowL,(nowL+nowR)/2);
        T vr = get(l,r,rc,(nowL+nowR)/2+1,nowR);
        this->arrayO[k] = idO;
        this->arrayT[k] = opT(arrayT[lc],arrayT[rc]);
        return opT(vl,vr);
    }
    
    void showTree(){
        int n = 1;
        int j=0;
        while(n+j < this->arrayT.size()){
            while(j < n){
                cout<<"("<<this->arrayT[n+j-1].first<<","<<this->arrayT[n+j-1].second<<")"<<"/"<<this->arrayO[n+j-1]<<" ";
                j++;
            }
            cout<<endl;
            n *= 2;
            j = 0;
        }
    }
};
