import random
n = random.randint(3,10)
m = random.randint(1,10)
a = [str(random.randint(1,10)) for i in range(n)]
print(n,m)
print(" ".join(a))