# フォルダを指定すると、その中のcreate_tests.pyを実行してrandom_testsフォルダを作成し、main.cppとsolver.cppの結果を比較する

import sys
import os
import subprocess
import glob

testcase_num = 100

print('/////// start random test /////////////')

# ディレクトリ名（atcoder/abc/abc403/e、みたいな）（絶対パス）
dir_name = sys.argv[1]
print('dir_name:',dir_name)

# すでにrandom_testsフォルダがあるなら無視
if os.path.exists(os.path.join(dir_name, "random_tests")):
    print('random_tests is already exists')
    print('skip create random_tests')
# ないなら作る
else:
    print('create random_tests...')
    # create_test.pyがなければダメ
    if os.path.exists(os.path.join(dir_name, "create_test.py")) == False:
        print('error! create_test.py is not exists')
        exit()
    # solver.cppがなければダメ
    if os.path.exists(os.path.join(dir_name, "solver.cpp")) == False:
        print('error! solver.cpp is not exists')
        exit()
    # solver.cppをコンパイル
    subprocess.run(['/usr/local/bin/g++-12',os.path.join(dir_name,"solver.cpp"), '-o', os.path.join(dir_name,"solver.out")])
    
    os.mkdir(os.path.join(dir_name, "random_tests"))
    # testcase_num個のテストケースを生成
    for i in range(testcase_num):
        print(f'create case{i}')
        # まずは入力を生成
        output_str = subprocess.run(['python3', os.path.join(dir_name, "create_test.py")], capture_output=True, text=True).stdout
        with open(os.path.join(dir_name,"random_tests",f"{i}.in"), mode="w") as f:
            f.write(output_str)
        # 入力をもとに出力を生成
        output_str = subprocess.run([os.path.join(dir_name, 'solver.out')], input=output_str, capture_output=True, text=True).stdout
        with open(os.path.join(dir_name,'random_tests', f'{i}.out'), mode="w") as f:
            f.write(output_str)
    print('done!')

print('compile main.cpp ...')
# main.cppをコンパイル
subprocess.run(['/usr/local/bin/g++-12',os.path.join(dir_name,"main.cpp"), '-o', os.path.join(dir_name,"main.out")])
print('done!')

inputs = glob.glob(os.path.join(dir_name, "random_tests", '*.in'))
ng = 0
suc = 0

print('check all case...')
with open(os.path.join(dir_name, 'random_test_result.txt'), mode='w') as result:
    for i in range(len(inputs)):
        with open(os.path.join(dir_name,'random_tests',f'{i}.in'), mode='r') as f:
            inp = f.read()
        with open(os.path.join(dir_name,'random_tests',f'{i}.out'), mode='r') as f:
            outp = f.read()

        main_out = subprocess.run([os.path.join(dir_name, 'main.out')], input=inp, capture_output=True, text=True).stdout
        if outp == main_out:
            print(f'case{i}: success!',file=result)
            suc += 1
        else:
            print(f'case{i}: failed...',file=result)
            print('output:', file=result)
            print(main_out, file=result)
            print('', file=result)
            print('answer:', file=result)
            print(outp, file=result)
            print('', file=result)
            ng += 1
print('done!')
print(f'success --- {suc}')
print(f'failed ---- {ng}')