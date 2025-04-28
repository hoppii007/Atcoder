# 一つ分のランダムなテストケースを標準出力する

import random
import os

charSet = "abcdefghijklmnopqrstuvwxyz"

#長さ1以上10以下の文字列を生成
def createWord():
    l = random.randint(1,10)
    s = ""
    for i in range(l):
        s += charSet[random.randint(0,len(charSet)-1)]
    return s

def createTest():
    q = random.randint(1,100)
    t = []
    s = []
    for i in range(q):
        t.append(random.randint(1,2))
        s.append(createWord())
    return q,t,s

q,t,s = createTest()

print(q)
for i in range(q):
    print(t[i],s[i])