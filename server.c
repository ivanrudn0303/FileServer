#include "Init.h"
#include "server_funcs.h"

#define MAX_BUF_SIZE 50000


int main (int argc, char const *argv[]) {
	struct sockaddr_in servaddr, cliaddr; 
	int sock_fd, conn_fd, num_last_packet_recv = 0;
	char* buf = (char*)malloc(sizeof(char) * MAX_BUF_SIZE);
	FILE* file;

	arguments* args = (arguments*)calloc(0, sizeof(arguments));
	if (args_parse(argv, argc, args)) {
		printf("Invalid command line arguments.\n");
		return -1;
	}
	sock_fd = create_sock_server(args);

	int len = sizeof(cliaddr);
	int id = 0;
	bool is_new_download = false;
	bool is_server_available = true;
	int curr_client_id = -1;


	while ((conn_fd = accept(sock_fd, (struct sockaddr *) &cliaddr, (socklen_t *) &len) > 0)) {
		printf("Connecton established.\n");
		id = client_authorization(conn_fd);

		if (!id) {
			id = get_id();
			give_client_id(id);
			is_new_download = true;
		} else if (id == curr_client_id)
			is_new_download = false;
		else
			is_server_available = false;

		if (is_new_download)
			file = get_new_file();

		if (is_server_available) {
			int n = num_last_packet_recv;
			download(conn_fd, buf, &num_last_packet_recv);
			write_to_file(buf, file, n);
			memset(buf, 0, sizeof(char) * MAX_BUF_SIZE);
		}

		is_server_available = true;
	}
	free(args);
	free(buf);
	close(sock_fd);
	return 0;
}
