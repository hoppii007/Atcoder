
// O(log a)
bool isPrime(ll a){
    if(a <= 1) return false;
    if(a == 2) return true;

    for(ll i=2;i*i <= a;i++){
        if(a%i == 0) return false;
    }
    return true;
}
