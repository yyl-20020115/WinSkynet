#ifndef socket_poll_h
#define socket_poll_h
#include <stdbool.h>
#ifdef _WIN32
#include <cstdint>
#endif
typedef int poll_fd;
typedef uintptr_t SOCKET;

struct event {
	void * s;
	bool read;
	bool write;
	bool error;
	bool eof;
};

static bool sp_invalid(poll_fd fd);
static poll_fd sp_create();
static void sp_release(poll_fd fd);
static int sp_add(poll_fd fd, SOCKET sock, void *ud);
static void sp_del(poll_fd fd, SOCKET sock);
static void sp_write(poll_fd fd, SOCKET sock, void *ud, bool enable);
static int sp_wait(poll_fd fd, struct event *e, int max);
static void sp_nonblocking(SOCKET sock);

#ifdef __linux__
#include "socket_epoll.h"
#endif

#if defined(__APPLE__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined (__NetBSD__)
#include "socket_kqueue.h"
#endif

#endif
