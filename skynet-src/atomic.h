#ifndef SKYNET_ATOMIC_H
#define SKYNET_ATOMIC_H
#ifndef _WIN32
#define ATOM_CAS(ptr, oval, nval) __sync_bool_compare_and_swap(ptr, oval, nval)
#define ATOM_CAS_POINTER(ptr, oval, nval) __sync_bool_compare_and_swap(ptr, oval, nval)
#define ATOM_INC(ptr) __sync_add_and_fetch(ptr, 1)
#define ATOM_FINC(ptr) __sync_fetch_and_add(ptr, 1)
#define ATOM_DEC(ptr) __sync_sub_and_fetch(ptr, 1)
#define ATOM_FDEC(ptr) __sync_fetch_and_sub(ptr, 1)
#define ATOM_ADD(ptr,n) __sync_add_and_fetch(ptr, n)
#define ATOM_SUB(ptr,n) __sync_sub_and_fetch(ptr, n)
#define ATOM_AND(ptr,n) __sync_and_and_fetch(ptr, n)
#else
#include <intrin0.h>
#define ATOM_CAS(ptr, oval, nval) _InterlockedCompareExchange(ptr, oval, nval)
#define ATOM_CAS_POINTER(ptr, oval, nval) _InterlockedCompareExchange(ptr, oval, nval)
//NOTICE:TODO: need checks
#define ATOM_INC(ptr) _InterlockedIncrement(ptr)
#define ATOM_FINC(ptr) _InterlockedExchangeAdd(ptr, 1)
#define ATOM_DEC(ptr) _InterlockedDecrement(ptr)
#define ATOM_FDEC(ptr) _InterlockedExchange(ptr, *ptr-1)
#define ATOM_ADD(ptr,n) _InterlockedExchangeAdd(ptr, n)
#define ATOM_SUB(ptr,n) _InterlockedExchange(ptr, (*ptr - n))
#define ATOM_AND(ptr,n) _InterlockedAnd(ptr, n)
#endif
#endif
