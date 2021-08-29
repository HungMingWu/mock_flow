#define _GNU_SOURCE
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

static int (*real_connect)(int sockfd, const struct sockaddr *addr,
                   socklen_t addrlen) = NULL;

static void connect_init(void)
{
    real_connect = dlsym(RTLD_NEXT, "connect");
    if (NULL == real_connect) {
        fprintf(stderr, "Error in `dlsym`: %s\n", dlerror());
    }
}

int connect(int sockfd, const struct sockaddr *addr,
                   socklen_t addrlen)
{
	if (real_connect == NULL) {
		connect_init();
	}
	struct ifreq ifr;

	memset(&ifr, 0, sizeof(ifr));
	snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), "tun0");
	if (setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, (void *)&ifr, sizeof(ifr)) < 0) {
		printf("bind to tun1 failed\n");
		return -1;
	}
	return real_connect(sockfd, addr, addrlen);
}

