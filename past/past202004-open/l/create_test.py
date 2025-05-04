import random

n = random.randint(2,10)
d = random.randint(1,n)
k = random.randint(1,n)
print(n,k,d)
a = []
for i in range(n):
    a.append(random.randint(0,10))
print(" ".join(map(str,a)))