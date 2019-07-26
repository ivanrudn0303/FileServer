#include "Talk.h"
#include <errno.h>
#include <poll.h>
#include <stddef.h>
#include <sys/time.h>

ssize_t wsend(int timeout, int sockfd, const void *buf, size_t len, int flags)
{
	struct timeval tv;
	unsigned long long mstart, mcurr;
	int ret;
	size_t size = 0;
	struct pollfd pfd = {sockfd, POLLOUT, 0};

	gettimeofday(&tv, NULL);
	mstart = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	mcurr = mstart;

	while (mcurr < mstart + timeout)
	{
		ret = poll(&pfd, 1, mstart + timeout - mcurr);
		if (ret == 0)
			return size;
		else if ((ret < 0) || (pfd.revents != POLLOUT))
			return -1;

		ret = send(sockfd, (char*)buf + size, len - size, flags | MSG_DONTWAIT);
		if ((ret < 0) && ((errno != EAGAIN) || (errno != EWOULDBLOCK)))
			return -1;
		else
			size += ret;
                if (len == size)
                        return size;
		gettimeofday(&tv, NULL);
		mcurr = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	}
	return size;
}

ssize_t wrecv(int timeout, int sockfd, const void *buf, size_t len, int flags)
{
        struct timeval tv;
        unsigned long long mstart, mcurr;
        int ret;
        size_t size = 0;
        struct pollfd pfd = {sockfd, POLLIN, 0};

        gettimeofday(&tv, NULL);
        mstart = tv.tv_sec * 1000 + tv.tv_usec / 1000;
        mcurr = mstart;

        while (mcurr < mstart + timeout)
        {
                ret = poll(&pfd, 1, mstart + timeout - mcurr);
                if (ret == 0)
                        return size;
                else if ((ret < 0) || (pfd.revents != POLLIN))
                        return -1;

                ret = recv(sockfd, (char*)buf + size, len - size, flags | MSG_DONTWAIT);
                if ((ret < 0) && ((errno != EAGAIN) || (errno != EWOULDBLOCK)))
                        return -1;
                else
                        size += ret;
                if (len == size)
                        return size;
                gettimeofday(&tv, NULL);
                mcurr = tv.tv_sec * 1000 + tv.tv_usec / 1000;
        }
        return size;
}

int error_handler(int conn_fd, char* error_string) {
        printf("%s\n", error_string);
        int err_str_len = strlen(error_string) + sizeof('\0');
        char buf [1024];
        ((uint32_t*)buf)[0] = ERROR_MESSAGE;
        ((uint32_t*)buf)[1] = err_str_len;
        memcpy(buf + sizeof(uint32_t) * 2, error_string, err_str_len);  
        int res = wsend(100, conn_fd, buf, sizeof(message) + err_str_len, 0);

        if (!res) {
                printf("ERROR_MSG_ERROR: Error occurred while sending error message...\n");
                return -1;
        } else if (res == -1) {
                printf("ERROR_MSG_ERROR: %s.\n", strerror(errno));
                return -1;
        }

        return 0;
}
