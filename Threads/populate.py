import random

fin=open("testfile.txt","wt")

for i in range(int(2048*4)):
	line = str(random.randint(1,100))
	fin.write(line)

fin.close()
