#if defined(__APPLE__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined (__NetBSD__)

#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/event.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

bool 
sp_invalid(poll_fd kfd) {
	return kfd == -1;
}

static poll_fd
sp_create() {
	return kqueue();
}

void
sp_release(poll_fd kfd) {
	close(kfd);
}

void 
sp_del(poll_fd kfd, int sock) {
	struct kevent ke;
	EV_SET(&ke, sock, EVFILT_READ, EV_DELETE, 0, 0, NULL);
	kevent(kfd, &ke, 1, NULL, 0, NULL);
	EV_SET(&ke, sock, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
	kevent(kfd, &ke, 1, NULL, 0, NULL);
}

int 
sp_add(poll_fd kfd, int sock, void *ud) {
	struct kevent ke;
	EV_SET(&ke, sock, EVFILT_READ, EV_ADD, 0, 0, ud);
	if (kevent(kfd, &ke, 1, NULL, 0, NULL) == -1 ||	ke.flags & EV_ERROR) {
		return 1;
	}
	EV_SET(&ke, sock, EVFILT_WRITE, EV_ADD, 0, 0, ud);
	if (kevent(kfd, &ke, 1, NULL, 0, NULL) == -1 ||	ke.flags & EV_ERROR) {
		EV_SET(&ke, sock, EVFILT_READ, EV_DELETE, 0, 0, NULL);
		kevent(kfd, &ke, 1, NULL, 0, NULL);
		return 1;
	}
	EV_SET(&ke, sock, EVFILT_WRITE, EV_DISABLE, 0, 0, ud);
	if (kevent(kfd, &ke, 1, NULL, 0, NULL) == -1 ||	ke.flags & EV_ERROR) {
		sp_del(kfd, sock);
		return 1;
	}
	return 0;
}

void 
sp_write(poll_fd kfd, int sock, void *ud, bool enable) {
	struct kevent ke;
	EV_SET(&ke, sock, EVFILT_WRITE, enable ? EV_ENABLE : EV_DISABLE, 0, 0, ud);
	if (kevent(kfd, &ke, 1, NULL, 0, NULL) == -1 || ke.flags & EV_ERROR) {
		// todo: check error
	}
}

int 
sp_wait(poll_fd kfd, struct event *e, int max) {
	struct kevent ev[max];
	int n = kevent(kfd, NULL, 0, ev, max, NULL);

	int i;
	for (i=0;i<n;i++) {
		e[i].s = ev[i].udata;
		unsigned filter = ev[i].filter;
		bool eof = (ev[i].flags & EV_EOF) != 0;
		e[i].write = (filter == EVFILT_WRITE) && (!eof);
		e[i].read = (filter == EVFILT_READ);
		e[i].error = (ev[i].flags & EV_ERROR) != 0;
		e[i].eof = eof;
	}

	return n;
}

void
sp_nonblocking(int fd) {
	int flag = fcntl(fd, F_GETFL, 0);
	if ( -1 == flag ) {
		return;
	}

	fcntl(fd, F_SETFL, flag | O_NONBLOCK);
}

#endif
