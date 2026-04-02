// ツアーの順番に頂点番号を記録（入ったところは v 、出たところは ~v）
// in と out を見れば逆引きも可能
struct EulerTour {
	vll tour;
	vll in, out;

	EulerTour(vvll &nexts, ll root){
		in.resize(nexts.size());
		out.resize(nexts.size());

		dfs(root, nexts);
	}

	void dfs(ll now, vvll& nexts, ll parent=-1){
		in[now] = tour.size();
		tour.pb(now);
		for(ll nxt: nexts[now]) if(nxt != parent) {
			dfs(nxt, nexts, now);
		}
		out[now] = tour.size();
		tour.pb(~now);
	}
};