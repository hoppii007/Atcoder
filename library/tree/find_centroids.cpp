#pragma once
//木の重心を求めるやつ
//木DPで全部の部分木の大きさを求めるやり方
//n : 木の頂点数
//centroids : 最初は空にしといて、重心の候補が入る
//size_of_subtree : 各頂点を根としたときの部分ぎの大きさ。最初は全部0にしとく
//nexts : 次に行ける場所全部
//これを呼び出すときに渡したvが根となる。
void findCentroids(const ll &n, vll &centroids, vll &size_of_subtree, vvll &nexts, ll v, ll parent = -1){
	//まずは自分をカウント
	size_of_subtree[v] = 1;
	//全子供のサイズがn/2以下か
	bool is_centroid = true;
	//全部の子供のやつを求める
	for(ll next : nexts[v]){
		//親にはいかなくていい
		if(next == parent) continue;
		//子供の部分木のサイズを求める
		findCentroids(n, centroids, size_of_subtree, nexts, next, v);
		//子供の部分木のサイズを自分のサイズに加算
		size_of_subtree[v] += size_of_subtree[next];
		//子供の部分木のサイズがn/2を超えたらアウト
		if(size_of_subtree[next] > n/2) is_centroid = false;
	}
	//自分より上のサイズもn/2を超えたらアウト
	if(n-size_of_subtree[v] > n/2) is_centroid = false;

	if(is_centroid == true) centroids.push_back(v);
}


//木の重心を求めるやつ
//存在証明に近いやつ（更新が終わったら重心になってるの証明ガチ天才）