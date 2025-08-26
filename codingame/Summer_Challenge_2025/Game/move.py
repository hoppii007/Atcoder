from astar import astar


# ops1で移動する奴らに対して、移動先を記録
def set_move_position(ops, agents, grid):
    for op in ops:
        op = op.split(";")
        agent = None
        for i in range(len(op)):
            if i == 0:
                agent = agents[int(op[0])]
                continue
            kind = op[i].split(' ')[0]
            if kind == 'MOVE':
                tx = int(op[i].split(' ')[1])
                ty = int(op[i].split(' ')[2])
                path = astar(grid,(agent['y'], agent['x']),(ty, tx))
                if not path:
                    continue
                agent['next_pos'] = path[1]

def move(ops1, ops2, agents, grid, live_agent_count):
    #確定人数
    confirmed = 0

    print("len(agents):",len(agents))

    #一旦動く位置を初期化して保存
    for i in range(len(agents)):
        if agents[i+1]['wetness'] >= 100: continue
        agents[i+1]['next_pos'] = (agents[i+1]['y'],agents[i+1]['x'])
        agents[i+1]['confirmed'] = False
    
    set_move_position(ops1, agents, grid)
    set_move_position(ops2, agents, grid)

    #静止するエージェントは確定
    for i in range(len(agents)):
        if agents[i+1]['wetness'] >= 100: continue
        if agents[i+1]['next_pos'] == (agents[i+1]['y'],agents[i+1]['x']):
            agents[i+1]['confirmed'] = True
            confirmed += 1
    #静止するエージェントとの衝突
    for i in range(len(agents)):
        if agents[i+1]['wetness'] >= 100: continue
        if agents[i+1]['confirmed']: continue
        #移動先に確定がいたら
        next_pos = list(agents[i+1]['next_pos'])
        tile_type = grid[next_pos[0]][next_pos[1]]
        if grid[next_pos[0]][next_pos[1]] > 0 and agents[tile_type]['confirmed']:
            print(f'{i+1}->{tile_type} collision!')
            agents[i+1]['confirmed'] = True
            confirmed += 1
    #更新がなくなるまで
    while True:
        cancel = 0
        #同じ場所に行く二人はキャンセル
        for i in range(len(agents)):
            if agents[i+1]['wetness'] >= 100: continue
            if agents[i+1]['confirmed']: continue
            for j in range(i+1,len(agents)):
                if agents[j+1]['wetness'] >= 100: continue
                if agents[j+1]['confirmed']: continue
                if agents[i+1]['next_pos'] == agents[j+1]['next_pos']:
                    cancel = 1
                    agents[i+1]['confirmed'] = True
                    agents[j+1]['confirmed'] = True
        #入れ替わる二人はキャンセル
        for i in range(len(agents)):
            if agents[i+1]['wetness'] >= 100: continue
            if agents[i+1]['confirmed']: continue
            for j in range(i+1,len(agents)):
                if agents[j+1]['wetness'] >= 100: continue
                if agents[j+1]['confirmed']: continue
                if agents[i+1]['next_pos'] == (agents[j+1]['y'],agents[j+1]['x']) and agents[j+1]['next_pos'] == (agents[i+1]['y'], agents[i+1]['x']):
                    cancel = 1
                    agents[i+1]['confirmed'] = True
                    agents[j+1]['confirmed'] = True
        if cancel == 0:
            break

    #衝突判定が終わったら確定してないやつを動かす
    for i in range(len(agents)):
        if agents[i+1]['wetness'] >= 100: continue
        if agents[i+1]['confirmed']: continue
        grid[agents[i+1]['y']][agents[i+1]['x']] = 0
        agents[i+1]['y'] = agents[i+1]['next_pos'][0]
        agents[i+1]['x'] = agents[i+1]['next_pos'][1]
        grid[agents[i+1]['y']][agents[i+1]['x']] = i+1
