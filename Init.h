#ifndef INIT_H
#define INIT_H

#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <strings.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <unistd.h>


#define CONN_LIMIT 17
#define SIZE_OF_DATA 1000


enum MESSAGE_TYPES
{
	AUTH_MESSAGE,
	ERROR_MESSAGE,
	DATA_MESSAGE
};


typedef struct
{
	const char* file;
	struct in_addr addr;
	uint16_t port;
} arguments;


typedef struct 
{
	enum MESSAGE_TYPES type;
	char data [SIZE_OF_DATA];
} message;


int args_parse(const char* argv[], int argc, arguments* res);
uint32_t get_id(void);

#endif
