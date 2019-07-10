#include <sys/types.h>
#include <sys/socket.h>

//timeout in milliseconds
ssize_t wsend(int timeout, int sockfd, const void *buf, size_t len, int flags);
ssize_t wrecv(int timeout, int sockfd, const void *buf, size_t len, int flags);
