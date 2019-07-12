#ifndef INIT_H
#define INIT_H

#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/socket.h>

#define CONN_LIMIT 17

typedef struct
{
	const char* file;
	struct in_addr addr;
	uint16_t port;
} arguments;

int args_parse(const char* argv[], int argc, arguments* res);
int create_sock_client();
int create_sock_server(arguments* args);
uint32_t get_id();

#endif
