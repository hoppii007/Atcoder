import random

from collections import deque

def can_reach_all(wall_map, start):
    """
    wall_map: 2次元配列 (bool)。True = 壁、False = 空き
    start: [row, col] の座標
    戻り値: startからすべての空きマスに到達可能なら True、そうでなければ False
    """

    H, W = len(wall_map), len(wall_map[0])
    sr, sc = start

    # startが壁なら即False
    if wall_map[sr][sc]:
        return False

    # まず空きマスの総数を数える
    total_free = sum(row.count(False) for row in wall_map)

    # BFSで到達できる空きマスを数える
    visited = [[False]*W for _ in range(H)]
    q = deque()
    q.append((sr, sc))
    visited[sr][sc] = True
    reachable = 1  # start位置も含める

    directions = [(1,0), (-1,0), (0,1), (0,-1)]
    while q:
        r, c = q.popleft()
        for dr, dc in directions:
            nr, nc = r+dr, c+dc
            if 0 <= nr < H and 0 <= nc < W:
                if not wall_map[nr][nc] and not visited[nr][nc]:
                    visited[nr][nc] = True
                    q.append((nr, nc))
                    reachable += 1

    # 到達できた数が空きマス総数と一致すればOK
    return reachable == total_free

def create_and_write_input(file):
    N = random.randint(20, 40)
    K = max(1, random.randint(0, N*N // 6))
    ps = []
    for i in range(N):
        for j in range(N):
            if i == 0 and j == N//2: continue
            ps.append([i,j])
    random.shuffle(ps)
    TREE_MAP = [[False for j in range(N)] for i in range(i)]

    ps_idx = 0
    for _ in range(K):
        TREE_MAP[ps[ps_idx][0]][ps[ps_idx][1]] = True
        while can_reach(TREE_MAP, [0, N//2]) == False:
            TREE_MAP[ps[ps_idx][0]][ps[ps_idx][1]] = False
            ps_idx += 1
            TREE_MAP[ps[ps_idx][0]][ps[ps_idx][1]] = True

    ps_idx += 1
    Ti = ps[ps_idx][0]
    Tj = ps[ps_idx][1]

    sts = []
    for i in range(N):
        for j in range(N):
            if i == 0 and j == N//2: continue
            sts.append([i,j])
    random.shuffle(sts)

    print(f'{N} {Ti} {Tj}', file=file)
    for i in range(N):
        s = ""
        for j in range(N):
            if TREE_MAP[i][j]: s += "T"
            else:
                s += "."
                bank_idxs.append([i,j])
        print(s, file=file)
    for i in range(len(sts)):
        print(f'{sts[i][0]} {sts[i][1]}', file=file)

for i in range(100):
    with open(f"inputs/{i}.txt", "w", encoding="utf-8") as f:
        create_and_write_input(f)
        