#include "Init.h"
#include "server_funcs.h"
#include "Talk.h"
#include <fcntl.h>

#define MAX_BUF_SIZE 50000


int main (int argc, char const *argv[]) {
	struct sockaddr_in servaddr, cliaddr; 
	int sock_fd, conn_fd, num_last_packet_recv = 0;
	char* buf = (char*)malloc(sizeof(char) * MAX_BUF_SIZE);
	int file;
	size_t file_len = 0;

	arguments* args = (arguments*)calloc(0, sizeof(arguments));
	if (args_parse(argv, argc, args)) {
		printf("Invalid command line arguments.\n");
		return -1;
	}
	sock_fd = create_sock_server(args);

	int len = sizeof(cliaddr);
	int id = 0;
	bool is_server_available = true;
	bool finish = false;
	int curr_client_id = -1;
	file = open(args->file, O_WRONLY);

	//REDO: reject all other clients
	//send them message with 0 id
	while ((conn_fd = accept(sock_fd, (struct sockaddr *) &cliaddr, (socklen_t *) &len) > 0)) {
		printf("Connecton established.\n");
		id = client_authorization(conn_fd, &file_len);

		if (!id) {
			id = get_id();
			give_client_id(id, conn_fd); // add num_last_packet_recv in param
			curr_client_id = id;
		} else if (id != curr_client_id) {
			give_client_id(0, conn_fd); //send to client message with id 0
			is_server_available = false;
		} else if (id == curr_client_id) {
			give_client_id(id, conn_fd);
		}

		if (is_server_available) {
			download(conn_fd, &num_last_packet_recv, file, &file_len);
		}

		is_server_available = true;
		close(conn_fd);
		if (finish) {
			break;
		}
	}
	free(args);
	free(buf);
	close(sock_fd);
	return 0;
}
