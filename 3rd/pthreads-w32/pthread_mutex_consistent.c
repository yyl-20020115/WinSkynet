#if !defined(_UWIN)
  /*#   include <process.h> */
#endif
#include "pthread.h"
#include "implement.h"

INLINE
int
ptw32_robust_mutex_inherit(pthread_mutex_t* mutex)
{
    int result;
    pthread_mutex_t mx = *mutex;
    ptw32_robust_node_t* robust = mx->robustNode;

    switch ((LONG)PTW32_INTERLOCKED_COMPARE_EXCHANGE_LONG(
        (PTW32_INTERLOCKED_LONGPTR)&robust->stateInconsistent,
        (PTW32_INTERLOCKED_LONG)PTW32_ROBUST_INCONSISTENT,
        (PTW32_INTERLOCKED_LONG)-1 /* The terminating thread sets this */))
    {
    case -1L:
        result = EOWNERDEAD;
        break;
    case (LONG)PTW32_ROBUST_NOTRECOVERABLE:
        result = ENOTRECOVERABLE;
        break;
    default:
        result = 0;
        break;
    }

    return result;
}

/*
 * The next two internal support functions depend on only being
 * called by the thread that owns the robust mutex. This enables
 * us to avoid additional locks.
 * Any mutex currently in the thread's robust mutex list is held
 * by the thread, again eliminating the need for locks.
 * The forward/backward links allow the thread to unlock mutexes
 * in any order, not necessarily the reverse locking order.
 * This is all possible because it is an error if a thread that
 * does not own the [robust] mutex attempts to unlock it.
 */

INLINE
void
ptw32_robust_mutex_add(pthread_mutex_t* mutex, pthread_t self)
{
    ptw32_robust_node_t** list;
    pthread_mutex_t mx = *mutex;
    ptw32_thread_t* tp = (ptw32_thread_t*)self.p;
    ptw32_robust_node_t* robust = mx->robustNode;

    list = &tp->robustMxList;
    mx->ownerThread = self;
    if (NULL == *list)
    {
        robust->prev = NULL;
        robust->next = NULL;
        *list = robust;
    }
    else
    {
        robust->prev = NULL;
        robust->next = *list;
        (*list)->prev = robust;
        *list = robust;
    }
}

INLINE
void
ptw32_robust_mutex_remove(pthread_mutex_t* mutex, ptw32_thread_t* otp)
{
    ptw32_robust_node_t** list;
    pthread_mutex_t mx = *mutex;
    ptw32_robust_node_t* robust = mx->robustNode;

    list = &(((ptw32_thread_t*)mx->ownerThread.p)->robustMxList);
    mx->ownerThread.p = otp;
    if (robust->next != NULL)
    {
        robust->next->prev = robust->prev;
    }
    if (robust->prev != NULL)
    {
        robust->prev->next = robust->next;
    }
    if (*list == robust)
    {
        *list = robust->next;
    }
}


int
pthread_mutex_consistent(pthread_mutex_t* mutex)
{
    pthread_mutex_t mx = *mutex;
    int result = 0;

    /*
     * Let the system deal with invalid pointers.
     */
    if (mx == NULL)
    {
        return EINVAL;
    }

    if (mx->kind >= 0
        || (PTW32_INTERLOCKED_LONG)PTW32_ROBUST_INCONSISTENT != PTW32_INTERLOCKED_COMPARE_EXCHANGE_LONG(
            (PTW32_INTERLOCKED_LONGPTR)&mx->robustNode->stateInconsistent,
            (PTW32_INTERLOCKED_LONG)PTW32_ROBUST_CONSISTENT,
            (PTW32_INTERLOCKED_LONG)PTW32_ROBUST_INCONSISTENT))
    {
        result = EINVAL;
    }

    return (result);
}

