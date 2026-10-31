//============================================================================
// AHC072 単一入力向けモンテカルロ探索(実験用、main.cpp とは独立)
//----------------------------------------------------------------------------
// ポテンシャル Φ (EVAL で切り替え)
//   EVAL=0: Σ(各スライムの巣までの床距離) + WG*(スライムのいるマス数) + WM*(塔の中の色の切れ目数)
//   EVAL=1: 塔を下から見て同色の連続(ラン)ごとに見込み手数を足す(同色はまとめて1回で運べる = 一気に運ぶ)。
//           下から b 番目に始まるランは下の b 匹を踏み台に b+1 マス先まで投げられるので、
//           見込み = min(d(今のマス), 1 + min_{着地点} d(着地点)) (切り離して巣の近くに投げる)。
//           これを throw_cost[c][cell][b] として前計算。Φ = Σ ラン見込み + WG*(マス数) + WM*(切れ目数)
// プレイアウト: 合法手すべての ΔΦ を計算し、exp(-ΔΦ/TEMP) に比例した確率で1手選ぶ。
// NMCS(Nested Monte Carlo Search): level L では各合法手について level L-1 を1回走らせ、
//   最良の手順の次の1手を指す。スライムのいるマス数(各マスから最低1回は跳ぶ)を下界に枝刈り。
// 使い方: mc.exe <time_sec> <level> <seed> [TEMP WG WM SLACK EVAL] < in > out   (最良手数は stderr)
//============================================================================
#include <bits/stdc++.h>
using namespace std;

int N, K, NN;
vector<int> is_floor, nest_color, nest_cell;
vector<vector<int>> dist_to;   // dist_to[c][cell]
vector<vector<array<int, 8>>> throw_cost;   // throw_cost[c][cell][b]: 下に b 匹いるランの見込み手数
int EVAL = 1;
double TEMP = 0.7, WG = 2.0, WM = 0.5;
int SLACK = 8;   // 枝刈りは best_total + SLACK 手で行う(内側の探索が何か返せるように)
const int INF = 1e9;
const int DI[4] = {-1, 1, 0, 0}, DJ[4] = {0, 0, -1, 1};
const char DC[4] = {'U', 'D', 'L', 'R'};

struct Move { uint8_t cell, k, d, l; };

struct State {
    array<uint8_t, 400> h{};
    array<array<int8_t, 8>, 400> st{};  // 下から上
    int remain = 0, occupied = 0;
};

mt19937_64 rng;
chrono::steady_clock::time_point t_start;
double elapsed() { return chrono::duration<double>(chrono::steady_clock::now() - t_start).count(); }
double time_limit;

double cell_cost(const State& s, int c) {
    int h = s.h[c];
    if (h == 0) return 0;
    double r = WG;
    if (EVAL == 0) {
        for (int t = 0; t < h; t++) {
            r += dist_to[s.st[c][t]][c];
            if (t > 0 && s.st[c][t] != s.st[c][t - 1]) r += WM;
        }
        return r;
    }
    for (int t = 0; t < h; t++) {
        if (t > 0 && s.st[c][t] == s.st[c][t - 1]) continue;   // ランの先頭(一番下)だけ数える
        if (t > 0) r += WM;
        r += throw_cost[s.st[c][t]][c][t];
    }
    return r;
}

void do_move(State& s, const Move& m) {
    int src = m.cell, h = s.h[src], cnt = h - m.k;
    int i = src / N + DI[m.d] * m.l, j = src % N + DJ[m.d] * m.l;
    int dst = i * N + j;
    int gd = s.h[dst];
    if (gd == 0) s.occupied++;
    for (int t = 0; t < cnt; t++) s.st[dst][gd + t] = s.st[src][h - 1 - t];
    s.h[dst] = gd + cnt;
    s.h[src] = m.k;
    if (m.k == 0) s.occupied--;
    for (int c : {src, dst}) {
        int nc = nest_color[c];
        if (nc < 0) continue;
        while (s.h[c] > 0 && s.st[c][s.h[c] - 1] == nc) { s.h[c]--; s.remain--; if (s.h[c] == 0) s.occupied--; }
    }
}

void gen_moves(const State& s, vector<Move>& out) {
    out.clear();
    for (int c = 0; c < NN; c++) {
        int h = s.h[c];
        if (h == 0) continue;
        int ci = c / N, cj = c % N;
        for (int d = 0; d < 4; d++) {
            // 方向 d に床が続く長さ
            int run = 0;
            for (int l = 1; l <= h; l++) {
                int i = ci + DI[d] * l, j = cj + DJ[d] * l;
                if (i < 0 || i >= N || j < 0 || j >= N || !is_floor[i * N + j]) break;
                run = l;
            }
            for (int l = 1; l <= run; l++) {
                int dst = (ci + DI[d] * l) * N + (cj + DJ[d] * l);
                int g = s.h[dst];
                for (int k = max(0, l - 1); k < h; k++) {
                    if (g + (h - k) > 8) continue;
                    out.push_back({(uint8_t)c, (uint8_t)k, (uint8_t)d, (uint8_t)l});
                }
            }
        }
    }
}

int dst_of(const Move& m) { return (m.cell / N + DI[m.d] * m.l) * N + (m.cell % N + DJ[m.d] * m.l); }

double delta(State& s, const Move& m) {
    int src = m.cell, dst = dst_of(m);
    auto hs = s.h[src], hd = s.h[dst];
    auto ss = s.st[src], sd = s.st[dst];
    int rem = s.remain, occ = s.occupied;
    double before = cell_cost(s, src) + cell_cost(s, dst);
    do_move(s, m);
    double after = cell_cost(s, src) + cell_cost(s, dst);
    s.h[src] = hs; s.h[dst] = hd; s.st[src] = ss; s.st[dst] = sd; s.remain = rem; s.occupied = occ;
    return after - before;
}

// プレイアウト: 手数 base_len から始めて bound 手未満で終わらなければ打ち切り。成功なら全長を返す
int playout(State s, int base_len, int bound, vector<Move>& seq) {
    seq.clear();
    vector<Move> mv; vector<double> w;
    int len = base_len;
    while (s.remain > 0) {
        if (len + s.occupied >= bound) return INF;
        gen_moves(s, mv);
        w.resize(mv.size());
        double mn = 1e18;
        for (size_t x = 0; x < mv.size(); x++) { w[x] = delta(s, mv[x]); mn = min(mn, w[x]); }
        double tot = 0;
        for (auto& v : w) { v = exp(-(v - mn) / TEMP); tot += v; }
        double r = uniform_real_distribution<double>(0, tot)(rng);
        size_t pick = 0;
        for (; pick + 1 < mv.size(); pick++) { r -= w[pick]; if (r <= 0) break; }
        do_move(s, mv[pick]);
        seq.push_back(mv[pick]);
        len++;
    }
    return len;
}

int best_total = INF;
vector<Move> best_total_seq;

void record(int len, const vector<Move>& prefix, const vector<Move>& rest) {
    if (len < best_total) {
        best_total = len;
        best_total_seq = prefix;
        best_total_seq.insert(best_total_seq.end(), rest.begin(), rest.end());
        fprintf(stderr, "[%.2fs] best %d\n", elapsed(), best_total);
    }
}

// NMCS: prefix は根からここまでの手順
int nmcs(State s, int level, vector<Move>& prefix, vector<Move>& out_seq) {
    if (level == 0) {
        int len = playout(s, prefix.size(), best_total + SLACK, out_seq);
        if (len < INF) record(len, prefix, out_seq);
        return len;
    }
    vector<Move> best_seq; int best_len = INF;
    vector<Move> played;  // この呼び出しで指した手
    vector<Move> mv, sub;
    while (s.remain > 0) {
        if (elapsed() > time_limit) break;
        if ((int)prefix.size() + s.occupied >= best_total + SLACK) break;
        gen_moves(s, mv);
        shuffle(mv.begin(), mv.end(), rng);
        for (auto& m : mv) {
            State t = s; do_move(t, m);
            prefix.push_back(m);
            int len = nmcs(t, level - 1, prefix, sub);
            prefix.pop_back();
            if (len < best_len) {
                best_len = len;
                best_seq = played; best_seq.push_back(m);
                best_seq.insert(best_seq.end(), sub.begin(), sub.end());
            }
            if (elapsed() > time_limit) break;
        }
        if (best_len == INF || best_seq.size() <= played.size()) break;
        Move nxt = best_seq[played.size()];
        do_move(s, nxt); played.push_back(nxt); prefix.push_back(nxt);
    }
    for (size_t x = 0; x < played.size(); x++) prefix.pop_back();
    out_seq = best_seq;
    return best_len;
}

int main(int argc, char** argv) {
    t_start = chrono::steady_clock::now();
    time_limit = argc > 1 ? atof(argv[1]) : 10.0;
    int level = argc > 2 ? atoi(argv[2]) : 1;
    rng.seed(argc > 3 ? atoll(argv[3]) : 1);
    if (argc > 4) TEMP = atof(argv[4]);
    if (argc > 5) WG = atof(argv[5]);
    if (argc > 6) WM = atof(argv[6]);
    if (argc > 7) SLACK = atoi(argv[7]);
    if (argc > 8) EVAL = atoi(argv[8]);

    cin >> N >> K; NN = N * N;
    is_floor.assign(NN, 0); nest_color.assign(NN, -1); nest_cell.assign(K, -1);
    State s0;
    for (int i = 0; i < N; i++) {
        string row; cin >> row;
        for (int j = 0; j < N; j++) {
            char ch = row[j]; int c = i * N + j;
            is_floor[c] = ch != '#';
            if ('A' <= ch && ch <= 'L') { nest_color[c] = ch - 'A'; nest_cell[ch - 'A'] = c; }
            if ('a' <= ch && ch <= 'l') { s0.h[c] = 1; s0.st[c][0] = ch - 'a'; s0.remain++; s0.occupied++; }
        }
    }
    dist_to.assign(K, vector<int>(NN, 1000));
    for (int c = 0; c < K; c++) {
        deque<int> q{nest_cell[c]}; dist_to[c][nest_cell[c]] = 0;
        while (!q.empty()) {
            int v = q.front(); q.pop_front();
            for (int d = 0; d < 4; d++) {
                int i = v / N + DI[d], j = v % N + DJ[d];
                if (i < 0 || i >= N || j < 0 || j >= N || !is_floor[i * N + j]) continue;
                int u = i * N + j;
                if (dist_to[c][u] > dist_to[c][v] + 1) { dist_to[c][u] = dist_to[c][v] + 1; q.push_back(u); }
            }
        }
    }

    throw_cost.assign(K, vector<array<int, 8>>(NN));
    for (int c = 0; c < K; c++)
        for (int v = 0; v < NN; v++) {
            if (!is_floor[v]) continue;
            for (int b = 0; b < 8; b++) {
                int best = dist_to[c][v];
                for (int d = 0; d < 4; d++)
                    for (int l = 1; l <= b + 1; l++) {
                        int i = v / N + DI[d] * l, j = v % N + DJ[d] * l;
                        if (i < 0 || i >= N || j < 0 || j >= N || !is_floor[i * N + j]) break;
                        best = min(best, 1 + dist_to[c][i * N + j]);
                    }
                throw_cost[c][v][b] = best;
            }
        }

    // 時間いっぱい NMCS を繰り返す(各反復は best_total を上限に枝刈り)
    long long iters = 0;
    while (elapsed() < time_limit) {
        vector<Move> prefix, seq;
        nmcs(s0, level, prefix, seq);
        iters++;
    }
    fprintf(stderr, "iters %lld best %d\n", iters, best_total);
    for (auto& m : best_total_seq)
        printf("%d %d %d %c %d\n", m.cell / N, m.cell % N, m.k, DC[m.d], m.l);
}
