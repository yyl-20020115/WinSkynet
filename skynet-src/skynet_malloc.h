#ifndef skynet_malloc_h
#define skynet_malloc_h
#include <stddef.h>

#ifdef _WIN32

void* skynet_malloc(size_t sz);
void* skynet_calloc(size_t nmemb, size_t size);
void* skynet_realloc(void* ptr, size_t size);
void skynet_free(void* ptr);
//void* skynet_memalign(size_t alignment, size_t size);
//void* skynet_aligned_alloc(size_t alignment, size_t size);
//int skynet_posix_memalign(void** memptr, size_t alignment, size_t size);
void* skynet_lalloc(void* ptr, size_t osize, size_t nsize);	// use for lua
char* skynet_strdup(const char* str);

#else
#include <stdlib.h>
#define skynet_malloc(sz) malloc(sz)
#define skynet_calloc(nmemb,size) calloc(nmemb,size)
#define skynet_realloc(ptr,size) realloc(ptr,size)
#define skynet_free(ptr) free(ptr)
#define skynet_memalign memalign
#define skynet_aligned_alloc aligned_alloc
#define skynet_posix_memalign posix_memalign
void* skynet_lalloc(void* ptr, size_t osize, size_t nsize);	// use for lua
char* skynet_strdup(const char* str);


#endif
#endif
