#ifdef _WIN32
#include "skynet.h"
#include <Windows.h>
int sigaction(int signum, const struct sigaction* act,
    struct sigaction* oldact) {
    return 0;
}
int sigemptyset(sigset_t* set)
{
    return 0;
}

int sigfillset(sigset_t* set)
{
    return 0;
}


unsigned int sleep(unsigned int seconds) {
    Sleep(seconds * 1000);
    return 0;
}
char* dlerror(void) {
    return 0;
}
void* dlsym(void* handle, const char* symbol) {
    return 0;
}
void* dlopen(const char* filename, int flags)
{
    return 0;
}

int dlclose(void* handle)
{
    return 0;
}

long int random(void) {
    return rand();
}

void srandom(unsigned int seed)
{
    srand(seed);
}
int clock_gettime(unsigned int clk_id, struct timespec* tp) {
    //TODO:
    return 0;
}
int daemon(int nochdir, int noclose) {
    //NOTICE: put myself into background
    return 0;   
}
char* strsep(char** stringp, const char* delim)
{
    char* start = *stringp;
    char* p;

    p = (start != NULL) ? strpbrk(start, delim) : NULL;

    if (p == NULL)
    {
        *stringp = NULL;
    }
    else
    {
        *p = '\0';
        *stringp = p + 1;
    }

    return start;
}

int usleep(unsigned int __useconds) {
    LARGE_INTEGER litmp;
    LONGLONG QPart1, QPart2;
    double dfMinus, dfFreq, dfTim;
    QueryPerformanceFrequency(&litmp);
    dfFreq = (double)litmp.QuadPart;
    QueryPerformanceCounter(&litmp);
    QPart1 = litmp.QuadPart;
    do {

        QueryPerformanceCounter(&litmp);
        QPart2 = litmp.QuadPart;
        dfMinus = (double)(QPart2 - QPart1);
        dfTim = dfMinus / dfFreq;
    } while (dfTim < (0.000001 * __useconds));

    return 0;
}

#endif