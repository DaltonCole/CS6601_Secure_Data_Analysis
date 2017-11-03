u = []
with open("../vectorU.txt", 'r') as f:
	for line in f:
		u.append(int(line))

v = []
with open("../vectorV.txt", 'r') as f:
	for line in f:
		v.append(int(line))

total = 0
for i, j in zip(u, v):
	total += i * j

print(total)