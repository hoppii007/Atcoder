
def damage_rate(agent, target, grid):
    height = len(grid)
    width = len(grid[0])
    rate = 1
    dist = abs(agent['x']-target['x']) + abs(agent['y']-target['y'])
    if dist > agent['range']*2:
        return 0
    if dist > agent['range']:
        rate = 0.5
    if target['HUNKER_DOWN']:
        rate *= 0.5
    
    #右壁
    if target['x']+1 < width and grid[target['y']][target['x']+1] == -2:
        if target['x']+2 < agent['x']:
            return rate * 0.25
        if target['x']+2 == agent['x'] and abs(target['y']-agent['y']) <= 1:
            return rate * 0.25
    #左壁
    if target['x']-1 >= 0 and grid[target['y']][target['x']-1] == -2:
        if agent['x'] < target['x']-2:
            return rate * 0.25
        if agent['x'] == target['x']-2 and abs(target['y']-agent['y']) <= 1:
            return rate * 0.25
    #上壁
    if target['y']-1 >= 0 and grid[target['y']-1][target['x']] == -2:
        if agent['y'] < target['y']-2:
            return rate * 0.25
        if agent['y'] == target['y']-2 and abs(target['x']-agent['x']) <= 1:
            return rate * 0.25
    # した壁
    if target['y']+1 < height and grid[target['y']+1][target['x']] == -2:
        if target['y']+2 < agent['y']:
            return rate * 0.25
        if target['y']+2 == agent['y'] and abs(target['x']-agent['x']) <= 1:
            return rate * 0.25
    
    #右壁
    if target['x']+1 < width and grid[target['y']][target['x']+1] == -1:
        if target['x']+2 < agent['x']:
            return rate * 0.5
        if target['x']+2 == agent['x'] and abs(target['y']-agent['y']) <= 1:
            return rate * 0.5
    #左壁
    if target['x']-1 >= 0 and grid[target['y']][target['x']-1] == -1:
        if agent['x'] < target['x']-2:
            return rate * 0.5
        if agent['x'] == target['x']-2 and abs(target['y']-agent['y']) <= 1:
            return rate * 0.5
    #上壁
    if target['y']-1 >= 0 and grid[target['y']-1][target['x']] == -1:
        if agent['y'] < target['y']-2:
            return rate * 0.5
        if agent['y'] == target['y']-2 and abs(target['x']-agent['x']) <= 1:
            return rate * 0.5
    # した壁
    if target['y']+1 < height and grid[target['y']+1][target['x']] == -1:
        if target['y']+2 < agent['y']:
            return rate * 0.5
        if target['y']+2 == agent['y'] and abs(target['x']-agent['x']) <= 1:
            return rate * 0.5
    return rate

def shoot(ops1, ops2, agents, grid):
    for ops in [ops1, ops2]:
        for op in ops:
            op = op.split(";")
            agent = None
            for i in range(len(op)):
                if i == 0:
                    agent = agents[int(op[0])]
                    continue
                kind = op[i].split(' ')[0]
                if kind == 'SHOOT':
                    target_id = int(op[i].split(' ')[1])
                    target = agents[target_id]
                    target['wetness'] += int(agent['power'] * damage_rate(agent, target, grid))
                    agent['cd'] = agent['cooldown']+1