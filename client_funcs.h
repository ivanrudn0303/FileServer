#ifndef CLIENT_FUNCS
#define CLIENT_FUNCS

#include "Init.h"

int create_sock_client();
//0-correct, func returns id and file position in *len
int start_session(int sock_fd, uint32_t* id, unsigned int* len);
//0-correct, func transmits data by sock_fd read from file, data size from pos to len
int transmission(int file, int sock_fd, unsigned int len);
//0-correct, terminates session and if correct, closes sock_fd
int finnish(int sock_fd);
#endif
