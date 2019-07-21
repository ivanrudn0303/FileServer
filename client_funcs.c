#include "Init.h"
#include "Talk.h"
#include "client_funcs.h"
#include <errno.h>
#include <stdio.h>


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
	message msg;
	int res = wrecv(TIMEOF_WAIT_ERROR, sock_fd, &msg, sizeof(msg), 0);
	if (res < 0)
	{
		printf("Error in recieving(transmission): errno = %d\n", errno);
		return 1;
	} else if ((res > 0) && (ERROR_MESSAGE == msg.type))
	{
		printf("Error from server(transmission): %s\n", msg.data);
		return 2;
	}
	res = read(file, &msg.data, len);
	if (res < 0)
	{
		sprintf(&msg.data, "Error in reading from: errno = %d\n", errno);
		printf(&msg.data);
		msg.type = ERROR_MESSAGE;
		wsend(TIMEOUT_CLIENT, sock_fd, &msg, sizeof(msg), 0);
		return 1;
	}
	else
	{
		msg.type = DATA_MESSAGE;
		res = wsend(TIMEOUT_CLIENT, sock_fd, &msg, sizeof(msg), 0);
		if (res < sizeof(msg))
		{
			printf("Error in sending(transmission): res = %d, errno = %d\n", res, errno);
			return 2;
		}
	}
	return 0;
}

int finish(int sock_fd)
{
	message msg;
	int res = wrecv(TIMEOUT_CLIENT, sock_fd, &msg, sizeof(msg), 0);
	if (res <= 0)
	{
		printf("Error in recieving(finnish): res = %d, errno = %d\n", res, errno);
		return 1;
	}
	if (ERROR_MESSAGE == msg.type)
	{
		printf("Error from server(finnish): %s\n", msg.data);
		return 2;
	}
	close(sock_fd);
	return 0;
}

int start_session(int sock_fd, uint32_t* id, unsigned int* len)
{
	int res;
	message msg;
	msg.type = AUTH_MESSAGE;
	((uint32_t*)msg.data)[0] = *id;
	((uint32_t*)msg.data)[1] = *len;
	res = wsend(TIMEOUT_CLIENT, sock_fd, &msg, sizeof(msg), 0);
	if (res < sizeof(msg))
	{
		printf("Error in sending(start_session): res = %d, errno = %d\n", res, errno);
		return 2;
	}
	res = wrecv(TIMEOUT_CLIENT, sock_fd, &msg, sizeof(msg), 0);
	if (res <= 0)
	{
		printf("Error in recieving(start_session): res = %d, errno = %d\n", res, errno);
		return 1;
	} else if ((AUTH_MESSAGE != msg.type) || (0 == ((uint32_t*)msg.data)[0]))
	{
		printf("Error in auth: msg.type == %d\n", msg.type);
		return 3;
	}
	*id = ((uint32_t*)msg.data)[0];
	*len = ((uint32_t*)msg.data)[1];
	return 0;
}
