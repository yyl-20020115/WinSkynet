#pragma once
#ifdef _WIN32
typedef int pid_t;
pid_t getpid();
#define O_NONBLOCK     00004000
#endif
