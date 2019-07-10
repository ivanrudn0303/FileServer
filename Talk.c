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
                gettimeofday(&tv, NULL);
                mcurr = tv.tv_sec * 1000 + tv.tv_usec / 1000;
        }
        return size;
}

