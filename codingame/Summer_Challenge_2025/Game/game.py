from create_game import create_game
from player import Player
from move import move
from hunker_down import hunker_down
from shoot import shoot
from astar import astar

import random
import subprocess
import os
import time
import select
import queue
import threading


# 最初の表示
height, width, agent_count, live_player_count, live_enemy_count, agents, grid = create_game()

p1 = Player(0, 'Player1')
p2 = Player(1, 'Player2')
    
# 実行
print('最初の入力流し込み中...')
p1.first_input(agent_count, agents, grid)
p2.first_input(agent_count, agents, grid)

def show_grid():
    for i in range(len(grid)):
        s = ""
        for j in range(len(grid[i])):
            if grid[i][j] == 0:
                s += "."
            elif grid[i][j] == -1:
                s += "="
            elif grid[i][j] == -2:
                s += "#"
            else:
                s += str(grid[i][j])
        print(s)

show_grid()

time.sleep(1)

while True:

    #クールダウンを減らす
    for i in range(len(agents)):
        agents[i+1]['cd'] = max(0, agents[i+1]['cd']-1)

    #生きているエージェントの情報を入力していく
    p1.turn_input(agent_count, live_enemy_count+live_player_count, live_player_count, agents)
    p2.turn_input(agent_count, live_enemy_count+live_player_count, live_enemy_count, agents)

    time.sleep(1)

    p1.print_err()
    p2.print_err()
    ops1 = p1.print_out(agents)
    ops2 = p2.print_out(agents)

    move(ops1, ops2, agents, grid, live_enemy_count+live_player_count)
    hunker_down(ops1, ops2, agents)
    shoot(ops1, ops2, agents, grid)

    #死亡判定
    for i in range(len(agents)):
        a = agents[i+1]
        if agents[i+1]['wetness'] < 100: continue
        if grid[a['y']][a['x']] == 0: continue
        grid[a['y']][a['x']] = 0

    show_grid()



# path = []
# for i in range(len(grid)):
#     s = ""
#     for j in range(len(grid[i])):
#         if (i,j) in path:
#             s += "@"
#         elif grid[i][j] == 0:
#             s += "."
#         elif grid[i][j] == -1:
#             s += "="
#         elif grid[i][j] == -2:
#             s += "#"
#         else:
#             s += str(grid[i][j])
#     print(s)