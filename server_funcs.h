#ifndef SERVER_FUNCS_H
#define SERVER_FUNCS_H

#include "Talk.h"
#include <errno.h>

#define MSG_TIMEOUT 100


int create_sock_server(arguments* args);
int client_authorization(int conn_fd, size_t* file_len);
void give_client_id(int id, int conn);
int download(int conn_fd, int* num_last_packet_recv, int file, int* file_len);


#endif
