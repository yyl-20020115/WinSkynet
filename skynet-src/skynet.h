#ifndef SKYNET_H
#define SKYNET_H

#include "skynet_malloc.h"

#include <stddef.h>
#include <stdint.h>
typedef int pid_t;

#ifdef _WIN32
int usleep(unsigned int __useconds);
unsigned int sleep(unsigned int seconds);
int clock_gettime(unsigned int clk_id, struct timespec* tv);
char* strsep(char** stringp, const char* delim);
void* dlsym(void* handle, const char* symbol);
char* dlerror(void);
int dlclose(void* handle);
void* dlopen(const char* filename, int flags);
void srandom(unsigned int seed);
long int random(void);

#define CLOCK_MONOTONIC 1
#define CLOCK_REALTIME  0
#define CLOCK_THREAD_CPUTIME_ID 3

/* The MODE argument to `dlopen' contains one of the following: */
#define RTLD_LAZY       0x00001 /* Lazy function call binding.  */
#define RTLD_NOW        0x00002 /* Immediate function call binding.  */
#define RTLD_BINDING_MASK   0x3 /* Mask of binding time value.  */
#define RTLD_NOLOAD     0x00004 /* Do not load the object.  */
#define RTLD_DEEPBIND   0x00008 /* Use deep binding.  */

/* If the following bit is set in the MODE argument to `dlopen',
   the symbols of the loaded object and its dependencies are made
   visible as if the object were linked directly into the program.  */
#define RTLD_GLOBAL     0x00100

   /* Unix98 demands the following flag which is the inverse to RTLD_GLOBAL.
	  The implementation does this by default and so we can define the
	  value to zero.  */
#define RTLD_LOCAL      0

	  /* Do not delete object when closed.  */
#define RTLD_NODELETE   0x01000

#endif


#define PTYPE_TEXT 0
#define PTYPE_RESPONSE 1
#define PTYPE_MULTICAST 2
#define PTYPE_CLIENT 3
#define PTYPE_SYSTEM 4
#define PTYPE_HARBOR 5
#define PTYPE_SOCKET 6
// read lualib/skynet.lua examples/simplemonitor.lua
#define PTYPE_ERROR 7	
// read lualib/skynet.lua lualib/mqueue.lua lualib/snax.lua
#define PTYPE_RESERVED_QUEUE 8
#define PTYPE_RESERVED_DEBUG 9
#define PTYPE_RESERVED_LUA 10
#define PTYPE_RESERVED_SNAX 11

#define PTYPE_TAG_DONTCOPY 0x10000
#define PTYPE_TAG_ALLOCSESSION 0x20000

struct skynet_context;

void skynet_error(struct skynet_context * context, const char *msg, ...);
const char * skynet_command(struct skynet_context * context, const char * cmd , const char * parm);
uint32_t skynet_queryname(struct skynet_context * context, const char * name);
int skynet_send(struct skynet_context * context, uint32_t source, uint32_t destination , int type, int session, void * msg, size_t sz);
int skynet_sendname(struct skynet_context * context, uint32_t source, const char * destination , int type, int session, void * msg, size_t sz);
int skynet_isremote(struct skynet_context *, uint32_t handle, int * harbor);
typedef int (*skynet_cb)(struct skynet_context * context, void *ud, int type, int session, uint32_t source , const void * msg, size_t sz);
void skynet_callback(struct skynet_context * context, void *ud, skynet_cb cb);
void* skynet_lalloc(void* ptr, size_t osize, size_t nsize);
uint32_t skynet_current_handle(void);
uint64_t skynet_now(void);
void skynet_debug_memory(const char *info);	// for debug use, output current service memory to stderr
#endif
