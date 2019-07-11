#include "Init.h"
#include <stdlib.h>

#define CONN_LIMIT 17

int get_storage_index (int id) {
	return id % CONN_LIMIT
}

storage_elem* get_storage_elem (storage_elem* head, int id) {
	if (!head) return NULL;
	do {
		if (head->id == id) return head;
		head = head->next_elem;
	} while (head->next_elem)
	return NULL;
}


int create_sock_server(arguments* args) {
  	int sock_server = -1;
  	uint16_t port = args->port;
  	struct sockaddr_in addr;

  	sock_server = socket(AF_INET, SOCK_STREAM, 0);
  	if (sock_server < 0)
  	{
    	perror("socket() failed");
    	exit(-1);
  	}

	addr.sin_family = AF_INET; 
    addr.sin_addr.s_addr = htonl(args->addr); 
    addr.sin_port = htons(args->port);

  	if ((bind(sock_server, (struct sockaddr *)&addr, sizeof(addr))) < 0)
  	{
    	perror("bind() failed");
    	close(sock_server);
    	exit(-1);
  	}

  	if ((listen(sockfd, CONN_LIMIT)) != 0) { 
        perror("Listen failed...\n"); 
        exit(-1); 
    } 

	return sock_server;
}


int client_authorization(int conn_fd) {
	/*
		receive message with id from client
	*/
}


void resume_download(int conn_fd, int id, storage_elem* storage) {

}


void strart_download(int conn_fd, int id, storage_elem* storage) {

}
