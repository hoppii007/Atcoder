import random

q = random.randint(3,10)
print(q)

abc = "abcdefghijklmnopqrstuvwxyz"
for i in range(q):
    t = random.randint(1,2)
    if t == 1:
        print(t, abc[random.randint(0,len(abc)-1)],random.randint(1,5))
    else:
        print(t, random.randint(1,10))