#pragma once

#ifdef _WIN32
typedef HANDLE MUTEX;
#else
typedef pthread_mutex_t MUTEX;
#endif

#ifdef _WIN32
void initMutex( HANDLE& m );
void lockMutex( HANDLE m );
void unlockMutex( HANDLE m );
void freeMutex( HANDLE m );
#else
void initMutex( pthread_mutex_t& m );
void lockMutex( pthread_mutex_t& m );
void unlockMutex( pthread_mutex_t& m );
void freeMutex( pthread_mutex_t& m );
#endif
