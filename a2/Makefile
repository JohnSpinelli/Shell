
all: matmultp matformatter myshell

matmultp: matmultp.c multiply.c
	gcc -c matmultp.c -o matmultp.o
	gcc multiply.c -o multiply

matformatter: matformatter.c
	gcc -c matformatter.c -o matformatter.o

myshell: myshell.c
	gcc myshell.c -o myshell

make clean:
	rm -f *.o myshell matmult_t matformatter *.txt *~
