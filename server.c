#include "Init.h"
#include "server_funcs.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int main (int argc, char const *argv[]) {
	struct sockaddr_in servaddr, cliaddr;
	int sock_fd, conn_fd;
	storage_elem** storage = (storage_elem**)malloc(sizeof(storage_elem*) * CONN_LIMIT);
	int i = 0;

	for (i = 0; i < CONN_LIMIT; i++) {
		storage[i] = (storage_elem*)malloc(sizeof(storage_elem));
		memset(storage[i], 0, sizeof(storage_elem));
	}

	arguments* args = (arguments*)malloc(sizeof(arguments));

	args_parse(argv, argc, args);

	sock_fd = create_sock_server(args);

	int len = sizeof(cliaddr);
	int id = 0;
	int conn_count = 0;

	while ((conn_fd = accept(sock_fd, (struct sockaddr_in *) &cliaddr, &len) > 0)) {
		if (id = client_authorization(conn_fd)) {
			resume_download(conn_fd, id, storage);
		} else {
			id = ++conn_count;
			start_download(conn_fd, id, storage);
		}
	}
	free(args);
	close(sock_fd);
	return 0;
}
