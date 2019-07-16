#include "Init.h"
#include "server_funcs.h"


int create_sock_server(arguments* args) {
  int sock_server = -1;
  uint16_t port = args->port;
  struct sockaddr_in addr;

  sock_server = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_server < 0) {
    perror("socket() failed\n");
    return -1;
  }

  addr.sin_family = AF_INET; 
  addr.sin_addr = args->addr; 
  addr.sin_port = args->port;

  if ((bind(sock_server, (struct sockaddr *)&addr, sizeof(addr))) < 0) {
    perror("bind() failed\n");
    close(sock_server);
    return -1;
  }

  if ((listen(sock_server, CONN_LIMIT)) != 0) { 
    perror("Listen failed...\n"); 
    return -1; 
  } 

  return sock_server;
}


int client_authorization(int conn_fd, size_t* file_len) {
  const void* buf = (const void*)malloc(sizeof(message));
  message* msg = NULL;
  int id;
  int res = wrecv(MSG_TIMEOUT, conn_fd, buf, sizeof(message), 0);

  if (!res) {
    error_handler(conn_fd, "SERVER: Error while receiving auth message...\n");
    return -1;
  } else if (res == -1) {
    error_handler(conn_fd, "SERVER: Error while receiving auth message.\n");
    // process errno, think about way to make an informative desc
    return -1;
  }

  msg = (message*)buf;

  if (msg->type != AUTH_MESSAGE) {
    error_handler(conn_fd, "SERVER: Authorization failed, disconnect client.\n");
    close(conn_fd);
    return -1; //DONE if msg type not auth disconnect
  }

  //DONE client sends message with id, new client send message with id equals 0
  id = *(int*)msg->data; //DONE *(int*)data -- id cast to int.  Add length of file (int*) data [1]
  *file_len = (int*)msg->data[1];
  return id;
}


void give_client_id(int id, int conn_fd) {
  message msg;
  msg.type = AUTH_MESSAGE;
  *(int*)msg.data = id; 
  int res = wsend(MSG_TIMEOUT, conn_fd, (const void*)&msg, sizeof(message), 0);

  if (!res) {
    error_handler(conn_fd, "SERVER: Error while sending id message...\n");
    return;
  } else if (res == -1) {
    error_handler(conn_fd, "SERVER: Error while sending id message.\n");
    // process errno
    return;
  } // WONT DO: position to download from start in bytes, new client starts from the beginning 
}


int download(int conn_fd, int* num_last_packet_recv, int file, int* file_len) {
  int len = *file_len;
  bool is_download_in_progress = true;
  message msg;
  int i = 0;
  int file_pos = *num_last_packet_recv * SIZE_OF_DATA;

  if (lseek(file, file_pos, SEEK_SET) != file_pos) {
      error_handler(conn_fd, "SERVER: Cannot set position in file.\n");
      close(file);
      return -1;
  }

  while (is_download_in_progress) {
    int res = wrecv(MSG_TIMEOUT, conn_fd, (const void*)&msg, sizeof(message), 0);

    if (!res) {
      error_handler(conn_fd, "SERVER: Download speed is low, disconnect client.\n");
      close(conn_fd);
      return -1;
    } else if (res == -1) {
      error_handler(conn_fd, "SERVER: Error while receiving data message...\n"); 
      // process errno
    }

    switch (msg.type) {
      case DATA_MESSAGE:

        if (len / SIZE_OF_DATA) {
          write(file, msg.data, SIZE_OF_DATA);
          len -= SIZE_OF_DATA;
        } else {
          write(file, msg.data, len);
          if (!len) {
            error_handler(conn_fd, "SERVER: Error while writing file to disk.\n");
            return -1;
          }
          is_download_in_progress = false;
        }

        *num_last_packet_recv += 1;
        break;

      case ERROR_MESSAGE:
        printf("Error ocurred on client while sending data message.\n");

        break;
    } //DONE add write to file here, by chuncks
  }
  
}
