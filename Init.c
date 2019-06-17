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
