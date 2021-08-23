#ifndef _TARGET_H_
#define _TARGET_H_

#if OV_SYNC_NTMUTEX

#include <windows.h>

#define TLSF_MLOCK_T            HANDLE
#define TLSF_CREATE_LOCK(l)     *(l) = CreateMutexA(NULL, FALSE, NULL)
#define TLSF_DESTROY_LOCK(l)    CloseHandle(*(l)); *(l) = NULL;
#define TLSF_ACQUIRE_LOCK(l)    WaitForSingleObject(*(l), INFINITE)
#define TLSF_RELEASE_LOCK(l)    ReleaseMutex(*(l))

#else

#include <pthread.h>

#define TLSF_MLOCK_T            pthread_mutex_t
#define TLSF_CREATE_LOCK(l)     pthread_mutex_init (l, NULL)
#define TLSF_DESTROY_LOCK(l)    pthread_mutex_destroy(l)
#define TLSF_ACQUIRE_LOCK(l)    pthread_mutex_lock(l)
#define TLSF_RELEASE_LOCK(l)    pthread_mutex_unlock(l)

#endif

#endif
