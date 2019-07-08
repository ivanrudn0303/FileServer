#include "Init.h"

#define CONN_LIMIT 10

struct conn_str
{
	int id;
	int conn_fd;

};

void* create_conn (void* conn_args) {

} 

int main (int argc, char const *argv[]) {
	struct sockaddr_in servaddr, cliaddr; 
	int sock_fd, conn_fd;

	arguments* args = (arguments*)malloc(sizeof(arguments));

	args_parse(argv, argc, args);

	sock_fd = create_sock_server(args);

	pthread_t conns [CONN_LIMIT];
	struct conn_str conn_args [CONN_LIMIT]; 
	int len = sizeof(cliaddr);
	int id = 0;

	while ((conn_fd = accept(sock_fd, (struct sockaddr_in *) &cliaddr, &len) > 0)) {
		conn_args[id].id = id;
		conn_args[id].conn_fd = conn_fd;
		pthread_create(&conns[id], NULL, create_conn, &conn_args[id]);
		id++; // not nice: when some client disconnects it should be taken in account
		      // and the next connection should refer to some of previously used conns
			  // better: have a pull of ids and new client takes the first free one
	}

	for (int i = 0; i < CONN_LIMIT; i++) 
		pthread_join(conns[i]);
	close(sock_fd);
	return 0;
}
