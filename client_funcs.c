#include "Init.h"
#include "Talk.h"
#include "client_funcs.h"
#include <errno.h>
#include <stdio.h>


#define TIMEOUT_CLIENT 1000*3600

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
	int res = wrecv(5, sock_fd, &msg, sizeof(msg), 0);
	if (res < 0)
	{
		printf("Error in recieving: errno = %d\n", errno);
		return 1;
	} else if ((res > 0) && (res < sizeof(msg)))
	{
		res = wrecv(TIMEOUT_CLIENT, sock_fd, &msg + res, sizeof(msg) - res, 0);
		if (res < 0)
		{
			printf("Error in recieving: errno = %d\n", errno);
			return 1;
		}
		if (ERROR_MESSAGE == msg.type)
		{
			printf("error from server: %s\n", msg.data);
			return 2;
		}
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
			printf("Error in sending: res = %d, errno = %d\n", res, errno);
			return 2;
		}
	}
	return 0;
}

int finnish(int sock_fd)
{
	message msg;
	int res = wrecv(TIMEOUT_CLIENT, sock_fd, &msg, sizeof(msg), 0);
	if (res < 0)
	{
		printf("Error in recieving: errno = %d\n", errno);
		return 1;
	}
	if (ERROR_MESSAGE == msg.type)
	{
		printf("error from server: %s\n", msg.data);
		return 2;
	}
	close(sock_fd);
	return 0;
}

int start_session(int sock_fd, uint32_t* id, unsigned int* len)
{
	return 0;
}
