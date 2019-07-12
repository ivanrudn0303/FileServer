#include "Talk.h"
#include "Init.h"
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
		if (0 != res)
                        printf("error: case1: %d != %d\n", res, 0);

		res = wsend(100, fd_a, "test", sizeof("test"), 0);
		if (sizeof("test") != res)
			printf("error: case2: %d != %d\n", sizeof("test"), res);

		res = wsend(100, fd_a, buf, sizeof(buf), 0);
		if ((res >= sizeof(buf)) || (res < 0))
			printf("error: case5: %d\n", res);

		res = wsend(2000, fd_a, buf, sizeof(buf), 0);
		if (-1 != res)
			printf("error: case6: %d != %d\n", -1, res);

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
		if (sizeof("test") != res)
			printf("error: case3: %d != %d\n", sizeof("test"), res);

		if (0 != strcmp(buf, "test"))
			printf("error: case4\n");

		usleep(200000);
		close(fd_c);
	}
	return 0;
}
