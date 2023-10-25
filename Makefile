


all:
	gcc -m32 main.c bellow.c -I. -ggdb -o bellow

g: all
	gdb ./bellow

e:
	gcc -m32 bellow.c -I. -E
	

run: all
	./bellow
