#ifdef _WIN32
#include "skynet.h"
#include <time.h>
#include <Windows.h>
#if 0
int sigaction(int signum, const struct sigaction* act,
    struct sigaction* oldact) 
{
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
#endif
void* dlopen(const char* filename, int flags)
{
    return LoadLibraryA(filename);
}
int dlclose(void* handle)
{
    CloseHandle(handle);
    return 0;
}
static char szBuf[256] = { 0 };
static char* GetLastErrorMessage() {
    LPVOID lpMsgBuf;
    DWORD dw = GetLastError();
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0, NULL);
    memset(szBuf, 0, sizeof(szBuf));
    strncpy(szBuf, lpMsgBuf, sizeof(szBuf) - 1);
    LocalFree(lpMsgBuf);
    return szBuf;
}
char* dlerror(void) {
    return GetLastErrorMessage();
}
void* dlsym(void* handle, const char* symbol) {
    return GetProcAddress((HMODULE)handle,symbol);
}
LARGE_INTEGER getFILETIMEoffset()
{
    SYSTEMTIME s = { 0 };
    FILETIME f = { 0 };
    LARGE_INTEGER t = { 0 };

    s.wYear = 1970;
    s.wMonth = 1;
    s.wDay = 1;
    s.wHour = 0;
    s.wMinute = 0;
    s.wSecond = 0;
    s.wMilliseconds = 0;
    SystemTimeToFileTime(&s, &f);
    t.QuadPart = f.dwHighDateTime;
    t.QuadPart <<= 32;
    t.QuadPart |= f.dwLowDateTime;
    return (t);
}
pid_t getpid()
{
    return GetCurrentProcessId();
}
int clock_gettime(unsigned int clk_id, struct timespec* tv)
{
    LARGE_INTEGER t = { 0 };
    FILETIME f = { 0 };
    double                  microseconds = 0.0;
    static LARGE_INTEGER    offset = { 0 };
    static double           frequencyToMicroseconds = 0.0;
    static int              initialized = 0;
    static BOOL             usePerformanceCounter = 0;

    if (!initialized) {
        LARGE_INTEGER performanceFrequency = { 0 };
        initialized = 1;
        usePerformanceCounter = QueryPerformanceFrequency(&performanceFrequency);
        if (usePerformanceCounter) {
            QueryPerformanceCounter(&offset);
            frequencyToMicroseconds = (double)performanceFrequency.QuadPart / 1000000.0;
        }
        else {
            offset = getFILETIMEoffset();
            frequencyToMicroseconds = 10.0;
        }
    }
    if (usePerformanceCounter) QueryPerformanceCounter(&t);
    else {
        GetSystemTimeAsFileTime(&f);
        t.QuadPart = f.dwHighDateTime;
        t.QuadPart <<= 32;
        t.QuadPart |= f.dwLowDateTime;
    }

    t.QuadPart -= offset.QuadPart;
    microseconds = (double)t.QuadPart / frequencyToMicroseconds;
    t.QuadPart = (LONGLONG)microseconds;
    tv->tv_sec = t.QuadPart / 1000000;
    tv->tv_nsec = t.QuadPart % 1000000;
    return 0;
}
long int random(void) 
{
    return rand();
}
unsigned int sleep(unsigned int seconds) 
{
    Sleep(seconds * 1000);
    return 0;
}
void srandom(unsigned int seed)
{
    srand(seed);
}
int daemon(int nochdir, int noclose) {
    //NOTICE: put myself into background
    return 0;   
}
char* strsep(char** stringp, const char* delim)
{
    char* start = *stringp;
    char* p = 0;
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
    LARGE_INTEGER litmp = { 0 };
    LONGLONG QPart1 = 0L, QPart2 = 0L;
    double dfMinus = 0.0, dfFreq = 0.0, dfTim = 0.0;
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
int pipe(SOCKET fd[2])
{
    SECURITY_ATTRIBUTES sa = { 0 };

    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL; 
    sa.bInheritHandle = TRUE;
    return CreatePipe((PHANDLE)&fd[0], (PHANDLE)&fd[1], &sa, 0) ? 0 : -1;
}
#endif
