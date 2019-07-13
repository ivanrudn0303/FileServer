#ifndef SERVER_FUNCS
#define SERVER_FUNCS

#include "Talk.h"

int create_sock_server(arguments* args);
int client_authorization(int conn_fd);
void give_client_id(int id);
void download(int conn_fd, char* buf, int* num_last_packet_recv);
FILE* get_new_file();
void write_to_file(char* buf, FILE* file, int num_last_packet_recv);

#endif
