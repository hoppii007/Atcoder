import random
n = random.randint(1,20)
d = random.randint(1,n)
r = random.randint(1,n)
h = list(range(n))
for i in range(n):
    h[i] += 1
random.shuffle(h)
print(n,d,r)
print(" ".join(map(str,h)))