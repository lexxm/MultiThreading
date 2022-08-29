#include "stdafx.h"
#include "BaseMultiThreadFunctionality.h"
#include "WaitForMultipleObjectException.h"
#include "xtimer.h"

extern double timeLockWFMO;

void crossplatformSleep( unsigned int msec )
{
#ifdef _WIN32
	Sleep( msec );
#else
	usleep( msec * 1000 );
#endif
}

void waitForMultipleObjectNew( int* valueForSynchroniztion, int countTasks, MUTEX& mutexForSynchronize )
{
	//TODO: понять почему используется cond value in mutex
#ifdef _DEBUG
	int maxDelay = 10000;
#else
	int maxDelay = 5000;
#endif
	CXTimer tDelay;
	while ( true )
	{
		if ( tDelay.GetElapsedMSec() > maxDelay )
			throw WaitForMultipleObjectException();

		CXTimer t0;
		lockMutex( mutexForSynchronize );
		timeLockWFMO += t0.GetElapsedMSec();

		if ( *valueForSynchroniztion == countTasks )
		{
			unlockMutex( mutexForSynchronize );
			break;
		}
		unlockMutex( mutexForSynchronize );
		crossplatformSleep( 1 );
	}
}
