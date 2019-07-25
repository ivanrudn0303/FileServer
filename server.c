#include "server_funcs.h"

#define MAX_BUF_SIZE 50000


int main (int argc, char const *argv[]) {
	struct sockaddr_in servaddr, cliaddr; 
	int sock_fd, conn_fd, num_last_packet_recv = 0;
	char* buf = (char*)malloc(sizeof(char) * MAX_BUF_SIZE);
	int file;
	size_t file_len = 0;
	int download_res = -1;
	arguments* args = (arguments*)calloc(0, sizeof(arguments));

	if (args_parse(argv, argc, args)) {
		printf("Invalid command line arguments.\n");
		free(args);
		free(buf);
		return -1;
	}

	if ((sock_fd = create_sock_server(args)) == -1) {
		printf("Creation of socket failed.\n");
		free(args);
		free(buf);
		return -1;
	}

	int len = sizeof(cliaddr);
	int id = 0;
	bool is_server_available = true;
	bool finish = false;
	int curr_client_id = -1;
	file = open(args->file, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);

	//REDO: reject all other clients
	//send them message with 0 id
	do { 
		conn_fd = accept(sock_fd, (struct sockaddr *) &cliaddr, (socklen_t *) &len);

		if (conn_fd == -1) {
			printf("accept() failed, error: %s\n", strerror(errno));
			close(file);
			close(sock_fd);
			free(buf);
			free(args);
			return -1;
		}

		printf("Connecton established.\n");
		id = client_authorization(conn_fd, &file_len);

		if (!id && -1 == curr_client_id) {
			id = get_id();
			if (give_client_id(id, 0, conn_fd)) {
				close(file);
				close(sock_fd);
				free(buf);
				free(args);
				return -1;
			} // add num_last_packet_recv in param

			curr_client_id = id;
			printf("curr id: %d\n", curr_client_id);
		} else if (id != curr_client_id) {

			if (give_client_id(0, 0, conn_fd)) {
				close(file);
				close(sock_fd);
				free(buf);
				free(args);
				return -1;
			} //send to client message with id 0

			is_server_available = false;
		} else if (id == curr_client_id) {
			printf("start from %d\n", num_last_packet_recv * SIZE_OF_DATA);
			give_client_id(id, num_last_packet_recv * SIZE_OF_DATA, conn_fd);
		}

		printf("SERVER: current id: %d, id: %d\n", curr_client_id, id);

		if (is_server_available) {
			printf("Download starts...\n");
			download_res = download(conn_fd, &num_last_packet_recv, file, &file_len, &finish);
			printf("Download is completed. File of %d length is received.\n", download_res);
		}

		is_server_available = true;
		close(conn_fd);
		if (finish) {
			break;
		}
	} while (conn_fd > 0);
	free(args);
	free(buf);
	close(sock_fd);
	return 0;
}
