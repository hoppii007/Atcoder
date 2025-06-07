#include<bits/stdc++.h>
//入力系
#define cinll(...) ll __VA_ARGS__; input(__VA_ARGS__);
#define cinint(...) int __VA_ARGS__; input(__VA_ARGS__);
#define cinstr(...) string __VA_ARGS__; input(__VA_ARGS__);
#define cinchar(...) char __VA_ARGS__; input(__VA_ARGS__);
#define cinvll(a,n) vll a(n); rep(i,n) cin>>a[i];
#define cinvs(a,n) vs a(n); rep(i,n) cin>>a[i];
//繰り返し系
#define rep1(n) for(int i=0;i<n;i++)
#define rep2(i,n) for(int i=0;i<n;i++)
#define rep3(i,a,n) for(int i=a;i<n;i++)
#define rep4(i,a,n,b) for(int i=a;i<n;i+=b)
#define overload4(a,b,c,d,e,...) e
#define rep(...) overload4(__VA_ARGS__,rep4,rep3,rep2,rep1)(__VA_ARGS__)
#define mrep1(n) for(int i=n;i>=0;i--)
#define mrep2(i,n) for(int i=n;i>=0;i--)
#define mrep3(i,n,a) for(int i=n;i>=a;i--)
#define mrep4(i,n,a,b) for(int i=n;i>=a;i-=b)
#define mrep(...) overload4(__VA_ARGS__,mrep4,mrep3,mrep2,mrep1)(__VA_ARGS__)
//iterator系
#define all(a) a.begin(),a.end()
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

const ll mod = 998244353LL;
const ll inf = 100100100100100100LL;

//便利系
//vectorをjoinするやつ
template<typename T>
string v_join(vector<T> &a,string c){
	string ret = "";
	rep(i,a.size()-1) ret += (to_string(a[i]) + c);
	return ret + to_string(a[a.size()-1]);
}
//空白区切り表示
template<class T, class... Ts>
void out(const T& a,const Ts&... b){
	cout<<a;
	(cout<< ... << (cout << ' ',b));
	cout<<endl;
}
//入力
template<class... T>
void input(T&... a){
	(cin >> ... >> a);
}

ll frombin(string s){
    //s = "1"+s+"1";
    ll n = s.size();
    ll ret = 0;
    ll now = 1;
    reverse(all(s));
    rep(i,n){
        if(s[i] == '1') ret+=now;
        now*=2LL;
    }
    return ret;
}

string tobin(ll a){
    string ret = "";
    while(a>0){
        if(a%2LL == 0) ret += "0";
        else ret += "1";
        a /= 2LL;
    }
    reverse(all(ret));
    return ret;
}

int main(void){
    cinll(a,b,c);
    ll cc = c;
    string ttc = tobin(c);
    ll ccount = 0;
    ll zcount = 0;
    rep(i,ttc.size()){
        if(ttc[i] == '1') ccount++;
        else zcount++;
    }
    ll ta = 0;
    ll tb = ccount;
    bool isExist = false;
    while(tb >= ta){
        if(tb - ta == max(a,b)-min(a,b)){
            isExist = true;
            break;
        }
        ta++;
        tb--;
    }
    if(isExist == false){
        cout<<"-1"<<endl;
        return 0;
    }
    ll dd = min(a,b)-min(ta,tb);
    if(dd < 0){
        cout<<"-1"<<endl;
        return 0;
    }
    if(dd - zcount - (60-(ll)ttc.size()) > 0){
        cout<<"-1"<<endl;
        return 0;
    }
    
    
    string tta = "";
    string ttb = "";
    rep(i,dd-zcount){
        tta += '1';
        ttb += '1';
    }
    dd = min(dd,zcount);
    
    //多い方にtb個、少ない方にta個
    if(a <= b){
        rep(i,ttc.size()){
            if(ttc[i] == '1'){
                if(ta > 0){
                    ta--;
                    tta += '1';
                    ttb += '0';
                }
                else{
                    tta += '0';
                    ttb += '1';
                }
            }
            else{
                if(dd > 0){
                    tta += '1';
                    ttb += '1';
                    dd--;
                }
                else{
                    tta += '0';
                    ttb += '0';
                }
                
            }
        }
    }
    else{
        rep(i,ttc.size()){
            if(ttc[i] == '1'){
                if(ta > 0){
                    ta--;
                    ttb += '1';
                    tta += '0';
                }
                else{
                    ttb += '0';
                    tta += '1';
                }
            }
            else{
                if(dd > 0){
                    tta += '1';
                    ttb += '1';
                    dd--;
                }
                else{
                    tta += '0';
                    ttb += '0';
                }
            }
        }
    }
    cout<<frombin(tta)<<" "<<frombin(ttb)<<endl;
    //(all(tta));
    //reverse(all(ttb));
    //reverse(all(ttc));
    //cout<<tta<<endl<<ttb<<endl<<ttc<<endl;
    
	return 0;
}
