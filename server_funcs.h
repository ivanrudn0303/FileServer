#ifndef SERVER_FUNCS
#define SERVER_FUNCS

struct storage_elem
{
	struct storage_elem* next_elem;
	int id;
	void* data;
};

typedef struct storage_elem storage_elem;

int client_authorization(int conn_fd, int id);
void resume_download(int conn_fd, int id, storage_elem* storage);
void start_download(int conn_fd, int id, storage_elem* storage);

#endif
