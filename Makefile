build: 2048.o
	gcc 2048.o -lncurses -o 2048

2048.o: 2048.c
	gcc -c 2048.c

clean:
	rm -rf 2048.o 2048