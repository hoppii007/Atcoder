import random
print(1)
n = random.randint(1,10000)
p = list(range(n))
random.shuffle(p)
print(n)
print(" ".join(map(str,p)))