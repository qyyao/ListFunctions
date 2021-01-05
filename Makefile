all:
	gcc -o test list.h list.c main.c

clean:
	rm test