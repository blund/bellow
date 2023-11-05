


all:
	gcc -m32 main.c bellow.c -I. -ggdb -o bellow

g: all
	cat prelude.bl - | gdb -ex run --args ./bellow

e:
	gcc -m32 bellow.c -I. -E


run: all
	cat prelude.bl - | ./bellow
