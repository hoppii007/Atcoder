
import heapq

def astar(grid, start, goal):
    height = len(grid)
    width = len(grid[0])
    directions = [(-1,0), (1,0), (0,-1), (0,1)]

    def heuristic(a, b):
        return abs(a[0] - b[0]) + abs(a[1] - b[1])  # マンハッタン距離

    open_set = []
    heapq.heappush(open_set, (0 + heuristic(start, goal), 0, start))
    came_from = {}
    cost_so_far = {start: 0}

    while open_set:
        _, cost, current = heapq.heappop(open_set)

        if current == goal:
            # 経路を再構築
            path = [current]
            while current in came_from:
                current = came_from[current]
                path.append(current)
            path.reverse()
            return path

        for d in directions:
            ni = current[0] + d[0]
            nj = current[1] + d[1]
            next_coord = (ni, nj)

            if 0 <= ni < height and 0 <= nj < width:
                if next_coord != goal and grid[ni][nj] != 0:
                    continue  # スタート・ゴール以外は 0 でなければ通れない

                new_cost = cost + 1
                if next_coord not in cost_so_far or new_cost < cost_so_far[next_coord]:
                    cost_so_far[next_coord] = new_cost
                    priority = new_cost + heuristic(next_coord, goal)
                    heapq.heappush(open_set, (priority, new_cost, next_coord))
                    came_from[next_coord] = current

    return None  # 到達不能
