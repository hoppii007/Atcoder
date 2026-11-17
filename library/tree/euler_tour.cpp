#pragma once
/**
 * @brief オイラーツアー（入るとき v、出るとき ~v を記録）
 *
 *   euler_tour et(nexts, root) … 構築  O(N)
 *   et.tour                   … 訪問順の列（長さ 2N。入りは v、出は ~v = -v-1）
 *   et.in[v]                  … tour 上で v が現れる位置
 *   et.out[v]                 … tour 上で ~v が現れる位置
 *
 * @note v の部分木 = tour の閉区間 [in[v], out[v]]（半開区間のセグ木では get(in[v], out[v] + 1)）
 *
 * @code
 * euler_tour et(nexts, 0);
 * ll sub = (et.out[v] - et.in[v] + 1) / 2;  // v の部分木の頂点数
 * @endcode
 */
struct euler_tour {
	vll tour;
	vll in, out;

	euler_tour(vvll &nexts, ll root){
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