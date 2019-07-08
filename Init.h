#ifndef INIT_H
#define INIT_H

#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/socket.h>

typedef struct
{
	const char* file;
	struct in_addr addr;
	uint16_t port;
} arguments;

int args_parse(const char* argv[], int argc, arguments* res);
uint32_t get_id();
int create_sock_server(arguments* args)

#endif
