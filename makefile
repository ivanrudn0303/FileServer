CC = gcc

all: test_init test_talk


Init.o: Init.c
	${CC} -c Init.c
test_init: Init.o TestInit.o
	${CC} -o out_test_init Init.o TestInit.o

TestInit.o: TestInit.c
	${CC} -c TestInit.c

test_talk: Talk.o Init.o TestTalk.o
	${CC} -o out_test_talk Talk.o Init.o TestTalk.o

Talk.o: Talk.c
	${CC} -c Talk.c

TestTalk.o: TestTalk.c
	${CC} -c TestTalk.c

clean:
	-rm -rf out* *.o
