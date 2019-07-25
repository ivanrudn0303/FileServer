#include "server_funcs.h"


int main (int argc, char const *argv[]) {
	struct sockaddr_in servaddr, cliaddr; 
	int sock_fd, conn_fd, num_last_packet_recv = 0;
	int file;
	size_t file_len = 0;
	int download_res = -1;
	arguments* args = (arguments*)calloc(0, sizeof(arguments));

	if (args_parse(argv, argc, args)) {
		printf("Invalid command line arguments.\n");
		free(args);
		return -1;
	}

	if ((sock_fd = create_sock_server(args)) == -1) {
		printf("Creation of socket failed.\n");
		free(args);
		return -1;
	}

	free(args);
	int len = sizeof(cliaddr);
	int id = 0;
	bool is_server_available = true;
	bool finish = false;
	int curr_client_id = -1;
	int error_code = 0;
	file = open(args->file, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);

	//REDO: reject all other clients
	//send them message with 0 id
	do { 
		conn_fd = accept(sock_fd, (struct sockaddr *) &cliaddr, (socklen_t *) &len);

		if (conn_fd == -1) {
			printf("accept() failed, error: %s\n", strerror(errno));
			close(file);
			close(sock_fd);
			return -1;
		}

		printf("Connecton established.\n");
		if ((error_code = client_authorization(conn_fd, &id, &file_len))) {
			close(file);
			close(sock_fd);
			return error_code;
		}

		if (!id) {
			id = get_id();
			if ((error_code = give_client_id(id, conn_fd))) {
				close(file);
				close(sock_fd);
				return error_code;
			} 

			curr_client_id = id;
		} else if (id != curr_client_id) {

			if ((error_code = give_client_id(0, conn_fd))) {
				close(file);
				close(sock_fd);
				return error_code;
			}

			is_server_available = false;
		} else if (id == curr_client_id) {
			if ((error_code = give_client_id(id, conn_fd))) {
				close(file);
				close(sock_fd);
				return error_code;
			}
		}

		printf("SERVER: current id: %d, id: %d\n", curr_client_id, id);

		if (is_server_available) {
			printf("Download starts...\n");
			if ((error_code = download(conn_fd, &num_last_packet_recv, file, &file_len, &finish))) {
				close(file);
				close(sock_fd);				
			}
			printf("Download is completed. File of %d length is received.\n", file_len);
		}

		is_server_available = true;
		close(conn_fd);

		if (finish) {
			break;
		}

	} while (conn_fd > 0);

	close(sock_fd);
	return 0;
}
