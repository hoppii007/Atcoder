#pragma once
/**
 * @brief 木の重心
 *
 *   find_centroids(n, centroids, size_of_subtree, nexts, v) … 重心を centroids に追加  O(N)
 *
 * @param centroids       空で渡す。重心（1 個か 2 個）が入る
 * @param size_of_subtree サイズ n で渡す。v を根としたときの各頂点の部分木サイズが入る
 *
 * @code
 * vll centroids, sz(n);
 * find_centroids(n, centroids, sz, nexts, 0);
 * @endcode
 */
void find_centroids(const ll &n, vll &centroids, vll &size_of_subtree, vvll &nexts, ll v, ll parent = -1){
	//まずは自分をカウント
	size_of_subtree[v] = 1;
	//全子供のサイズがn/2以下か
	bool is_centroid = true;
	//全部の子供のやつを求める
	for(ll next : nexts[v]){
		//親にはいかなくていい
		if(next == parent) continue;
		//子供の部分木のサイズを求める
		find_centroids(n, centroids, size_of_subtree, nexts, next, v);
		//子供の部分木のサイズを自分のサイズに加算
		size_of_subtree[v] += size_of_subtree[next];
		//子供の部分木のサイズがn/2を超えたらアウト
		if(size_of_subtree[next] > n/2) is_centroid = false;
	}
	//自分より上のサイズもn/2を超えたらアウト
	if(n-size_of_subtree[v] > n/2) is_centroid = false;

	if(is_centroid == true) centroids.push_back(v);
}
