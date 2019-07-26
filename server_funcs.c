#include "Init.h"
#include "server_funcs.h"


int process_connection_errors(int conn_fd, int res, char* error_str, char* func_name, int msg_size);
int process_write_errors(int conn_fd, int res, char* error_str, char* func_name, int size);


int create_sock_server(arguments* args) {
  int sock_server = -1;
  uint16_t port = args->port;
  struct sockaddr_in addr;

  sock_server = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_server < 0) {
    printf("socket() failed\n");
    return -1;
  }

  addr.sin_family = AF_INET; 
  addr.sin_addr = args->addr; 
  addr.sin_port = args->port;

  if ((bind(sock_server, (struct sockaddr *)&addr, sizeof(addr))) < 0) {
    printf("bind() failed\n");
    close(sock_server);
    return -1;
  }

  if ((listen(sock_server, CONN_LIMIT)) != 0) { 
    printf("Listen failed...\n"); 
    return -1; 
  } 

  return sock_server;
}


int client_authorization(int conn_fd, uint32_t* id, size_t* file_len) {
  message  msg;
  char error_str[1024];
  char func_name [] = "SERVER (client_authorization) error:";
  int error_code;
  int res = wrecv(MSG_TIMEOUT, conn_fd, &msg, sizeof(message), 0);

  if((error_code = process_connection_errors(conn_fd, res, error_str, func_name, sizeof(message)))) {
    return error_code;
  }

  if (msg.type != AUTH_MESSAGE) {
    sprintf(error_str, "%s authorization protocol is broken, disconnect client. Type of message received: %d.\n", func_name, msg.type);
    error_handler(conn_fd, error_str);
    return ERROR_AUTH; 
  }

  char* buf = (char*)malloc(msg.size);
  res = wrecv(MSG_TIMEOUT, conn_fd, buf, msg.size, 0);

  if ((error_code = process_connection_errors(conn_fd, res, error_str, func_name, msg.size))) {
    free(buf);
    return error_code;
  }
  
  *id = ((uint32_t*)buf)[0];
  *file_len = ((uint32_t*)buf)[1];

  if (*file_len == 0) {
    sprintf(error_str, "%s file length equals 0, disconnect client.\n", func_name);
    error_handler(conn_fd, error_str);
    free(buf);
    return ERROR_AUTH;

  }

  printf("Authorization is completed. Client id: %d, file length: %zu\n", *id, *file_len);
  free(buf);
  return 0;
}


int give_client_id(int id, uint32_t pos, int conn_fd) {
  int buf_size = sizeof(message) + sizeof(uint32_t) * 2;
  char buf[sizeof(message) + sizeof(uint32_t) * 2];
  int error_code;
  int res;
  char error_str[1024];
  char func_name [] = "SERVER (give_client_id) error:";
  ((uint32_t*)buf)[0] = AUTH_MESSAGE;
  ((uint32_t*)buf)[1] = sizeof(uint32_t) * 2;
  ((uint32_t*)buf)[2] = id;
  ((uint32_t*)buf)[3] = pos;
  res = wsend(MSG_TIMEOUT, conn_fd, &buf, sizeof(buf), 0);

  if ((error_code = process_connection_errors(conn_fd, res, error_str, func_name, buf_size))) {
    return error_code;
  }

  printf("Id message was sent to client. The id given: %d\n", id); 
  return 0;
}


int download(int conn_fd, int* num_last_packet_recv, int file, int* file_len, bool* finish) {
  char write_buf[SIZE_TO_WRITE];
  char download_buf[SIZE_OF_DATA];
  int error_code;
  char error_str[1024];
  char func_name [] = "SERVER (download) error:";
  int res;
  bool is_download_in_progress = true;
  message msg;
  int sizeof_buf = SIZE_OF_DATA;
  int chunk = 0;
  int expected_size = 0;
  printf("File length: %d\n", *file_len);

  while (is_download_in_progress) {
    res = wrecv(MSG_TIMEOUT, conn_fd, &msg, sizeof(message), 0);

    if ((error_code = process_connection_errors(conn_fd, res, error_str, func_name, sizeof(message)))) {
      return error_code;
    }

    printf("Message type: %d\n", msg.type);

    switch (msg.type) {
      case DATA_MESSAGE:
        sizeof_buf = msg.size;
        
        if (sizeof_buf < SIZE_OF_DATA) {
          *finish = true;
          is_download_in_progress = false;
        }

        printf("Start download...\n");
        res = wrecv(MSG_TIMEOUT, conn_fd, &download_buf, sizeof_buf, 0);

        if ((error_code = process_connection_errors(conn_fd, res, error_str, func_name, sizeof_buf))) {
          return error_code;
        }

        printf("Writing to file packge #%d...\n", *num_last_packet_recv);

        expected_size = SIZE_TO_WRITE;

        for (; chunk < sizeof_buf; chunk += SIZE_TO_WRITE) {

          if ((sizeof_buf - chunk) < SIZE_TO_WRITE) {
            res = write(file, download_buf + chunk, sizeof_buf - chunk);
            expected_size = sizeof_buf - chunk;
          } else
            res = write(file, download_buf + chunk, SIZE_TO_WRITE);

          if ((error_code = process_write_errors(conn_fd, res, error_str, func_name, expected_size))) {
            return error_code;
          } 

        }

        res = fsync(file);

        if (res) {
          sprintf(error_str, "%s %s.\n", func_name, strerror(errno));
          error_handler(conn_fd, error_str);
        }

        if (*finish) {
          msg.type = FINISH_MESSAGE;
          msg.size = 0;
          res = wsend(MSG_TIMEOUT, conn_fd, &msg, sizeof(message), 0);

          if ((error_code = process_connection_errors(conn_fd, res, error_str, func_name, sizeof(message)))) {
            return error_code;
          }
        }

        chunk = 0;
        *num_last_packet_recv += 1;
        break;

      case ERROR_MESSAGE:
        printf("Error occurred on client: ");

        res = wrecv(MSG_TIMEOUT, conn_fd, &error_str, msg.size, 0);

        if ((error_code = process_connection_errors(conn_fd, res, error_str, func_name, sizeof(message)))) {
          return error_code;
        }

        printf("%s\n", error_str);
        return ERROR_CLIENT;

      default:
        sprintf(error_str, "Error, unexpected message type: %d.\n", msg.type);

        res = wsend(MSG_TIMEOUT, conn_fd, &error_str, strlen(error_str), 0);

        if ((error_code = process_connection_errors(conn_fd, res, error_str, func_name, sizeof(message)))) {
          return error_code;
        }

        return ERROR_MSG_TYPE;

    }
  }
  return 0;
  
}


int process_connection_errors(int conn_fd, int res, char* error_str, char* func_name, int msg_size) {

  if ((res >= 0) && (res < msg_size)) {
    sprintf(error_str, "%s download speed is low, reconnect. res: %d\n", func_name, res);
    error_handler(conn_fd, error_str);
    return ERROR_SLOW;
  } else if (res == -1) {
    sprintf(error_str, "%s %s\n", func_name, strerror(errno));
    error_handler(conn_fd, error_str);
    return ERROR_CONN;
  }

  return 0;
}


int process_write_errors(int conn_fd, int res, char* error_str, char* func_name, int size) {

  if ((res >= 0) && (res < size)) {
    sprintf(error_str, "%s some data was missed while writing to file.\n", func_name);
    error_handler(conn_fd, error_str);
    return ERROR_WR_MISS;
  } else if (res == -1) {
    sprintf(error_str, "%s %s.\n", func_name, strerror(errno));
    error_handler(conn_fd, error_str);
    return ERROR_WR;
  }

  return 0;
}
