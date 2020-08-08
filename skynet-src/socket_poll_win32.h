#ifndef socket_poll_win32_h
#define socket_poll_win32_h

#include <stdbool.h>
#include "wepoll.h"
typedef HANDLE poll_fd;
struct event {
	void * s;
	bool read;
	bool write;
	bool error;
	bool eof;
};
bool sp_invalid(poll_fd fd);
poll_fd sp_create();
void sp_release(poll_fd fd);
int sp_add(poll_fd fd, int sock, void *ud);
void sp_del(poll_fd fd, int sock);
void sp_write(poll_fd fd, int sock, void *ud, bool enable);
int sp_wait(poll_fd fd, struct event *e, int max);
void sp_nonblocking(int sock);
#endif
