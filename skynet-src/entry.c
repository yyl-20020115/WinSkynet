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

BOOL ctrlhandler(DWORD fdwctrltype)
{
	switch (fdwctrltype)
	{
		// handle the ctrl-c signal.
	case CTRL_C_EVENT:
		printf("Quting skynet...\n\n");
		handle_int(SIGINT);
		return(TRUE);
		// ctrl-close: confirm that the user wants to exit.
		//case CTRL_CLOSE_EVENT:
		//	printf("ctrl-close event\n\n");
		//	return(true);
		//	// pass other signals to the next handler.
		//case CTRL_BREAK_EVENT:
		//	printf("ctrl-break event\n\n");
		//	return false;
		//case CTRL_LOGOFF_EVENT:
		//	printf("ctrl-logoff event\n\n");
		//	return false;
		//case CTRL_SHUTDOWN_EVENT:
		//	printf("ctrl-shutdown event\n\n");
		//	return false;
	default:
		return FALSE;
	}
}

int main(int argc, char* argv[])
{
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)ctrlhandler, TRUE);

    return skynet_main(argc, argv);
}
