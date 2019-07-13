#include "Init.h"
#include "client_funcs.h"

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

