#ifndef SKYNET_ATOMIC_H
#define SKYNET_ATOMIC_H
#ifndef _WIN32
#define ATOM_CAS32(ptr, oval, nval) __sync_bool_compare_and_swap(ptr, oval, nval)
#define ATOM_CAS64(ptr, oval, nval) __sync_bool_compare_and_swap(ptr, oval, nval)
#define ATOM_CAS_POINTER(ptr, oval, nval) __sync_bool_compare_and_swap(ptr, oval, nval)
#define ATOM_INC(ptr) __sync_add_and_fetch(ptr, 1)
#define ATOM_INC16(ptr) __sync_add_and_fetch(ptr, 1)
#define ATOM_INC64(ptr) __sync_add_and_fetch(ptr, 1)
#define ATOM_FINC(ptr) __sync_fetch_and_add(ptr, 1)
#define ATOM_DEC(ptr) __sync_sub_and_fetch(ptr, 1)
#define ATOM_DEC16(ptr) __sync_sub_and_fetch(ptr, 1)
#define ATOM_DEC64(ptr) __sync_sub_and_fetch(ptr, 1)
#define ATOM_ADD(ptr,n) __sync_add_and_fetch(ptr, n)
#define ATOM_ADD64(ptr,n) __sync_add_and_fetch(ptr, n)
#define ATOM_SUB(ptr,n) __sync_sub_and_fetch(ptr, n)
#define ATOM_SUB64(ptr,n) __sync_sub_and_fetch(ptr, n)
#define ATOM_AND(ptr,n) __sync_and_and_fetch(ptr, n)
#else
#include <intrin0.h>
#define ATOM_CAS32(ptr, oval, nval) _InterlockedCompareExchange(ptr, oval, nval)
#define ATOM_CAS64(ptr, oval, nval) _InterlockedCompareExchange64(ptr, oval, nval)
#define ATOM_CAS_POINTER(ptr, oval, nval) _InterlockedCompareExchangePointer((void volatile *)ptr, oval, nval)
//NOTICE: need checks
#define ATOM_INC(ptr) _InterlockedIncrement(ptr)
#define ATOM_INC16(ptr) _InterlockedIncrement16(ptr)
#define ATOM_INC64(ptr) _InterlockedIncrement64(ptr)
#define ATOM_FINC(ptr) (_InterlockedIncrement(ptr)-1)
#define ATOM_DEC(ptr) _InterlockedDecrement(ptr)
#define ATOM_DEC16(ptr) _InterlockedDecrement16(ptr)
#define ATOM_DEC64(ptr) _InterlockedDecrement64(ptr)
#define ATOM_ADD(ptr,n) _InterlockedExchangeAdd(ptr, n)
#define ATOM_ADD64(ptr,n) _InterlockedExchangeAdd64(ptr, n)
#define ATOM_SUB(ptr,n) _InterlockedExchange(ptr, (*ptr - n))
#define ATOM_SUB64(ptr,n) _InterlockedExchange64(ptr, (*ptr - n))
#define ATOM_AND(ptr,n) _InterlockedAnd(ptr, n)
#endif
#endif
