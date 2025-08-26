import subprocess
import queue
import threading
from astar import astar

class Player:
    # file_nameはPlayer1 or Player2
    def __init__(self, my_id, file_name):
        self.my_id = my_id
        self.file_name = file_name
        print('コンパイル中...')
        compile_result = subprocess.run(["/usr/local/bin/g++-12", f"/Users/owner/Desktop/Github/Atcoder/codingame/Summer_Challenge_2025/Game/{file_name}.cpp", "-o", file_name], capture_output=True, text=True)
        if compile_result.returncode != 0:
            print("コンパイルエラー:")
            print(compile_result.stderr)
            exit()
        print('done!')
        self.proc = subprocess.Popen([f"./{file_name}"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, bufsize=1)
    
        # stdout読み取り専用キュー
        self.std_q = queue.Queue()
        self.err_q = queue.Queue()
        # スレッドでstdoutを読み取る
        def read_stdout(proc, q):
            while True:
                line = proc.stdout.readline()
                if not line:
                    continue
                q.put(line.strip())
        def read_stderr(proc, q):
            while True:
                line = proc.stderr.readline()
                if not line:
                    continue
                q.put(line.strip())
        threading.Thread(target=read_stdout, args=(self.proc, self.std_q), daemon=True).start()
        threading.Thread(target=read_stderr, args=(self.proc, self.err_q), daemon=True).start()

    def first_input(self, agent_count, agents, grid):
        #エージェント情報入力
        self.proc.stdin.write(f'{self.my_id}\n')
        self.proc.stdin.flush()
        self.proc.stdin.write(f'{agent_count}\n')
        self.proc.stdin.flush()
        for i in range(agent_count):
            self.proc.stdin.write(f'{i+1} {agents[i+1]["player"]} {agents[i+1]["cooldown"]} {agents[i+1]["range"]} {agents[i+1]["power"]} {agents[i+1]["bomb"]}\n')
            self.proc.stdin.flush()
        self.proc.stdin.write(f'{len(grid[0])} {len(grid)}\n')
        self.proc.stdin.flush()
        #盤面入力
        for i in range(len(grid)):
            for j in range(len(grid[0])):
                tile_type = grid[i][j]
                if tile_type >= 0:
                    tile_type = 0
                else:
                    tile_type *= -1
                self.proc.stdin.write(f'{j} {i} {tile_type}\n')
                self.proc.stdin.flush()
    
    def print_err(self):
        received_lines = []
        while not self.err_q.empty():
            received_lines.append(self.err_q.get())
        # for line in received_lines:
        #     print(f'{self.file_name} err : {line.strip()}')
    
    def print_out(self, agents):
        received_lines = []
        while not self.std_q.empty():
            received_lines.append(self.std_q.get().strip())
        for line in received_lines:
            print(f'{self.file_name} out : {line}')
        
        return received_lines



    def turn_input(self, agent_count, live_agent_count, live_my, agents):
        self.proc.stdin.write(f'{live_agent_count}\n')
        self.proc.stdin.flush()
        cnt = 0
        for i in range(agent_count):
            if(agents[i+1]['wetness'] >= 100): continue
            cnt += 1
            self.proc.stdin.write(f'{i+1} {agents[i+1]["x"]} {agents[i+1]["y"]} {agents[i+1]["cd"]} {agents[i+1]["bomb"]} {agents[i+1]["wetness"]}\n')
            self.proc.stdin.flush()
        self.proc.stdin.write(f'{live_my}\n')
        self.proc.stdin.flush()