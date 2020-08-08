#ifndef skynet_malloc_h
#define skynet_malloc_h

#include <stddef.h>

//#ifdef _WIN32
//#include "jemalloc.h"
//#define skynet_malloc je_malloc
//#define skynet_calloc je_calloc
//#define skynet_realloc je_realloc
//#define skynet_free je_free
//#define skynet_memalign je_memalign
//#define skynet_aligned_alloc je_aligned_alloc
//#define skynet_posix_memalign je_posix_memalign
//#else
//#define skynet_malloc malloc
//#define skynet_calloc calloc
//#define skynet_realloc realloc
//#define skynet_free free
//#define skynet_memalign memalign
//#define skynet_aligned_alloc aligned_alloc
//#define skynet_posix_memalign posix_memalign
//#endif

void * skynet_malloc(size_t sz);
void * skynet_calloc(size_t nmemb,size_t size);
void * skynet_realloc(void *ptr, size_t size);
void skynet_free(void *ptr);
char * skynet_strdup(const char *str);
void * skynet_lalloc(void *ptr, size_t osize, size_t nsize);	// use for lua
void * skynet_memalign(size_t alignment, size_t size);
void * skynet_aligned_alloc(size_t alignment, size_t size);
int skynet_posix_memalign(void **memptr, size_t alignment, size_t size);

#endif
