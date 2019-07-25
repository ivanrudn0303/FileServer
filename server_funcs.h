#ifndef SERVER_FUNCS
#define SERVER_FUNCS

#include "Talk.h"
#include "Init.h"
#include <errno.h>
#include <fcntl.h>

#define MSG_TIMEOUT 2000

#define ERROR_RECV 1 // could not receive msg when wrecv returned -1, put errno to desc
#define ERROR_SEND 2 // could not receive msg when wsend returned -1, put errno to desc
#define ERROR_AUTH 3 // authorization of client failed, use error_handler
#define ERROR_SOCK 4 // cmd arguments are incorrect
#define ERROR_SLOW 5
#define ERROR_CONN 6



int create_sock_server(arguments* args);
int client_authorization(int conn_fd, size_t* file_len);
int give_client_id(int id, int conn);
int download(int conn_fd, int* num_last_packet_recv, int file, int* file_len, bool* finish);


#endif
