#include "stdafx.h"
#include "crossplatformmutex.h"


#ifdef _WIN32
void initMutex( HANDLE& m )
{
	m = CreateMutex( NULL, FALSE, NULL );
	if ( NULL == m )
		throw SoftwareException( "CouldNotInitMutex" );
}

void lockMutex( HANDLE m )
{
	WaitForSingleObject( m, INFINITE );
}

void unlockMutex( HANDLE m )
{
	ReleaseMutex( m );
}

void freeMutex( HANDLE m )
{
	CloseHandle( m );
}
#else

void initMutex( pthread_mutex_t& m )
{
	m = PTHREAD_MUTEX_INITIALIZER;
	if ( pthread_mutex_init( &m, NULL ) != 0 )
			throw SoftwareException( "CouldNotInitMutex" );
}
void lockMutex( pthread_mutex_t& m )
{
	pthread_mutex_lock( &m );
}

void unlockMutex( pthread_mutex_t& m )
{
	pthread_mutex_unlock( &m );
}

void freeMutex( pthread_mutex_t& m )
{
	pthread_mutex_destroy( &m );
}
#endif
