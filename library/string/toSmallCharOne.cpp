
char toSmallCharOne(char c){
    if('a' <= c && c <= 'z') return c;
    else return c-'A'+'a';
}
