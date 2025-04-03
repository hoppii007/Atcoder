//visitは全てfalseで作っとく
//visit[root]はtrueにしとく
void dfs(vvll &nexts, vb &visit, ll now, ll prev = -1){
    for(ll next: nexts[now]) {
        if(next == prev) continue;
        if(visit[next]) continue;
        visit[next] = true;
        dfs(nexts, visit, next, now);
    }
}
