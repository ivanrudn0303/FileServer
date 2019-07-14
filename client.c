#include "client_funcs.h"
#include "Init.h"
#include "Talk.h"
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>



int main(char* argv[], int argc)
{
	int file;
	uint32_t id = 0;
	unsigned int pos = 0;
	arguments params;
	struct stat info;

	if (args_parse(argv, argc, &params))
	{
		printf("Wrong arguments\n");
		return 1;
	}
	file = open(params.file, O_RDONLY);
	if(file < 0)
	{
		printf("Could not open file: %s\n", params.file);
		return 1;
	}
	if(fstat(file, &info) < 0)
	{
		printf("Could not get info about file: %s\n", params.file);
		close(file);
		return 1;
	}
	pos = info.st_size;

	while (true)
	{
		bool trans_succ = true;
		int sock_fd = create_sock_client();
		struct sockaddr_in strct = {AF_INET, params.port, params.addr};
		if (sock_fd < 0)
		{
			printf("Could not create socket\n");
			close(file);
			return 1;
		}
		if (connect(sock_fd, (struct sockaddr*)&strct, sizeof(strct)) < 0)
		{
			printf("Could not connect to server\n");
			close(sock_fd);
			close(file);
			return 1;
		}
		if (start_session(&id, &pos))
		{
			printf("Server refused transmission\n");
			close(sock_fd);
			close(file);
			return 2;
		}
		if (lseek(file, pos, SEEK_SET) != pos)
		{
			printf("Could not set position in file\n");
			close(sock_fd);
                        close(file);
                        return 1;
		}

		// main loop
		for (; pos < info.st_size; pos += SIZE_OF_DATA)
		{
			if (transmission(file, sock_fd, (info.st_size - pos) % SIZE_OF_DATA + 1))
			{
				trans_succ = false;
				break;
			}
		}
		if (trans_succ && (0 == finnish(sock_fd)))
			break;
		close(sock_fd);
	}
	close(file);
	return 0;
}
