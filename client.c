#include "Init.h"
#include "client_funcs.h"

int main(int argc, char const *argv[])
{
	struct sockaddr_in servaddr; 
	int sock_fd, conn_fd;
	bool is_download_in_progress = true;

	arguments* args = (arguments*)malloc(sizeof(arguments));
	args_parse(argv, argc, args);
	sock_fd = create_sock_client();

	bzero(&servaddr, sizeof(servaddr)); 
 	
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr = args->addr; 
    servaddr.sin_port = args->port; 

	while (is_download_in_progress) {
		if (connect(sock_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) { 
        	perror("connection with the server failed...\n"); 
        	exit(-1); 
    	} else 
        	printf("connected to the server..\n");
        	is_download_in_progress = false;
     	/*
     		Not implemented yet
     	*/
	}
	close(sock_fd);
	return 0;
}