#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#ifndef SIGHUP
#define SIGHUP 1
#endif
#ifndef SIGINT
#define SIGINT 2
#endif
extern int skynet_main(int argc, char* argv[]);
extern void handle_int(int signal);

BOOL WINAPI ctrlhandler(DWORD fdwctrltype)
{
	switch (fdwctrltype)
	{
	case CTRL_C_EVENT:
		printf("Quting skynet...\n\n");
		handle_int(SIGINT);
		return TRUE;
	default:
		return FALSE;
	}
}

int main(int argc, char* argv[])
{
	SetConsoleCtrlHandler(ctrlhandler, TRUE);

    return skynet_main(argc, argv);
}
