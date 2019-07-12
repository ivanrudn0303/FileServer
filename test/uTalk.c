#include "Talk.h"
#include "Init.h"
#include "Macro.h"
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

int main()
{
	int fd_c, fd_s, pid;
	struct in_addr addr;
	int port = 44888;
	inet_pton(AF_INET, "127.0.0.1", &addr);
	int res;
	char buf[4*1024*1024] = {0};
	pid = fork();
	if (pid)
	{
		int fd_a;
		arguments arg = {NULL, addr, htons(port)};
		fd_s = create_sock_server(&arg);
		fd_a = accept(fd_s, NULL, NULL);
		res = wrecv(100, fd_a, buf, sizeof("test"), 0);
		CHECK_EQUAL(res, 0)

		res = wsend(100, fd_a, "test", sizeof("test"), 0);
		CHECK_EQUAL(res, sizeof("test"))

		res = wsend(100, fd_a, buf, sizeof(buf), 0);
		CHECK_NOT_EQUAL(res, sizeof(buf))
		CHECK_NOT_EQUAL(res < 0, 1)

		res = wsend(2000, fd_a, buf, sizeof(buf), 0);
		CHECK_EQUAL(res, -1)

		close(fd_a);
		close(fd_s);
		printf("finished Talk unittest\n");
	}
	else
	{
		struct sockaddr_in strct = {AF_INET, htons(port), addr};
		fd_c = create_sock_client();
		connect(fd_c, (struct sockaddr*)&strct, sizeof(strct));
		usleep(100000);

		res = wrecv(100, fd_c, buf, sizeof("test"), 0);
		CHECK_EQUAL(res, sizeof("test"))
		CHECK_EQUAL(strcmp(buf, "test"), 0)

		usleep(200000);
		close(fd_c);
	}
	return 0;
}
