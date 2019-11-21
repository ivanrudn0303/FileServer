CC = gcc
BASE = ../src
INC = ${BASE}

all: client server test_init test_talk


Init.o: ${BASE}/Init.c
	${CC} -I${INC}/ -c ${BASE}/Init.c
test_init: Init.o uInit.o
	${CC} -o out_test_init Init.o uInit.o

uInit.o: ${BASE}/test/uInit.c
	${CC} -I${INC}/ -c ${BASE}/test/uInit.c

test_talk: client_funcs.o server_funcs.o Talk.o uTalk.o
	${CC} -o out_test_talk client_funcs.o server_funcs.o Talk.o uTalk.o

Talk.o: ${BASE}/Talk.c
	${CC} -I${INC}/ -c ${BASE}/Talk.c

uTalk.o: ${BASE}/test/uTalk.c
	${CC} -I${INC}/ -c ${BASE}/test/uTalk.c

server.o: ${BASE}/server.c
	${CC} -I${INC}/ -c ${BASE}/server.c

server_funcs.o: ${BASE}/server_funcs.c
	${CC} -I${INC}/ -c ${BASE}/server_funcs.c

server: Init.o server.o server_funcs.o Talk.o
	${CC} -o out_server Init.o server.o server_funcs.o Talk.o

client.o: ${BASE}/client.c
	${CC} -I${INC}/ -c ${BASE}/client.c

client_funcs.o: ${BASE}/client_funcs.c
	${CC} -I${INC}/ -c ${BASE}/client_funcs.c

client: Init.o client.o client_funcs.o Talk.o
	${CC} -o out_client Init.o client.o client_funcs.o Talk.o

clean:
	-rm -rf out* *.o
