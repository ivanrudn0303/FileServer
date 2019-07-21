#include "Init.h"
#include "Macro.h"
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>

int main()
{
	arguments tmp;
	char *data[7];
	data[0] = "./test";
	data[1] = "-a";
	data[2] = "1.2.3.4:500";
	data[3] = "-i";
	data[4] = "name";
	data[5] = "-p";
	data[6] = "5";

	CHECK_EQUAL(args_parse(data, 5, &tmp), 0)
	CHECK_EQUAL(strcmp(tmp.file, "name"), 0)
	CHECK_EQUAL(ntohs(tmp.port), 500)
	CHECK_EQUAL(ntohl(tmp.addr.s_addr), 16909060)

	optind = 1;
	data[2] = "1.2.3.4";
	CHECK_EQUAL(args_parse(data, 7, &tmp), 0)
	CHECK_EQUAL(strcmp(tmp.file, "name"), 0)
	CHECK_EQUAL(ntohs(tmp.port), 5)
	CHECK_EQUAL(ntohl(tmp.addr.s_addr), 16909060)

	optind = 1;
	data[6] = "500";
	CHECK_EQUAL(args_parse(data + 4, 3, &tmp), 0)
	CHECK_EQUAL(tmp.file, NULL)
	CHECK_EQUAL(ntohs(tmp.port), 500)
	CHECK_EQUAL(ntohl(tmp.addr.s_addr), INADDR_ANY)

	CHECK_EQUAL(get_id(), 1)
	CHECK_EQUAL(get_id(), 2)
	printf("finished Init test\n");
	return 0;
}

