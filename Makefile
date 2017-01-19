simulator:
	gcc -Wall -std=c99 -o simulator simulator.c
clean:
	-rm -rf *.o simulator

