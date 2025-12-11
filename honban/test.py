
import subprocess
import os
from concurrent.futures import ThreadPoolExecutor, as_completed

# C++ソースとコンパイル後の実行ファイル名
cpp_file = "main.cpp"
exe_file = "main.out"  # Windowsなら .exe, Linux/Macなら任意
dir_name = "/Users/owner/Desktop/Github/Atcoder/honban"


def run_case(i):
    # 1行目から n を取得
    with open(f"./inputs/{i}.txt", "r", encoding="utf-8") as f:
        lines = f.readlines()
    n = int(lines[0].strip().split()[0])
    input_data = "".join(lines)

    # subprocess に入力を渡す
    run_result = subprocess.run(
        [f"./{exe_file}"],
        input=input_data,          # ← stdin=f ではなく input=
        capture_output=True,
        text=True,
        check=True                 # エラーがあれば例外を投げる
    )

    stdout_lines = run_result.stdout.splitlines()

    with open(f'./outputs/{i}.txt', 'w', encoding='utf-8') as f:
        print("\n".join(stdout_lines), file=f)

    score = int(stdout_lines[-1])
    return n, i, score

total = 0
ma = 0

ave_low = 0
size_low = 0
ave_high = 0
size_high = 0

# 並列実行（最大8並列などに制御）
with ThreadPoolExecutor(max_workers=6) as executor:
    futures = [executor.submit(run_case, i) for i in range(100)]
    for future in as_completed(futures):
        n, i, score = future.result()
        total += score
        ma = max(ma, score)
        if(n < 30):
            ave_low += score
            size_low += 1
            print(f'i:{i}\tscore:{score}\tlow')
        else:
            ave_high += score
            size_high += 1
            print(f'i:{i}\tscore:{score}\thigh')

# 直列実行
# for i in range(100):
#     i, score = run_case(i)
#     print(f'i:{i} score:{score}')
#     total += score
#     ma = max(ma, score)

print(f'ave:{total/100} max:{ma} ave_low:{ave_low/size_low} ave_high:{ave_high/size_high}')