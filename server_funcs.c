#include "Init.h"
#include "server_funcs.h"


int create_sock_server(arguments* args) {
  int sock_server = -1;
  uint16_t port = args->port;
  struct sockaddr_in addr;

  sock_server = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_server < 0)
  {
    perror("socket() failed");
    return -1;
  }

  addr.sin_family = AF_INET; 
  addr.sin_addr = args->addr; 
  addr.sin_port = args->port;

  if ((bind(sock_server, (struct sockaddr *)&addr, sizeof(addr))) < 0)
  {
    perror("bind() failed");
    close(sock_server);
    return -1;
  }

  if ((listen(sock_server, CONN_LIMIT)) != 0) { 
    perror("Listen failed...\n"); 
    return -1; 
  } 

  return sock_server;
}


int client_authorization(int conn_fd) {
  const void* buf = (const void*)malloc(sizeof(message));
  message* msg = NULL;
  int id;

  do {
    int res = wrecv(MSG_TIMEOUT, conn_fd, buf, sizeof(buf), 0);
    if (!res) {
      printf("Error while receiving.\n");
      //error_handler
      return -1;
    }
    msg = (message*)buf;
  } while (msg->type != AUTH_MESSAGE);
  //client sends message with id, new client send message with id equals 0
  id = msg->data[0];
  return id;
}

void give_client_id(int id, int conn_fd) {
  message* msg = (message*)malloc(sizeof(message));
  memset(msg, 0, sizeof(message));
  msg->type = AUTH_MESSAGE;
  msg->data[0] = id;
  int res = wsend(MSG_TIMEOUT, conn_fd, msg, sizeof(msg), 0);
  if (!res) {
    printf("Error while receiving.\n");
    //error_handler
    return;
  }
}


void download(int conn_fd, char* buf, int* num_last_packet_recv) {
  int n = *num_last_packet_recv;
  bool is_download_in_progress = true;
  const void* msg = (const void*)calloc(0, sizeof(message));
  message* tmp = NULL;
  int i = 0;
  while (is_download_in_progress) {
    int res = wrecv(MSG_TIMEOUT, conn_fd, msg, sizeof(buf), 0);
    if (!res) {
      printf("Error while receiving.\n");
      //error_handler
      return;
    }
    tmp = (message*)msg;
    for (i = 0; i < SIZE_OF_DATA; i++)
      buf[n * SIZE_OF_DATA + i] = tmp->data[i];
    n++;
  }
}


FILE* get_new_file() {
  
}


void write_to_file(char* buf, FILE* file, int num_last_packet_recv) {

}
