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


typedef struct 
{
	storage_elem* next_elem;
	int id;
	void* data;
} storage_elem;


int args_parse(const char* argv[], int argc, arguments* res);
uint32_t get_id();
int create_sock_server(arguments* args);
int client_authorization(int conn_fd, int id);
void resume_download(int conn_fd, int id, storage_elem* storage);
void strart_download(int conn_fd, int id, storage_elem* storage);


#endif
