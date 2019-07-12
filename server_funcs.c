#include "Init.h"
#include "server_funcs.h"
#include <stdlib.h>

int get_storage_index (int id) {
	return id % CONN_LIMIT;
}

storage_elem* get_storage_elem (storage_elem* head, int id) {
	if (!head) return NULL;
	do {
		if (head->id == id) return head;
		head = head->next_elem;
	} while (head->next_elem)
	return NULL;
}


int client_authorization(int conn_fd) {
	/*
		receive message with id from client
	*/
}


void resume_download(int conn_fd, int id, storage_elem* storage) {

}


void start_download(int conn_fd, int id, storage_elem* storage) {

}
