#include "Init.h"
#include "Macro.h"
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

	CHECK_EQUAL(args_parse(data, 4, &tmp), 0)
	CHECK_EQUAL(strcmp(tmp.file, "name"), 0)
	CHECK_EQUAL(ntohs(tmp.port), 500)
	CHECK_EQUAL(ntohl(tmp.addr.s_addr), 16909060)

	data[1] = "1.2.3.4";
	CHECK_EQUAL(args_parse(data, 6, &tmp), 0)
	CHECK_EQUAL(strcmp(tmp.file, "name"), 0)
	CHECK_EQUAL(ntohs(tmp.port), 5)
	CHECK_EQUAL(ntohl(tmp.addr.s_addr), 16909060)

	data[5] = "500";
	CHECK_EQUAL(args_parse(data + 4, 2, &tmp), 0)
	CHECK_EQUAL(tmp.file, NULL)
	CHECK_EQUAL(ntohs(tmp.port), 500)
	CHECK_EQUAL(ntohl(tmp.addr.s_addr), INADDR_ANY)

	CHECK_EQUAL(get_id(), 1)
	CHECK_EQUAL(get_id(), 2)
	printf("finished Init test\n");
        return 0;
}

