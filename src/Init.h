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
#define SIZE_OF_MESSAGE 2*4
#define SIZE_OF_DATA 1024*1024
#define SIZE_OF_AUTH (sizeof(uint32_t) + sizeof(uint64_t))
#define SIZE_OF_ERROR 1024
#define SIZE_OF_FINISH 0
#define SIZE_TO_WRITE 4 * 1024


enum MESSAGE_TYPES
{
	AUTH_MESSAGE,
	ERROR_MESSAGE,
	DATA_MESSAGE,
	FINISH_MESSAGE
};


typedef struct
{
	const char* file;
	struct in_addr addr;
	uint16_t port;
} arguments;


struct auth
{
	uint32_t id;
	uint64_t size;
} __attribute__((packed));
typedef struct auth auth;

struct message
{
	uint32_t type;
	uint32_t size;
	union
	{
		auth auth_params;
		char err_str[SIZE_OF_ERROR];
		char data[SIZE_OF_DATA];
	};
} __attribute__((packed));
typedef struct message message;


int args_parse(const char* argv[], int argc, arguments* res);
uint32_t get_id(void);

#endif
