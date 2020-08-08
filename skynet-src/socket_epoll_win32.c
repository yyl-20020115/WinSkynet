#include "socket_poll_win32.h"
#include "wepoll.h"
#include <Windows.h>

#ifndef INVALID_HANDLE_VALUE
#define INVALID_HANDLE_VALUE ((HANDLE)(LONG_PTR)-1)
#endif

bool sp_invalid(poll_fd efd) {
	return efd == INVALID_HANDLE_VALUE;
}

poll_fd sp_create() {
	return epoll_create(1024);
}

void sp_release(poll_fd efd) {
	epoll_close(efd);
}

int sp_add(poll_fd efd, int sock, void *ud) {
	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.ptr = ud;
	if (epoll_ctl(efd, EPOLL_CTL_ADD, sock, &ev) == -1) {
		return 1;
	}
	return 0;
}

void sp_del(poll_fd efd, int sock) {
	epoll_ctl(efd, EPOLL_CTL_DEL, sock , NULL);
}

void sp_write(poll_fd efd, int sock, void *ud, bool enable) {
	struct epoll_event ev;
	ev.events = EPOLLIN | (enable ? EPOLLOUT : 0);
	ev.data.ptr = ud;
	epoll_ctl(efd, EPOLL_CTL_MOD, sock, &ev);
}

int sp_wait(poll_fd efd, struct event *e, int max) {
	struct epoll_event* ev = (struct epoll_event*)malloc(sizeof(struct epoll_event*)*max);
	int n = epoll_wait(efd , ev, max, -1);
	int i;
	for (i=0;i<n;i++) {
		e[i].s = ev[i].data.ptr;
		unsigned flag = ev[i].events;
		e[i].write = (flag & EPOLLOUT) != 0;
		e[i].read = (flag & (EPOLLIN | EPOLLHUP)) != 0;
		e[i].error = (flag & EPOLLERR) != 0;
		e[i].eof = false;
	}
	free(ev);
	return n;
}
int set_blocking_mode_epoll(const int socket, int is_blocking)
{
	int ret = TRUE;

#ifdef _WIN32
	u_long flags = is_blocking ? 0 : 1;
	ret = NO_ERROR == ioctlsocket(socket, FIONBIO, &flags);
#else
	int flags = fcntl(socket, F_GETFL, 0);
	if ((flags & O_NONBLOCK) && !is_blocking)
	{
		return ret;
	}
	if (!(flags & O_NONBLOCK) && is_blocking)
	{
		return ret;
	}
	ret = 0 == fcntl(socket, F_SETFL, is_blocking ? flags ^ O_NONBLOCK : flags | O_NONBLOCK);
#endif

	return ret;
}

void sp_nonblocking(int sock) {
	set_blocking_mode_epoll(sock, FALSE);
}
