#include "Init.h"
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>

int main()
{
        arguments tmp;
        char *data[6];
	data[0] = "-a";
	data[1] = "1.2.3.4:500";
	data[2] = "-i";
	data[3] = "name";
	data[4] = "-p";
	data[5] = "5";

	if(args_parse(data, 4, &tmp))
		printf("error: case1\n");
	if((0 != strcmp(tmp.file, "name")) || (500 != ntohs(tmp.port)) || (16909060 != ntohl(tmp.addr.s_addr)))
		printf("error: case1\n");

	data[1] = "1.2.3.4";
	if(args_parse(data, 6, &tmp))
		printf("error: case2\n");
	if((0 != strcmp(tmp.file, "name")) || (5 != ntohs(tmp.port)) || (16909060 != ntohl(tmp.addr.s_addr)))
		printf("error: case2\n");

	data[5] = "500";
	if(args_parse(data + 4, 2, &tmp))
		printf("error: case3\n");
	if((NULL != tmp.file) || (500 != ntohs(tmp.port)) || (INADDR_ANY != ntohl(tmp.addr.s_addr)))
		printf("error: case3\n");

	if(1 != get_id())
		printf("error: case4\n");
	if(2 != get_id())
		printf("error: case5\n");
	printf("finish\n");
        return 0;
}

