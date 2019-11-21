#include "Init.h"
#include "Talk.h"
#include "client_funcs.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>


#define TIMEOUT_CLIENT 2000
#define TIMEOF_WAIT_ERROR 5

int create_sock_client()
{
	int sock_client = -1;
	
	sock_client = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_client < 0)
  	{
    	perror("socket() failed");
    	exit(-1);
  	}

  	return sock_client;
}

int transmission(int file, int sock_fd, unsigned int len)
{
	char str[SIZE_OF_ERROR];
	message msg;
	unsigned int size = 0;
	int res = wrecv(TIMEOF_WAIT_ERROR, sock_fd, &msg, SIZE_OF_MESSAGE, 0);
	if (res < 0)
	{
		sprintf(str, "Error in recieving(transmission): errno = %d\n", errno);
		error_handler(sock_fd, str);
		return ERROR_RECV;
	} else if ((res > 0) && (ERROR_MESSAGE == msg.type))
	{
		res = wrecv(TIMEOF_WAIT_ERROR, sock_fd, &msg.data, msg.size, 0);
		if (msg.size == res)
			printf("Error from server(transmission): %s\n", msg.err_str);
		else
			printf("Error from server(transmission) and Error in recieving: res = %d, errno = %d\n", res, errno);
		return ERROR_EXTERNAL;
	}
	for (; size < len; size += res)
	{
		res = read(file, msg.data + size, len - size);
		if (res <= 0)
			break;
	}
	if (res < 0)
	{
		sprintf(str, "Error in reading from: errno = %d\n", errno);
		error_handler(sock_fd, str);
		return ERROR_READ;
	}
	else
	{
		msg.type = DATA_MESSAGE;
		msg.size = size;
		res = wsend(TIMEOUT_CLIENT, sock_fd, &msg, SIZE_OF_MESSAGE + size, 0);
		if (res < SIZE_OF_MESSAGE + size)
		{
			printf("Error in sending(transmission): size == %d res = %d, errno = %d\n", size, res, errno);
			return ERROR_SEND;
		}
	}
	return 0;
}

int finish(int sock_fd)
{
	char str[1024];
	message msg;
	int res = wrecv(TIMEOUT_CLIENT, sock_fd, &msg, SIZE_OF_MESSAGE, 0);
	if (res <= 0)
	{
		sprintf(str, "Error in recieving(finish): res = %d, errno = %d\n", res, errno);
		error_handler(sock_fd, str);
		return ERROR_RECV;
	}
	if (FINISH_MESSAGE != msg.type)
	{
		sprintf(str, "Error from server(finish): msg.type == %d\n", msg.type);
		error_handler(sock_fd, str);
		return ERROR_FINISH;
	}
	close(sock_fd);
	return 0;
}

int start_session(int sock_fd, uint32_t* id, unsigned int* len)
{
	char str[1024];
	int res;
	message msg;
	msg.type = AUTH_MESSAGE;
	msg.size = SIZE_OF_AUTH;
	msg.auth_params.id = *id;
	msg.auth_params.size = *len;
	res = wsend(TIMEOUT_CLIENT, sock_fd, &msg, SIZE_OF_MESSAGE + SIZE_OF_AUTH, 0);
	if (res < SIZE_OF_MESSAGE + SIZE_OF_AUTH)
	{
		printf("Error in sending(start_session): res = %d, errno = %d\n", res, errno);
		return ERROR_SEND;
	}
	res = wrecv(TIMEOUT_CLIENT, sock_fd, &msg, SIZE_OF_MESSAGE, 0);
	if (res <= 0)
	{
		sprintf(str, "Error in recieving(start_session): res = %d, errno = %d\n", res, errno);
		error_handler(sock_fd, str);
		return ERROR_RECV;
	} else if ((AUTH_MESSAGE != msg.type) || (SIZE_OF_AUTH != msg.size))
	{
		sprintf(str, "Error in auth: msg.type == %d msg.size == %d\n", msg.type, msg.size);
		error_handler(sock_fd, str);
		return ERROR_AUTH;
	}
	res = wrecv(TIMEOUT_CLIENT, sock_fd, &msg.auth_params, SIZE_OF_AUTH, 0);
	if (res < SIZE_OF_AUTH)
	{
		sprintf(str, "Error in recieving(start_session): res = %d, errno = %d\n", res, errno);
		error_handler(sock_fd, str);
		return ERROR_RECV;
	}
	if (0 == msg.auth_params.id)
	{
		printf("Error in auth: id == 0\n");
		return ERROR_AUTH;
	}
	*id = msg.auth_params.id;
	*len = msg.auth_params.size;
	return 0;
}
