CC = gcc

all: test_init


Init.o: Init.c
	${CC} -c Init.c
test_init: Init.o TestInit.o
	${CC} -o out_test_init Init.o TestInit.o

TestInit.o: TestInit.c
	${CC} -c TestInit.c 
clean:
	-rm -rf out* *.o
