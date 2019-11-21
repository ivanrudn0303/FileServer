#include "Init.h"
#include "Talk.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>


uint32_t get_id(void)
{
	static uint32_t id = 0;
	return ++id;
}

int args_parse(const char* argv[], const int argc, arguments* res)
{
	char buf[64];
	char *div;
	int opt;
	res->addr.s_addr = INADDR_ANY;
	res->file = NULL;
	res->port = 0;
	res->addr.s_addr = INADDR_ANY;
	while((opt = getopt(argc, argv, "a:p:i:o:")) != -1)
	{
		switch(opt)
		{
			case 'a':
				if(strlen(optarg) >= 64)
					return 1;
				strcpy(buf, optarg);
				div = strchr(buf, ':');
				if(NULL != div)
				{
					*div = '\0';
					res->port = htons(atoi(div+1));
				}
				if(inet_pton(AF_INET, buf, &res->addr) != 1)
					return 1;
				break;
			case 'p':
				res->port = htons(atoi(optarg));
				break;
			case 'o':
				res->file = optarg;
				break;
			case 'i':
				res->file = optarg;
				break;
			case '?':
				return 2;
			default:
				return 0;
		}
	}
	return 0;
}
