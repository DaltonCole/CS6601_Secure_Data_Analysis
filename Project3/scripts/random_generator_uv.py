from random import randrange

count = 100

with open("../vectorU.txt", 'w') as f:
	for line in range(count):
		f.write(str(randrange(0, 10000000000000000000000000000000)) + '\n')

with open("../vectorV.txt", 'w') as f:
	for line in range(count):
		f.write(str(randrange(0, 10000000000000000000000000000000)) + '\n')