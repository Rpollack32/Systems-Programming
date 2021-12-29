all: memgrind.c mymalloc.o
	gcc -o ./a.out mymalloc.c memgrind.c

mymalloc.o: mymalloc.c mymalloc.h
	gcc -c mymalloc.c

clean:
	rm mymalloc.o memgrind
