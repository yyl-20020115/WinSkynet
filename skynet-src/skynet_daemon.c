#include <stdio.h>
#include <sys/types.h>
#ifndef _WIN32
#include <unistd.h>
#include <sys/file.h>
#else
#include <io.h>
#include <Windows.h>
int getpid();
int daemon(int nochdir, int noclose);
#endif
#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>

#include "skynet_daemon.h"

static int check_pid(const char *pidfile) {
	int pid = 0;
	FILE *f = fopen(pidfile,"r");
	if (f == NULL)
		return 0;
	int n = fscanf(f,"%d", &pid);
	fclose(f);

	if (n !=1 || pid == 0 || pid == getpid()) {
		return 0;
	}
#ifndef _WIN32
	if (kill(pid, 0) && errno == ESRCH)
		return 0;
#else
	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
	if (NULL != hProcess) {

		if (!TerminateProcess(hProcess, 0)) {
			CloseHandle(hProcess);
			return 0;
		}
		CloseHandle(hProcess);
	}
#endif
	return pid;
}
#ifdef _WIN32
static int write_pid(const char* pidfile) {
	int pid = 0;
	HANDLE hFile = CreateFileA(pidfile, GENERIC_WRITE| GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE) {
		char buffer[256] = { 0 };
		DWORD count = 0;
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			if (ReadFile(hFile, buffer, sizeof(buffer) - 1, &count, NULL)) {
				if (count > 0) {
					if (sscanf(buffer, "%d", &pid) == 1) {

					}
				}
			}
		}
		else {
			pid = getpid();
			sprintf(buffer, "%d", pid);
			int l = (int)strlen(buffer);

			if (!WriteFile(hFile, buffer,l , &count, NULL))
			{
				pid = 0;
			}
		}
		CloseHandle(hFile);
	}

	return pid;
}

#else
static int write_pid(const char *pidfile) {
	FILE* f;
	int pid = 0;
	int fd = open(pidfile, O_RDWR | O_CREAT, 0644);
	if (fd == -1) {
		fprintf(stderr, "Can't create pidfile [%s].\n", pidfile);
		return 0;
	}
	f = fdopen(fd, "w+");
	if (f == NULL) {
		fprintf(stderr, "Can't open pidfile [%s].\n", pidfile);
		return 0;
	}

	if (flock(fd, LOCK_EX | LOCK_NB) == -1) {
		int n = fscanf(f, "%d", &pid);
		fclose(f);
		if (n != 1) {
			fprintf(stderr, "Can't lock and read pidfile.\n");
		}
		else {
			fprintf(stderr, "Can't lock pidfile, lock is held by pid %d.\n", pid);
		}
		return 0;
	}

	pid = getpid();
	if (!fprintf(f, "%d\n", pid)) {
		fprintf(stderr, "Can't write pid.\n");
		close(fd);
		return 0;
	}
	fflush(f);
	fclose(f);

	return pid;
}
#endif
static int
redirect_fds() {
	int nfd =
#ifdef _WIN32
		_open("/dev/null", O_RDWR)
#else
		open("/dev/null", O_RDWR)
#endif
		;
	if (nfd == -1) {
		perror("Unable to open /dev/null: ");
		return -1;
	}
	if (
#ifdef _WIN32
		_dup2(nfd, 0)
#else
		dup2(nfd, 0) 
#endif
		< 0) {
		perror("Unable to dup2 stdin(0): ");
		return -1;
	}
	if (
#ifdef _WIN32
		_dup2(nfd, 1)
#else
		dup2(nfd, 1)
#endif
		< 0) {
		perror("Unable to dup2 stdout(1): ");
		return -1;
	}
	if (
#ifdef _WIN32
		_dup2(nfd, 2)
#else
		dup2(nfd, 2)
#endif
		< 0) {
		perror("Unable to dup2 stderr(2): ");
		return -1;
	}

#ifdef _WIN32
	_close(nfd);
#else
	close(nfd);
#endif

	return 0;
}

int
daemon_init(const char *pidfile) {
	int pid = check_pid(pidfile);

	if (pid) {
		fprintf(stderr, "Skynet is already running, pid = %d.\n", pid);
		return 1;
	}

#ifdef __APPLE__
	fprintf(stderr, "'daemon' is deprecated: first deprecated in OS X 10.5 , use launchd instead.\n");
#else
	if (daemon(1,1)) {
		fprintf(stderr, "Can't daemonize.\n");
		return 1;
	}
#endif

	pid = write_pid(pidfile);
	if (pid == 0) {
		return 1;
	}

	if (redirect_fds()) {
		return 1;
	}

	return 0;
}

int
daemon_exit(const char *pidfile) {
	int t = 0;
#ifdef _WIN32
	t = _unlink(pidfile);
#else
	t = unlink(pidfile);
#endif
	return t;
}
