// 1手ずつのビームサーチ(全ケース用の実験)
// 評価: 色ごとに、その色を含むマスを巣に近い順に並べ、巣か先に並んだ同色のマスまでの距離の最小を足す
// 幅1から始め、残り時間で収まる見込みの幅に広げて解き直す。最良の手順を出力する
#include <bits/stdc++.h>
using namespace std;

constexpr int MAXC = 400;
const double TL = 1800;   // 探索を打ち切る時刻(ms)
const int DI[4] = {-1, 1, 0, 0}, DJ[4] = {0, 0, -1, 1};
const char DC[4] = {'U', 'D', 'L', 'R'};

chrono::steady_clock::time_point start_time;
double elapsed() { return chrono::duration<double, milli>(chrono::steady_clock::now() - start_time).count(); }

int N, K, NN;
vector<char> is_floor;
vector<int> nest_of_cell, nest_pos;
vector<vector<short>> dist_;
vector<array<int, 4>> run_len;
uint64_t zob[MAXC][8][12];
mt19937_64 rng(1);

struct State {
    uint32_t st[MAXC];   // 4bit x 8段(0段目が一番下)
    uint8_t h[MAXC];
    uint64_t hash;
    int remain;
};

inline int color_at(const State &s, int x, int z) { return (s.st[x] >> (4 * z)) & 15; }
inline int shift_of(int d) { return DI[d] * N + DJ[d]; }

inline void go_home(State &s, int x) {
    int c = nest_of_cell[x];
    if (c < 0) return;
    while (s.h[x] > 0 && color_at(s, x, s.h[x] - 1) == c) {
        int z = s.h[x] - 1;
        s.hash ^= zob[x][z][c];
        s.st[x] &= ~(15u << (4 * z));
        s.h[x]--;
        s.remain--;
    }
}

inline void apply(State &s, int u, int k, int d, int l) {
    int hu = s.h[u];
    int v = u + shift_of(d) * l;
    int m = hu - k, g = s.h[v];
    for (int t = 0; t < m; t++) {
        int zf = hu - 1 - t;
        int c = color_at(s, u, zf);
        s.hash ^= zob[u][zf][c];
        s.st[u] &= ~(15u << (4 * zf));
        int zt = g + t;
        s.st[v] |= (uint32_t)c << (4 * zt);
        s.hash ^= zob[v][zt][c];
    }
    s.h[u] = k;
    s.h[v] = g + m;
    go_home(s, u);
    go_home(s, v);
}

inline int color_mask(const State &s, int x) {
    int m = 0;
    for (int z = 0; z < s.h[x]; z++) m |= 1 << color_at(s, x, z);
    return m;
}

// 色cの見込み手数(その色を含むマスの一覧 T から)
int color_cost(int c, vector<int> &T) {
    int nest = nest_pos[c];
    const auto &dn = dist_[nest];
    sort(T.begin(), T.end(), [&](int a, int b) { return dn[a] < dn[b]; });
    int tot = 0;
    for (size_t a = 0; a < T.size(); a++) {
        int x = T[a];
        if (x == nest) { tot += 1; continue; }   // 自分の巣で埋もれている
        int best = dn[x];
        const auto &dx = dist_[x];
        for (size_t b = 0; b < a && best > 1; b++) best = min(best, (int)dx[T[b]]);
        tot += best;
    }
    return tot;
}

struct Node { int parent; uint32_t mv; };
struct Cand { float key; int par; uint32_t mv; uint64_t hash; int eval; };

// 幅Wのビームサーチ。deadline を過ぎたら false(ただし must_finish なら幅1に落として続ける)
bool beam(int W, double noise, double deadline, bool must_finish, const State &init,
          vector<uint32_t> &out_moves, int &out_remain) {
    uniform_real_distribution<double> ur(0, 1);
    vector<State> cur{init}, nxt;
    vector<int> cur_eval;
    {
        int e = 0;
        vector<int> T[12];
        for (int x = 0; x < NN; x++) { int m = color_mask(init, x); for (int c = 0; c < K; c++) if (m >> c & 1) T[c].push_back(x); }
        for (int c = 0; c < K; c++) e += color_cost(c, T[c]);
        cur_eval.push_back(e);
    }
    vector<vector<Node>> back;   // 層ごとの親と手
    vector<Cand> cands;
    unordered_set<uint64_t> seen;
    vector<int> T[12], T2;
    int goal = -1;
    int best_layer_state = 0;
    while (goal < 0) {
        if (elapsed() > deadline) {
            if (!must_finish) return false;
            W = 1;   // 時間切れ: 幅1で最後まで進める
            if (elapsed() > TL + 100) break;
        }
        cands.clear();
        for (int p = 0; p < (int)cur.size(); p++) {
            const State &s = cur[p];
            // 親の色ごとのマス一覧と見込み
            for (int c = 0; c < K; c++) T[c].clear();
            for (int x = 0; x < NN; x++) {
                if (!s.h[x]) continue;
                int m = color_mask(s, x);
                for (int c = 0; c < K; c++) if (m >> c & 1) T[c].push_back(x);
            }
            int ccost[12];
            int base = 0;
            for (int c = 0; c < K; c++) { ccost[c] = color_cost(c, T[c]); base += ccost[c]; }
            State t;
            for (int u = 0; u < NN; u++) {
                int hu = s.h[u];
                if (!hu) continue;
                for (int k = 0; k < hu; k++) {
                    int m = hu - k;
                    int gm = 0;
                    for (int z = k; z < hu; z++) gm |= 1 << color_at(s, u, z);
                    for (int d = 0; d < 4; d++) {
                        int lm = min(k + 1, run_len[u][d]);
                        for (int l = 1; l <= lm; l++) {
                            int v = u + shift_of(d) * l;
                            if (s.h[v] + m > 8) continue;
                            t = s;
                            apply(t, u, k, d, l);
                            // 動いた色だけ見込みを計算し直す
                            int e = base;
                            int mu = color_mask(t, u), mv = color_mask(t, v);
                            for (int c = 0; c < K; c++) {
                                if (!(gm >> c & 1)) continue;
                                T2.clear();
                                for (int x : T[c]) if (x != u && x != v) T2.push_back(x);
                                if (mu >> c & 1) T2.push_back(u);
                                if (mv >> c & 1) T2.push_back(v);
                                e += color_cost(c, T2) - ccost[c];
                            }
                            float key = t.remain == 0 ? -1e9f : (float)(e + noise * ur(rng));
                            cands.push_back({key, p, (uint32_t)(u | k << 9 | d << 12 | l << 14), t.hash, e});
                        }
                    }
                }
            }
        }
        if (cands.empty()) break;
        int take = min((int)cands.size(), 3 * W);
        nth_element(cands.begin(), cands.begin() + take - 1, cands.end(), [](const Cand &a, const Cand &b) { return a.key < b.key; });
        sort(cands.begin(), cands.begin() + take, [](const Cand &a, const Cand &b) { return a.key < b.key; });
        nxt.clear();
        vector<Node> bk;
        vector<int> nev;
        seen.clear();
        for (int i = 0; i < take && (int)nxt.size() < W; i++) {
            auto &c = cands[i];
            if (!seen.insert(c.hash).second) continue;
            State t = cur[c.par];
            uint32_t x = c.mv;
            apply(t, x & 511, x >> 9 & 7, x >> 12 & 3, x >> 14 & 15);
            nxt.push_back(t);
            bk.push_back({c.par, c.mv});
            nev.push_back(c.eval);
            if (t.remain == 0 && goal < 0) goal = (int)nxt.size() - 1;
        }
        swap(cur, nxt);
        cur_eval = nev;
        back.push_back(move(bk));
        best_layer_state = 0;
        if (goal >= 0) break;
        if ((int)back.size() >= 100000) break;
    }
    int id = goal >= 0 ? goal : best_layer_state;
    out_remain = cur.empty() ? -1 : cur[id].remain;
    out_moves.clear();
    for (int dd = (int)back.size() - 1; dd >= 0; dd--) { out_moves.push_back(back[dd][id].mv); id = back[dd][id].parent; }
    reverse(out_moves.begin(), out_moves.end());
    return goal >= 0;
}

int main() {
    start_time = chrono::steady_clock::now();
    for (auto &a : zob) for (auto &b : a) for (auto &c : b) c = rng();
    cin >> N >> K;
    NN = N * N;
    is_floor.assign(NN, 0); nest_of_cell.assign(NN, -1); nest_pos.assign(K, -1);
    static State init;
    memset(&init, 0, sizeof(init));
    for (int i = 0; i < N; i++) {
        string row; cin >> row;
        for (int j = 0; j < N; j++) {
            int x = i * N + j;
            char ch = row[j];
            is_floor[x] = ch != '#';
            if (isupper(ch)) { nest_of_cell[x] = ch - 'A'; nest_pos[ch - 'A'] = x; }
            if (islower(ch)) { int c = ch - 'a'; init.st[x] = c; init.h[x] = 1; init.hash ^= zob[x][0][c]; init.remain++; }
        }
    }
    run_len.assign(NN, {});
    for (int x = 0; x < NN; x++) for (int d = 0; d < 4; d++) {
        int l = 0, i = x / N, j = x % N;
        while (is_floor[x]) {
            int ni = i + DI[d] * (l + 1), nj = j + DJ[d] * (l + 1);
            if (ni < 0 || nj < 0 || ni >= N || nj >= N || !is_floor[ni * N + nj]) break;
            l++;
        }
        run_len[x][d] = l;
    }
    dist_.assign(NN, vector<short>(NN, 1000));
    for (int s = 0; s < NN; s++) {
        if (!is_floor[s]) continue;
        auto &D = dist_[s];
        deque<int> q{s}; D[s] = 0;
        while (!q.empty()) {
            int x = q.front(); q.pop_front();
            for (int d = 0; d < 4; d++) if (run_len[x][d] >= 1) {
                int y = x + shift_of(d);
                if (D[y] > D[x] + 1) { D[y] = D[x] + 1; q.push_back(y); }
            }
        }
    }

    // 幅1で解き、かかった時間から次の幅を決めて解き直す
    vector<uint32_t> best, mv;
    int best_remain = INT_MAX;
    int W = 1, tries = 0;
    while (true) {
        double t0 = elapsed();
        int rem;
        bool first = best.empty();
        bool ok = beam(W, 1.0, TL, first, init, mv, rem);
        tries++;
        if (ok || first) {
            long long sc = (long long)mv.size() + 100000LL * rem, bs = best.empty() ? LLONG_MAX : (long long)best.size() + 100000LL * best_remain;
            if (sc < bs) { best = mv; best_remain = rem; }
        }
        if (!ok) break;
        double used = elapsed() - t0, left = TL - elapsed();
        if (used < 1) used = 1;
        int nextW = (int)(W * left / used * 0.7);   // 時間は幅にほぼ比例
        if (nextW <= W) break;
        W = nextW;
    }
    for (auto x : best) {
        int u = x & 511, k = x >> 9 & 7, d = x >> 12 & 3, l = x >> 14 & 15;
        printf("%d %d %d %c %d\n", u / N, u % N, k, DC[d], l);
    }
    cerr << "W=" << W << " tries=" << tries << " T=" << best.size() << " remain=" << best_remain << " time=" << elapsed() << "\n";
}
