#ifndef SERVER_FUNCS
#define SERVER_FUNCS

#include "Talk.h"
#include "Init.h"
#include <errno.h>
#include <fcntl.h>

#define MSG_TIMEOUT 2000


int create_sock_server(arguments* args);
int client_authorization(int conn_fd, size_t* file_len);
void give_client_id(int id, int conn);
int download(int conn_fd, int* num_last_packet_recv, int file, int* file_len, bool* finish);


#endif
