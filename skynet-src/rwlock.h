#ifndef SKYNET_RWLOCK_H
#define SKYNET_RWLOCK_H

#ifdef _WIN32
#define USE_PTHREAD_LOCK
#endif

#ifndef USE_PTHREAD_LOCK
#ifdef _WIN32
#include <intrin.h>
#include <intrin0.h>
#endif
struct rwlock {
	int write;
	int read;
};

static inline void
rwlock_init(struct rwlock *lock) {
	lock->write = 0;
	lock->read = 0;
}

static inline void
rwlock_rlock(struct rwlock *lock) {
	for (;;) {
		while(lock->write) {
#ifndef _WIN32
			__sync_synchronize();
#else
			_ReadWriteBarrier();
#endif
		}
#ifndef _WIN32
		__sync_add_and_fetch(&lock->read,1);
#else
		_InterlockedIncrement(&lock->read);
#endif
		if (lock->write) {
#ifndef _WIN32
			__sync_sub_and_fetch(&lock->read,1);
#else
			_InterlockedDecrement(&lock->read);
#endif
		} else {
			break;
		}
	}
}

static inline void
rwlock_wlock(struct rwlock *lock) {
#ifndef _WIN32
	while (__sync_lock_test_and_set(&lock->write,1)) {}
#else
	while (_interlockedbittestandset(&lock->write, 1)) {}
#endif
	while(lock->read) {
#ifndef _WIN32
		__sync_synchronize();
#else
		_ReadWriteBarrier();
#endif
	}
}

static inline void
rwlock_wunlock(struct rwlock *lock) {
#ifndef _WIN32
	 __sync_lock_release(&lock->write);
#else
	_InterlockedExchange(&lock->write, 0);
#endif
}

static inline void
rwlock_runlock(struct rwlock *lock) {
#ifndef _WIN32
	__sync_sub_and_fetch(&lock->read, 1);
#else
	_InterlockedDecrement(&lock->read);
#endif
}

#else

#include <pthread.h>

// only for some platform doesn't have __sync_*
// todo: check the result of pthread api

struct rwlock {
	pthread_rwlock_t lock;
};

static inline void
rwlock_init(struct rwlock *lock) {
	pthread_rwlock_init(&lock->lock, NULL);
}

static inline void
rwlock_rlock(struct rwlock *lock) {
	 pthread_rwlock_rdlock(&lock->lock);
}

static inline void
rwlock_wlock(struct rwlock *lock) {
	 pthread_rwlock_wrlock(&lock->lock);
}

static inline void
rwlock_wunlock(struct rwlock *lock) {
	pthread_rwlock_unlock(&lock->lock);
}

static inline void
rwlock_runlock(struct rwlock *lock) {
	pthread_rwlock_unlock(&lock->lock);
}

#endif

#endif
