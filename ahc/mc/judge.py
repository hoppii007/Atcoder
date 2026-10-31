import sys
import os; sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
import bench
inp = open(sys.argv[1]).read().splitlines()
n, k = map(int, inp[0].split())
print(bench.judge(n, k, inp[1:], open(sys.argv[2]).read()))
