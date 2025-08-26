import random

# ステージ生成 //まず左半分を作って、右に対称にコピーする
def create_game():
    height = random.randint(6,10)
    width = height*2
    grid = [[0 for j in range(width)] for i in range(height)]
    for i in range(1, height-1):
        for j in range(1, width//2-1):
            r = random.random()
            if r <= 0.1:
                grid[i][j] = -2 #高いカバー
            elif r <= 0.2:
                grid[i][j] = -1 #低いカバー

    # エージェント生成
    agent_count = random.randint(3,5)
    if agent_count == 5:
        if random.random() <= 0.5:
            agent_count -= 1
    if agent_count == 4:
        if random.random() <= 0.5:
            agent_count -= 1
    live_player_count = agent_count
    live_enemy_count = agent_count
    agent_types = [
        {'cooldown': 1, 'power': 16, 'range': 4, 'bomb': 1},
        {'cooldown': 5, 'power': 24, 'range': 6, 'bomb': 0},
        {'cooldown': 2, 'power': 8, 'range': 2, 'bomb': 3},
        {'cooldown': 2, 'power': 16, 'range': 4, 'bomb': 2},
        {'cooldown': 5, 'power': 32, 'range': 2, 'bomb': 1},
    ]
    agents = {}
    for agent in range(agent_count):
        #左壁の中で空いてるところの中からランダムに選んで、追加する
        candidates = []
        for i in range(height):
            if grid[i][0] == 0:
                candidates.append(i)
        h = random.choice(candidates)
        grid[h][0] = agent+1
        agents[agent+1] = {
            'agent_id': agent+1,
            'player': 0,
            'cooldown': agent_types[agent]['cooldown'],
            'range': agent_types[agent]['range'],
            'power': agent_types[agent]['power'],
            'bomb': agent_types[agent]['bomb'],
            'cd': 0,
            'x': 0,
            'y': h,
            'wetness': 0,
        }

    # 右にコピー
    if random.random() <= 0.5:
        # 線対象にコピー
        for i in range(height):
            for j in range(width//2,width):
                if grid[i][width-1-j] > 0:
                    left_agent = grid[i][width-1-j]
                    grid[i][j] = left_agent+agent_count
                    agents[grid[i][j]] = {
                        'agent_id': grid[i][j],
                        'player': 1,
                        'cooldown': agent_types[left_agent-1]['cooldown'],
                        'range': agent_types[left_agent-1]['range'],
                        'power': agent_types[left_agent-1]['power'],
                        'bomb': agent_types[left_agent-1]['bomb'],
                        'cd': 0,
                        'x': j,
                        'y': i,
                        'wetness': 0,
                    }
                else:
                    grid[i][j] = grid[i][width-1-j]
    else:
        # 点対称にコピー
        for i in range(height):
            for j in range(width//2,width):
                if grid[height-1-i][width-1-j] > 0:
                    left_agent = grid[height-1-i][width-1-j]
                    grid[i][j] = left_agent+agent_count
                    agents[grid[i][j]] = {
                        'agent_id': grid[i][j],
                        'player': 1,
                        'cooldown': agent_types[left_agent-1]['cooldown'],
                        'range': agent_types[left_agent-1]['range'],
                        'power': agent_types[left_agent-1]['power'],
                        'bomb': agent_types[left_agent-1]['bomb'],
                        'cd': 0,
                        'x': j,
                        'y': i,
                        'wetness': 0,
                    }
                else:
                    grid[i][j] = grid[height-1-i][width-1-j]
    agent_count *= 2
    return height, width, agent_count, live_player_count, live_enemy_count, agents, grid
