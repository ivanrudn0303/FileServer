#include "Init.h"
#include <stdlib.h>
#include <string.h>

uint32_t get_id()
{
	static uint32_t id = 0;
	return ++id;
}

int args_parse(const char* argv[], const int argc, arguments* res)
{
	int i = 0;
	res->addr.s_addr = INADDR_ANY;
        res->file = NULL;	
	for(i = 0; i < argc; ++i)
	{
		if(*argv[i] == '-')
		{
			if(i + 1 == argc)
				return 1;
			else if((0 == strcmp(argv[i], "-o")) || (0 == strcmp(argv[i], "-i")))
				res->file = argv[i+1];
			else if(0 == strcmp(argv[i], "-p"))
				res->port = htons(atoi(argv[i+1]));
			else if(0 == strcmp(argv[i], "-a"))
			{
				char buf[64];
				char *div;
				if(strlen(argv[i+1]) >= 64)
					return 1;
				strcpy(buf, argv[i+1]);
				div = strchr(buf, ':');
				if(NULL != div)
				{
					*div = '\0';
					res->port = htons(atoi(div+1));
				}
				if(inet_pton(AF_INET, buf, &res->addr) != 1)
					return 1;
			}
			i++;
		}
	}
	return 0;
}

int create_sock_server(arguments* args) {
  	int rc, on = 1;
  	int sock_server = -1;
  	uint16_t port = args->port;
  	struct in_addr addr = args->addr;


  	sock_server = socket(AF_INET6, SOCK_STREAM, 0);
  	if (sock_fd < 0)
  	{
    	perror("socket() failed");
    	exit(-1);
  	}

  	rc = setsockopt(sock_server, SOL_SOCKET,  SO_REUSEADDR,
    	            (char *)&on, sizeof(on));
  	if (rc < 0)
  	{
    	perror("setsockopt() failed");
    	close(sock_server);
    	exit(-1);
  	}

 	rc = ioctl(sock_server, FIONBIO, (char *)&on);
	if (rc < 0)
	{
 		perror("ioctl() failed");
 		close(sock_server);
 		exit(-1);
	}

	memset(&addr, 0, sizeof(addr));
	addr.sin6_family = AF_INET6;
	memcpy(&addr.sin6_addr, &addr, sizeof(addr));
	addr.sin6_port = htons(port);

	rc = bind(sock_server, (struct sockaddr *)&addr, sizeof(addr));
  	if (rc < 0)
  	{
    	perror("bind() failed");
    	close(sock_server);
    	exit(-1);
  	}

	return sock_server;
}


int create_sock_client(addr) {
	
}
