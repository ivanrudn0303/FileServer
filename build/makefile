CC = gcc
BASE = ..
INC = ${BASE}

all: test_init test_talk


Init.o: ${BASE}/Init.c
	${CC} -I${INC}/ -c ${BASE}/Init.c
test_init: Init.o uInit.o
	${CC} -o out_test_init Init.o uInit.o

uInit.o: ${BASE}/test/uInit.c
	${CC} -I${INC}/ -c ${BASE}/test/uInit.c

test_talk: Talk.o Init.o uTalk.o
	${CC} -o out_test_talk Talk.o Init.o uTalk.o

Talk.o: ${BASE}/Talk.c
	${CC} -I${INC}/ -c ${BASE}/Talk.c

uTalk.o: ${BASE}/test/uTalk.c
	${CC} -I${INC}/ -c ${BASE}/test/uTalk.c

clean:
	-rm -rf out* *.o
