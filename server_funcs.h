#ifndef SERVER_FUNCS
#define SERVER_FUNCS

#include "Talk.h"
#include "Init.h"
#include <errno.h>
#include <fcntl.h>

#define MSG_TIMEOUT 2000

#define ERROR_RECV 1 
#define ERROR_SEND 2 
#define ERROR_AUTH 3 
#define ERROR_SOCK 4 
#define ERROR_SLOW 5
#define ERROR_CONN 6
#define ERROR_WR_MISS 7
#define ERROR_WR 8
#define ERROR_CLIENT 9
#define ERROR_MSG_TYPE 10



int create_sock_server(arguments* args);
int client_authorization(int conn_fd, size_t* file_len);
int give_client_id(int id, uint32_t pos, int conn);
int download(int conn_fd, int* num_last_packet_recv, int file, int* file_len, bool* finish);


#endif
